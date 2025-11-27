/*! \copyright
 
   https://opensource.org/license/mit/

   Copyright 2013-2023 Marco Bacchi <marco@bacchi.at>

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use,
   copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following
   conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.
*/

/*!
 * @file ptimer.h
 * @copyright  Copyright 2013-2023 Marco Bacchi <marco@bacchi.at>
 * @license    This file is released under the MIT License.\n
 *             https://opensource.org/licenses/MIT
 *
 * @brief Abstract Base Class for Process Timers in MyOS.
 *
 * @details ptimer in MyOS serves as an abstract base class for specialized timers like etimers (event timers) and ctimers (callback timers),
 *          providing foundational functionalities for these derived classes. As MyOS is a cooperative and non-preemptive operating system,
 *          ptimers are dependent on the OS's scheduling and cannot be used for bare-metal programming. They enable scheduled tasks
 *          to execute after a set interval, facilitating timed operations in an event-driven manner. This mechanism is crucial in
 *          MyOS for managing timed events and callbacks efficiently within the constraints of a cooperative environment.
 *
 * Usage Example:
 * // Example usage would typically involve etimers or ctimers, as ptimer is an abstract class.
 */
#ifndef PTIMER_H_
#define PTIMER_H_

#include "myos.h"
#include <stdbool.h>

#if (MYOSCONF_PTIMER_LIST_TYPE == MYOSCONF_DLIST)

// Definitions for double-linked list based ptimer list
#include "dlist.h"
typedef dlist_t ptlist_t;
typedef dlist_node_t ptlist_node_t;
#define PTLIST_NODE_TYPE                              DLIST_NODE_TYPE
#define ptlist_init(listptr)                          dlist_init(listptr)
#define ptlist_erase(listptr,nodeptr)                 dlist_erase(listptr,nodeptr)
#define ptlist_next(listptr,nodeptr)                  dlist_next(listptr,nodeptr)
#define ptlist_push_front(listptr,nodeptr)            dlist_push_front(listptr,nodeptr)
#define ptlist_prev(listptr,nodeptr)                  dlist_prev(listptr,nodeptr)
#define ptlist_foreach(listptr,iterator)              dlist_foreach(listptr,iterator)
#define ptlist_find(listptr,nodeptr)                  dlist_findlistptr,nodeptr)
#define ptlist_begin(listptr)                         dlist_begin(listptr)
#define ptlist_end(listptr)                           dlist_end(listptr)
#define ptlist_empty(listptr)                         dlist_empty(listptr)

#else

// Definitions for single-linked list based ptimer list
#include "slist.h"
typedef slist_t ptlist_t;
typedef slist_node_t ptlist_node_t;
#define PTLIST_NODE_TYPE                              SLIST_NODE_TYPE
#define ptlist_init(listptr)                          slist_init(listptr)
#define ptlist_erase(listptr,nodeptr)                 slist_erase(listptr,nodeptr)
#define ptlist_next(listptr,nodeptr)                  slist_next(listptr,nodeptr)
#define ptlist_push_front(listptr,nodeptr)            slist_push_front(listptr,nodeptr)
#define ptlist_prev(listptr,nodeptr)                  slist_prev(listptr,nodeptr)
#define ptlist_foreach(listptr,iterator)              slist_foreach(listptr,iterator)
#define ptlist_find(listptr,nodeptr)                  slist_findlistptr,nodeptr)
#define ptlist_begin(listptr)                         slist_begin(listptr)
#define ptlist_end(listptr)                           slist_end(listptr)
#define ptlist_empty(listptr)                         slist_empty(listptr)
#define ptlist_foreach(listptr,iterator)              slist_foreach(listptr,iterator)
#define ptlist_size(listptr)                          slist_size(listptr)
#endif


PROCESS_EXTERN(ptimer_process);


typedef struct ptimer_t ptimer_t;


/*!
 * @typedef ptimer_handler_t
 * @brief Function pointer type for process timer callback functions.
 * @param[in] timer Pointer to the process timer invoking the handler.
 *
 * Usage Example:
 * @code
 * void my_ptimer_handler(ptimer_t *timer) {
 *     // Handler logic for the process timer
 * }
 * @endcode
 */
typedef void (*ptimer_handler_t) (ptimer_t *data);


/*!
 * @struct ptimer_t
 * @brief Process Timer (ptimer) structure in MyOS.
 * @details This structure defines a process timer in MyOS, which includes a timer instance, a handler function,
 *          and a running status. Process timers are typically used for scheduling tasks in an operating system context,
 *          allowing for efficient management of time-based events.
 *
 * @var ptimer_t::timer
 *      The underlying timer instance.
 * @var ptimer_t::handler
 *      Callback function to be executed when the timer expires.
 * @var ptimer_t::running
 *      Indicates whether the timer is currently active.
 *
 * Usage Example:
 * @code
 *     ptimer_t my_ptimer;
 *     ptimer_start(&my_ptimer, 1000, my_ptimer_handler); // Start the ptimer with a 1000-unit duration
 *     if (ptimer_expired(&my_ptimer)) {
 *         // Code to execute when the ptimer expires
 *     }
 * @endcode
 */
struct ptimer_t{
   PTLIST_NODE_TYPE;
   timer_t timer;
   ptimer_handler_t handler;
   bool running;
};

