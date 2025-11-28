/*
 * Copyright (c) 2005, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */


/**
 * @file
 * @copyright  Copyright (c) 2005, Swedish Institute of Computer Science.
 * @license    This file is released under the 3-Clause BSD License.\n
 *             https://opensource.org/license/bsd-3-clause
 *
 * @author     Adam Dunkels <adam@sics.se>
 * @authors    Marco Bacchi <marco@bacchi.at>,
 *
 * @brief Process management and event scheduler for MyOs.
 *
 * @details
 * This file defines the API for managing processes and scheduling events in MyOs.
 * The implementation extends the concepts from the Contiki operating system,
 * adapting them to the specific needs and architecture of MyOs.
 *
 * Processes in MyOs are lightweight, protothread-based tasks that can be
 * scheduled to run concurrently. The process scheduler handles the execution
 * of these processes and the dispatching of events to them.
 *
 * Key features and differences from Contiki:
 * - Uses MyOs-specific data structures like ring buffers and (singly or doubly)
 *   linked lists for managing processes and events.
 * - Enhanced configurability, allowing the use of different list types for
 *   process management.
 * - Integration with MyOs's protothread implementation and local continuation
 *   concepts.
 * - A flexible event system that supports both synchronous and asynchronous
 *   event posting, suitable for various application scenarios, including
 *   interrupt handling.
 * - Detailed statistics collection for monitoring and debugging, depending
 *   on compile-time configuration (CONFIG_MYOS_STATISTICS).
 *
 * Event System and Process Polling:
 * - MyOs employs an event system where processes can communicate and signal
 *   each other through events.
 * - Processes can post events either synchronously or asynchronously. Asynchronous
 *   events are queued and processed in the main loop context.
 * - The process polling mechanism is primarily triggered from Interrupt Service
 *   Routines (ISR) and is executed in the main loop context, allowing processes
 *   to handle interrupt-driven events in a safe manner.
 *
 * @note This implementation diverges from Contiki by removing certain legacy
 *       constructs and focusing on a more modular and clean design, tailored to
 *       the specific requirements of MyOs.
 */

#ifndef PROCESS_H_
#define PROCESS_H_


#include "pt.h"
#include "ringbuffer.h"
#include <stdbool.h>
#include <stdint.h>
#include "rtimer.h"

/**
 * @file
 * @brief Configuration for the type of list used for the running process list in myos.
 *
 * @details
 * This section of the code allows for the configuration of the type of list used to manage
 * the running processes in the operating system. The type of list can be specified in the
 * myos configuration file. By default, a singly linked list is used.
 *
 * The configuration can be modified to use a doubly linked list by defining the
 * MYOSCONF_DLIST macro in the configuration file. If MYOSCONF_DLIST is defined, the
 * corresponding doubly linked list implementation will be used; otherwise, the default
 * singly linked list implementation (slist) is utilized.
 *
 * The list type affects how processes are stored, accessed, and manipulated during
 * execution. The choice between a singly or doubly linked list can have implications
 * for performance and memory usage, depending on the specific needs and constraints of
 * the system.
 *
 * Singly Linked List Implementation:
 * - A singly linked list (slist) is used when MYOSCONF_DLIST is not defined.
 * - The singly linked list provides a simple and memory-efficient way to manage the
 *   running processes with basic operations like initialization, addition, deletion,
 *   and iteration over the list.
 *
 * Example of configuring a singly linked list for process management:
 * \code{.c}
 * #define MYOSCONF_PROC_LIST_TYPE MYOSCONF_SLIST
 * \endcode
 *
 * @note Currently, only the singly linked list implementation is supported. The
 *       implementation for the doubly linked list (dlist) is planned for future
 *       development.
 */
#if defined(CONFIG_MYOS_PROC_LIST_TYPE_SLIST)
#include "slist.h"
typedef slist_t plist_t;
typedef slist_node_t plist_node_t;
#define PLIST_NODE_TYPE                              SLIST_NODE_TYPE
#define plist_init(listptr)                          slist_init(listptr)
#define plist_erase(listptr,nodeptr)                 slist_erase(listptr,nodeptr)
#define plist_next(listptr,nodeptr)                  slist_next(listptr,nodeptr)
#define plist_push_front(listptr,nodeptr)            slist_push_front(listptr,nodeptr)
#define plist_prev(listptr,nodeptr)                  slist_prev(listptr,nodeptr)
#define plist_foreach(listptr,iterator)              slist_foreach(listptr,iterator)
#elif defined(CONFIG_MYOS_PROC_LIST_TYPE_DLIST)
#include "dlist.h"
typedef dlist_t plist_t;
typedef dlist_node_t plist_node_t;
#define PLIST_NODE_TYPE                              DLIST_NODE_TYPE
#define plist_init(listptr)                          dlist_init(listptr)
#define plist_erase(listptr,nodeptr)                 dlist_erase(listptr,nodeptr)
#define plist_next(listptr,nodeptr)                  dlist_next(listptr,nodeptr)
#define plist_push_front(listptr,nodeptr)            dlist_push_front(listptr,nodeptr)
#define plist_prev(listptr,nodeptr)                  dlist_prev(listptr,nodeptr)
#define plist_foreach(listptr,iterator)              dlist_foreach(listptr,iterator)
#else
#error "No process list type defined."
#endif





/**
 * @def PROCESS_EVENT_START
 * @brief Event indicating that a process is starting.
 *
 * @details
 * This event is posted to a process when it is starting. It can be used by the process to perform
 * initialization tasks. The process receives this event only once when it is first started.
 */
#define PROCESS_EVENT_START     0

/**
 * @def PROCESS_EVENT_POLL
 * @brief Event for polling a process.
 *
 * @details
 * This event is used to poll a process, typically to request it to update its state.
 * The process receives this event when the system or another process explicitly requests
 * a status update or immediate attention from the process.
 */
#define PROCESS_EVENT_POLL      1

/**
 * @def PROCESS_EVENT_CONTINUE
 * @brief Event to signal a process to continue its operation.
 *
 * @details
 * This event is used to signal a process to continue its execution, usually after being
 * suspended or waiting for a condition. It is a way to resume the process's activity
 * programmatically.
 */
#define PROCESS_EVENT_CONTINUE  2

/**
 * @def PROCESS_EVENT_TIMEOUT
 * @brief Event indicating a timeout has occurred.
 *
 * @details
 * This event is posted to a process when a specified time duration has elapsed.
 * It is often used for handling time-related tasks, such as periodic updates or
 * timeouts in response to external events.
 */
#define PROCESS_EVENT_TIMEOUT   3

/**
 * @def PROCESS_EVENT_EXIT
 * @brief Event indicating that a process is exiting.
 *
 * @details
 * This event is sent to a process when it is about to exit or terminate. It allows
 * the process to perform any necessary cleanup before it stops executing. Processes
 * can also use this event to signal other processes about their termination.
 */
#define PROCESS_EVENT_EXIT      4

typedef struct process_t process_t;
typedef struct process_event_t process_event_t;
typedef uint8_t process_event_id_t;



/**
 * @typedef process_thread_t
 * @brief Defines the function type for process threads.
 *
 * @param process Pointer to the process structure.
 * @param evt Pointer to the process event.
 * @return Integer status of the process thread after execution.
 *
 * @details
 * Function pointer type for process thread functions. Each process in the system is represented
 * as a function of this type. The function takes a process and an event as arguments and returns
 * an integer status. This design allows for event-driven behavior within each process.
 */
typedef int(*process_thread_t)(process_t *process, process_event_t *evt);

/**
 * @struct process_t
 * @brief Represents a process in the system.
 *
 * @details
 * This structure defines a process in the operating system. Each process is a protothread
 * with additional features such as event handling and interaction with other processes.
 *
 * @var process_t::PLIST_NODE_TYPE
 * Node type for including this process in a process list.
 * @var process_t::thread
 * The protothread function representing the process logic.
 * @var process_t::data
 * Pointer to user-defined data specific to this process.
 * @var process_t::pt
 * Protothread state for this process.
 * @var process_t::maxslicetime
 * (Optional, with CONFIG_MYOS_STATISTICS) Records the maximum time slice used by this process.
 * @var process_t::pollreq
 * Flag indicating whether this process has requested to be polled.
 */
struct process_t {
   PLIST_NODE_TYPE;
   process_thread_t thread;
   void* data;
   pt_t pt;

#if defined(CONFIG_MYOS_STATISTICS)
   rtimer_timespan_t maxslicetime;
#endif