/*!
 * @brief Initializes the process timer module.
 * @details This macro starts the process timer (ptimer) process in MyOS. It's used to initialize
 *          the ptimer system by starting the ptimer_process, which is responsible for handling
 *          all the process timers. This initialization is crucial for the ptimer system to function
 *          correctly and should be called at system startup.
 *
 * Usage Example:
 * @code
 *     // At system initialization
 *     ptimer_module_init(); // Initialize the process timer module
 * @endcode
 */
#define ptimer_module_init() if(!PROCESS_IS_RUNNING(&ptimer_process))process_start(&ptimer_process, NULL)

/*!
 * @brief Processes pending process timers.
 * @details This function is responsible for handling and processing all pending process timers (ptimers) in MyOS.
 *          It checks if there are any ptimers pending and if the next stop time has passed. If so, it triggers
 *          the ptimer_poll_evt event to handle the expired timers. Additionally, if MYOSCONF_STATS is enabled,
 *          it records the time taken to process the timers for performance monitoring.
 *
 * Usage Example:
 * @code
 *     // Call this function periodically to process pending ptimers
 *     ptimer_processing();
 * @endcode
 */
void ptimer_processing(void);

/*!
 * @brief Starts a process timer with a specified duration and handler.
 * @details This function is used to start a process timer in MyOS. It sets the timer's handler function,
 *          initializes its duration using the provided timespan, and then adds the timer to the list of running timers.
 *          This is a key function for scheduling time-based actions within the MyOS, allowing for precise control
 *          over timing and execution of tasks. It combines the functionality of setting a timer and registering it
 *          for processing in a single operation.
 * @param[in] ptimer Pointer to the process timer to start.
 * @param[in] span Duration for the timer.
 * @param[in] handler Function to be called when the timer expires.
 *
 * Usage Example:
 * @code
 *     ptimer_t my_ptimer;
 *     ptimer_start(&my_ptimer, 1000, my_ptimer_handler); // Start my_ptimer to trigger after 1000 units
 * @endcode
 */
void ptimer_start(ptimer_t* ptimer, timespan_t span, ptimer_handler_t handler);

/*!
 * @brief Restarts a process timer.
 * @details This function is used to restart a process timer in MyOS. It resets the timer's start time to the current
 *          moment, effectively restarting its countdown. The timer is then added back to the list of running timers,
 *          ensuring it is actively tracked and managed. This function is useful for reusing timers or resetting their
 *          timing without changing the duration or handler function.
 * @param[in] ptimer Pointer to the process timer to restart.
 *
 * Usage Example:
 * @code
 *     ptimer_t my_ptimer;
 *     // Initialize and start my_ptimer
 *     ptimer_restart(&my_ptimer); // Restart my_ptimer, resetting its countdown
 * @endcode
 */
void ptimer_restart(ptimer_t* ptimer);

/*!
 * @brief Resets a process timer.
 * @details This function is crucial for the management of process timers in MyOS. It resets the timer associated
 *          with the provided ptimer structure, adjusting its start time to reflect a new cycle. This function is
 *          particularly useful for periodic tasks where the timer needs to be reused regularly. After resetting the
 *          timer, the ptimer is added back to the ptimer_running_list to ensure it remains active and is processed
 *          correctly.
 * @param[in] ptimer Pointer to the process timer to reset.
 *
 * Usage Example:
 * @code
 *     ptimer_t my_ptimer;
 *     // Initialize and start my_ptimer
 *     ptimer_reset(&my_ptimer); // Reset my_ptimer for another cycle
 * @endcode
 */
void ptimer_reset(ptimer_t* ptimer);

/*!
 * @def ptimer_stop(ptimerptr)
 * @brief Stops a process timer.
 * @details This macro is used to stop a running process timer in MyOS. It calls ptimer_remove_from_list to remove
 *          the specified timer from the ptimer_running_list, effectively stopping it. This action also updates
 *          any relevant system states and statistics related to the process timer. It's essential for correctly
 *          managing the lifecycle of a process timer, ensuring that timers are deactivated when they are no longer needed.
 *
 * Usage Example:
 * @code
 *     ptimer_t my_ptimer;
 *     // Initialize and start my_ptimer
 *     ptimer_stop(&my_ptimer); // Stop the ptimer and remove it from the running list
 * @endcode
 */
#define ptimer_stop(ptimerptr) ptimer_remove_from_list(ptimerptr);

/*!
 * @brief Checks if a process timer has expired.
 * @details This macro is used to determine whether a given process timer (ptimer) has reached its expiration time in MyOS.
 *          It checks the underlying generic timer's expiration status. This functionality is integral to time-based decision-making
 *          in process management, enabling processes to respond to timed events. The macro simplifies the interface for checking timer expiration.
 * @param[in] ptimerptr Pointer to the process timer being checked.
 *
 * Usage Example:
 * @code
 *     ptimer_t my_ptimer;
 *     // Initialize and start my_ptimer
 *     if (ptimer_expired(&my_ptimer)) {
 *         // Execute actions for the expired timer
 *     }
 * @endcode
 */
#define ptimer_expired(ptimerptr)   timer_expired(&(ptimerptr)->timer)


#endif /* PTIMER_H_ */