   bool pollreq;
} ;

/**
 * @struct process_event_t
 * @brief Represents an event in the system, used for inter-process communication.
 *
 * @details
 * This structure is used to define events in the operating system, facilitating
 * communication and synchronization between processes.
 *
 * @var process_event_t::id
 * The unique identifier of the event.
 * @var process_event_t::data
 * Pointer to data associated with the event.
 * @var process_event_t::from
 * Pointer to the process that posted this event.
 * @var process_event_t::to
 * Pointer to the process that the event is directed to.
 */
struct process_event_t {
   process_event_id_t id;
   void *data;
   process_t *from;
   process_t *to;
};


/**
 * @var process_current
 * @brief Pointer to the currently executing process.
 *
 * @details
 * This global pointer keeps track of the process that is currently running in the system.
 * It is used by the process scheduler to manage the execution context and to switch
 * between different processes.
 */
extern process_t *process_current;

/**
 * @def PROCESS_THIS()
 * @brief Macro to obtain the currently executing process.
 *
 * @return The currently executing process.
 *
 * @details
 * A convenience macro that provides access to the currently executing process anywhere
 * within the process code.
 */
#define PROCESS_THIS() process_current

/**
 * @def PROCESS_CONTEXT_BEGIN(processptr)
 * @brief Begins a new context for a specified process.
 *
 * @param processptr Pointer to the process for which the context is to be set.
 *
 * @details
 * This macro is used to switch the context to a specified process. It saves the current
 * process context and sets the new context to the specified process. It should be used
 * in conjunction with PROCESS_CONTEXT_END to ensure the original context is restored.
 */
#define PROCESS_CONTEXT_BEGIN(processptr) \
      do{ \
         process_t *process_backup; \
         process_backup = PROCESS_THIS(); \
         PROCESS_THIS() = processptr

/**
 * @def PROCESS_CONTEXT_END()
 * @brief Ends the current process context and restores the previous context.
 *
 * @details
 * This macro restores the previously saved process context. It should be used after
 * PROCESS_CONTEXT_BEGIN to ensure that the original process context is correctly
 * restored after the temporary context switch.
 */
#define PROCESS_CONTEXT_END() \
         PROCESS_THIS() = process_backup; \
      }while(0)

/**
 * @def PROCESS(name, threadname)
 * @brief Declares and initializes a process.
 *
 * @param name The name of the process variable.
 * @param threadname The name of the process thread function.
 *
 * @details
 * This macro declares a new process and its associated thread function. It initializes
 * the process structure and sets up the process thread. The process is ready to be
 * started with process_start().
 */
#define PROCESS(name,threadname) \
   int process_thread_##threadname(process_t *process, process_event_t *evt);  \
   process_t name = {{0},process_thread_##threadname,0,{0},false}

/**
 * @def PROCESS_EXTERN(name)
 * @brief Declares an external reference to a process.
 *
 * @param name The name of the process.
 *
 * @details
 * This macro is used to declare an external process that is defined in another file.
 * This allows the process to be referenced and used across different modules.
 */
#define PROCESS_EXTERN(name) \
      extern process_t name

/**
 * @def PROCESS_DATA()
 * @brief Accesses the user-defined data associated with the current process.
 *
 * @return The user-defined data.
 *
 * @details
 * This macro provides a convenient way to access the user-defined data field of the
 * currently executing process.
 */
#define PROCESS_DATA()            (process->data)

/**
 * @def PROCESS_PT()
 * @brief Accesses the protothread state of the current process.
 *
 * @return The protothread state.
 *
 * @details
 * This macro provides access to the protothread state of the currently executing process.
 * It is useful for manipulating the control flow of the process.
 */
#define PROCESS_PT()              (process->pt)

/**
 * @def PROCESS_EVENT_DATA()
 * @brief Accesses the data associated with the current event.
 *
 * @return The event data.
 *
 * @details
 * This macro provides access to the data field of the current event being processed by
 * the process.
 */
#define PROCESS_EVENT_DATA()      (evt->data)

/**
 * @def PROCESS_EVENT_ID()
 * @brief Accesses the identifier of the current event.
 *
 * @return The event identifier.
 *
 * @details
 * This macro provides access to the identifier of the current event being processed by
 * the process. It is useful for conditional handling of different event types.
 */
#define PROCESS_EVENT_ID()        (evt->id)

/**
 * @def EXTERN_PROCESS(name)
 * @brief Extern declaration of a process.
 *
 * @param name The name of the process.
 *
 * @details
 * This macro is used for declaring an external reference to a process that is defined
 * in another module or file. It allows the process to be accessed from different parts
 * of the system.
 */
#define EXTERN_PROCESS(name) \
   extern process_t name

/**
 * @def PROCESS_THREAD(name)
 * @brief Declares the function implementing a process thread.
 *
 * @param name The name of the process thread function.
 *
 * @details
 * This macro is used to declare a function that will serve as the thread (main logic)
 * for a process. The function will be called with a pointer to the process and the
 * current event as parameters.
 */
#define PROCESS_THREAD(name) \
int process_thread_##name(process_t *process, process_event_t *evt)

/**
 * @def PROCESS_RESPOND(evtid, dataptr)
 * @brief Respond to an event within a process.
 *
 * @param evtid The event ID to respond to.
 * @param dataptr Pointer to the data associated with the event response.
 *
 * @details
 * This macro is a convenient wrapper to send a response to an event from within a process.
 * It simplifies the process of event handling and responding in process threads.
 */
#define PROCESS_RESPOND(evtid,dataptr) \
   process_respond(evt,evtid,dataptr)

/**
 * @def PROCESS_INIT(processptr, threadname)
 * @brief Initialize a process with a specific thread function.
 *
 * @param processptr Pointer to the process to be initialized.
 * @param threadname The name of the thread function for the process.
 *
 * @details
 * This macro initializes a process and associates it with a specific thread function.
 * It sets up the necessary state for the process to run and handle events using the
 * given thread function.
 */
#define PROCESS_INIT(processptr,threadname) \
   process_init(processptr, process_thread_##threadname)

/**
 * @def PROCESS_IS_RUNNING(processptr)
 * @brief Check if a process is currently running.
 *
 * @param processptr Pointer to the process.
 * @return True if the process is running, False otherwise.
 *
 * @details
 * This macro checks if a given process is currently running by examining its protothread
 * state. It is useful for determining the status of processes in the system.
 */
#define PROCESS_IS_RUNNING(processptr) \
   (PT_IS_RUNNING(&(processptr)->pt))

/**
 * @def PROCESS_BEGIN()
 * @brief Marks the beginning of a process.
 *
 * @details
 * This macro should be placed at the start of a process thread function. It initializes
 * the protothread state and checks if the process should exit immediately, which is
 * useful for handling the PROCESS_EVENT_EXIT event.
 */
#define PROCESS_BEGIN() \
   if(PROCESS_EVENT_ID() == PROCESS_EVENT_EXIT) {PT_EXIT(&PROCESS_PT());} \
   PT_BEGIN(&PROCESS_PT())

/**
 * @def PROCESS_END()
 * @brief Marks the end of a process.
 *
 * @details
 * This macro should be placed at the end of a process thread function. It marks the
 * termination of the process and exits the protothread.
 */
#define PROCESS_END() PT_END(&PROCESS_PT())

/**
 * @def PROCESS_WAIT_EVENT(evtid)
 * @brief Blocks the process until a specific event occurs.
 *
 * @param evtid The event ID to wait for.
 *
 * @details
 * This macro is used within a process thread to block the process's execution until
 * a specific event with the given ID is posted to it.
 */
#define PROCESS_WAIT_EVENT(evtid) PT_YIELD_UNTIL(&PROCESS_PT(), PROCESS_EVENT_ID() == evtid)

/**
 * @def PROCESS_WAIT_EVENT_UNTIL(cond)
 * @brief Blocks the process until a condition is true.
 *
 * @param cond The condition to wait for.
 *
 * @details
 * This macro is used within a process thread to pause the process's execution until
 * the specified condition becomes true.
 */
#define PROCESS_WAIT_EVENT_UNTIL(cond) PT_YIELD_UNTIL(&PROCESS_PT(), cond)

/**
 * @def PROCESS_WAIT_ANY_EVENT()
 * @brief Blocks the process until any event is received.
 *
 * @details
 * This macro suspends the process's execution until it receives any event. It is
 * useful when the process needs to wait for an unspecified event before continuing.
 */
#define PROCESS_WAIT_ANY_EVENT() PT_YIELD(&PROCESS_PT())

/**
 * @def PROCESS_SUSPEND()
 * @brief Temporarily suspends the process.
 *
 * @details
 * This macro is used to suspend the current process. It posts a PROCESS_EVENT_CONTINUE
 * event to itself and then waits for this event before continuing execution. This
 * allows other processes or tasks to run while the current process is suspended.
 */
#define PROCESS_SUSPEND() \
   do{ \
      process_post(PROCESS_THIS(),PROCESS_EVENT_CONTINUE,NULL); \
      PROCESS_WAIT_EVENT(PROCESS_EVENT_CONTINUE); \
   }while(0)


/**
 * @def PROCESS_EXITHANDLER(handler)
 * @brief Specify an action to be performed when a process exits.
 *
 * @param handler The action (code block or function call) to be executed upon process exit.
 *
 * @details
 * This macro allows specifying a custom action that should be executed when a process
 * receives a PROCESS_EVENT_EXIT event. This is typically used for cleanup and resource
 * deallocation tasks that a process must perform before exiting.
 *
 * @note
 * This macro must be placed immediately before the PROCESS_BEGIN() macro within the
 * process thread function. It ensures that the specified action is performed only if
 * the process is about to exit.
 *
 * Usage example:
 * @code
 * PROCESS_THREAD(my_process_thread, ev, data) {
 *   PROCESS_EXITHANDLER(my_cleanup_function());
 *   PROCESS_BEGIN();
 *   ...
 *   PROCESS_END();
 * }
 * @endcode
 *
 * @hideinitializer
 */
#define PROCESS_EXITHANDLER(handler) if(PROCESS_EVENT_ID() == PROCESS_EVENT_EXIT) { handler; }


/**
 * @brief Initializes the process management system.
 *
 * @details
 * This function sets up the initial state for the process management system. It
 * initializes the list of running processes and the event queue used for inter-process
 * communication. After calling this function, the process system is ready to start
 * and manage processes.
 *
 * The function performs the following initializations:
 * - Sets up an empty list for managing running processes.
 * - Initializes the event queue used for posting events to processes.
 * - Resets the current process pointer to NULL, indicating that no process is currently
 *   being executed.
 *
 * @note
 * This function should be called at system startup, before any processes are started
 * or any events are posted.
 */
void process_init(void);

/**
 * @brief Starts a process.
 *
 * @param process Pointer to the process to be started.
 * @param data Pointer to data to be passed to the process.
 * @return True if the process was successfully started, False otherwise.
 *
 * @details
 * This function starts a specified process. If the process is already running, it will
 * not be started again. The function sets the process's data, initializes its protothread,
 * and adds it to the list of running processes. It then posts a PROCESS_EVENT_START event
 * to the process to inform it that it has been started.
 *
 * @note
 * The process must be properly defined and initialized before calling this function.
 * The `data` parameter allows passing a custom data structure to the process, which can
 * be accessed within the process using the PROCESS_DATA() macro.
 *
 * Debugging messages are printed at the start and end of the function if the DEBUG_PROCESS
 * macro is defined.
 *
 * Example usage:
 * @code
 * process_t my_process;
 * my_data_t my_data = { ... };
 * process_start(&my_process, &my_data);
 * @endcode
 */
bool process_start(process_t *process, void* data);

/**
 * @brief Exits a process.
 *
 * @param process Pointer to the process to be terminated.
 * @return True if the process was successfully terminated, False otherwise.
 *
 * @details
 * This function is used to terminate a process. It first checks if the process
 * is currently running. If the process is not running, the function returns false.
 * Otherwise, it posts a PROCESS_EVENT_EXIT event to the process, which triggers
 * the process to exit. The process will be automatically removed from the list
 * of running processes.
 *
 * @note
 * This function should be used with caution as it stops the process's execution
 * and removes it from the process management system. Once a process has exited,
 * it cannot be restarted without reinitializing and starting it again.
 *
 * Debugging messages are printed before exiting and after successful termination
 * if the DEBUG_PROCESS macro is defined.
 *
 * Example usage:
 * @code
 * process_exit(&my_process);
 * @endcode
 */
bool process_exit(process_t *process);

/**
 * @brief Posts an event to a process.
 *
 * @param to Pointer to the target process to which the event is posted.
 * @param evtid The event identifier.
 * @param data Pointer to data associated with the event.
 * @return True if the event was successfully posted, False if the event queue is full.
 *
 * @details
 * This function is used to post an event to a specific process or to all processes
 * if the target is PROCESS_BROADCAST. The function checks if the event queue is full
 * before posting the event. If the queue is full, the function returns false and
 * the event is not posted. If there is space in the queue, the function creates an
 * event structure, fills it with the event data, and then pushes it onto the event queue.
 *
 * @note
 * If the `CONFIG_MYOS_STATISTICS` flag is defined, the function also updates the statistics
 * related to the maximum count of events in the queue.
 *
 * Debugging messages are printed with information about the event posting if
 * the DEBUG_PROCESS macro is defined.
 *
 * Example usage:
 * @code
 * process_post(&my_process, MY_EVENT, my_data);
 * @endcode
 */
bool process_post(process_t *to, process_event_id_t evtid, void* data);

/**
 * @brief Posts an event to a process synchronously.
 *
 * @param to Pointer to the target process to which the event is posted.
 * @param evtid The event identifier.
 * @param data Pointer to data associated with the event.
 * @return True if the event was successfully delivered, False otherwise.
 *
 * @details
 * This function is used to post an event synchronously to a specific process.
 * Unlike `process_post`, this function immediately calls `process_deliver_event`
 * to handle the event. This means that the event handling is done in the same
 * context as the caller of `process_post_sync`, and not deferred.
 *
 * The function constructs a `process_event_t` structure with the provided
 * parameters, including the sender of the event (`PROCESS_THIS()`), the target
 * process, the event identifier, and the associated data.
 *
 * @note
 * This function should be used when immediate handling of the event is required,
 * and the caller can afford to wait until the event has been processed.
 *
 * Debugging messages are printed with information about the synchronous event
 * posting if the DEBUG_PROCESS macro is defined.
 *
 * Example usage:
 * @code
 * process_post_sync(&my_process, MY_EVENT, my_data);
 * @endcode
 */
bool process_post_sync(process_t *to, process_event_id_t evtid, void* data);

/**
 * @brief Runs the process scheduler.
 *
 * @return The number of events waiting to be processed.
 *
 * @details
 * This function runs the process scheduler of the operating system. It handles
 * all pending poll requests and then delivers events from the event queue to
 * the corresponding processes.
 *
 * Poll requests are typically triggered by interrupts and are processed first.
 * Each process that has a pending poll request will be handled, and the
 * poll request flag will be reset. After handling all poll requests, the
 * function processes the next event in the event queue, if any.
 *
 * The function returns the count of remaining events in the queue plus any
 * outstanding global poll requests. This can be used to determine if there
 * are more events to process or if the system can go to a low-power mode,
 * for example.
 *
 * @note
 * If `CONFIG_MYOS_STATISTICS` is defined, the function measures the processing time
 * and updates the maximum processing time statistic.
 *
 * Example usage:
 * @code
 * while(1) {
 *   process_run();
 *   // Additional operations such as entering low-power mode.
 * }
 * @endcode
 */
int process_run(void);

/**
 * @brief Requests a poll for a specific process.
 *
 * @param process Pointer to the process to be polled.
 *
 * @details
 * This function is used to request a poll for a specific process. When called,
 * it sets the `pollreq` flag of the specified process to true, indicating that
 * the process should be polled. Additionally, it sets a global flag
 * `process_global_pollreq` to true, indicating that at least one process needs
 * to be polled.
 *
 * The actual polling is performed in the `process_run` function, where all
 * processes with the `pollreq` flag set will be processed.
 *
 * @note
 * This function is typically used by interrupt service routines or other parts
 * of the system that need to trigger a process to run outside of its normal
 * scheduling.
 *
 * Debugging messages are printed with information about the process polling
 * request if the DEBUG_PROCESS macro is defined.
 *
 * Example usage:
 * @code
 * process_poll(&my_process);
 * @endcode
 */
void process_poll(process_t *process);

#endif /* PROCESS_H_ */


















