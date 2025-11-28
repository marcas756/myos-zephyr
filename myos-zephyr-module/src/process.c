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



#include "myos.h"
#include <stdlib.h>
#include "debug.h"




#if defined(CONFIG_MYOS_DEBUG_PROCESS)
#define DBG(...) DBG_FUNC(__VA_ARGS__)
#define DBG_PROCESS(...) do{DBG("[%d] process: ",(int)timestamp_now());DBG(__VA_ARGS__);}while(0)
#else
#define DBG(...) do{}while(0)
#define DBG_PROCESS(...) do{}while(0)
#endif

/**
 * @var process_current
 * @brief Currently running process.
 *
 * @details
 * Pointer to the currently running process in the system. This is used to keep track of the
 * process context that is currently executing. It is set to NULL when no process is active.
 */
process_t *process_current = NULL;

/**
 * @var process_running_list
 * @brief List of processes that are currently active and scheduled for execution.
 *
 * @details
 * This list maintains all the processes that are currently running in the system. It is used
 * by the scheduler to manage and iterate through active processes for execution.
 * The implementation of the list is determined by the configuration (singly or doubly linked list).
 */
static plist_t process_running_list;

/**
 * @typedef process_event_queue
 * @brief Ringbuffer type for storing process events.
 *
 * @details
 * Defines a ringbuffer type for queuing process events. This ringbuffer holds events that are
 * posted to processes and are pending to be processed. The size of the ringbuffer is defined by
 * CONFIG_MYOS_PROC_EVENT_QUEUE_SIZE.
 *
 * @param process_event_t Type of items in the ringbuffer (process events).
 * @param MYOS_PROC_EVENT_QUEUE_SIZE Size of the ringbuffer, number of events it can hold.
 */
RINGBUFFER_TYPEDEF(process_event_queue,process_event_t,CONFIG_MYOS_PROC_EVENT_QUEUE_SIZE);

/**
 * @var process_event_queue
 * @brief Ringbuffer instance for managing process events.
 *
 * @details
 * An instance of the ringbuffer that is used to queue events for processes in the system.
 * Events posted to processes are stored in this ringbuffer until they are processed.
 * This ensures that events are handled in a FIFO manner and provides a mechanism for
 * event-driven process scheduling.
 */
static RINGBUFFER_T(process_event_queue) process_event_queue;

/**
 * @var process_global_pollreq
 * @brief Global flag indicating whether any process has requested a poll.
 *
 * @details
 * This flag is set to true when any process in the system requests to be polled. The poll request
 * indicates that a process needs immediate attention. When set, the system's scheduler will
 * prioritize handling poll requests before processing other events in the event queue.
 */
static bool process_global_pollreq = false;




void process_init(void)
{

#if defined(CONFIG_MYOS_PROC_LIST_TYPE_SLIST)
   DBG_PROCESS("Using singly-linked list for process management.\n");
#elif defined(CONFIG_MYOS_PROC_LIST_TYPE_DLIST)
   DBG_PROCESS("Using doubly-linked list for process management.\n");
#endif

   DBG_PROCESS("Using event queue of size %d \n",RINGBUFFER_SIZE(process_event_queue));

   /* Initialize the list for running processes. */
   plist_init(&process_running_list);

   /* Initialize the ring buffer for the process event queue. */
   RINGBUFFER_INIT(process_event_queue);

   /* Set the current process to NULL, indicating no process is running. */
   PROCESS_THIS() = NULL;
}


bool process_post(process_t *to, process_event_id_t evtid, void* data)
{
   process_event_t *evt;

   // Check if the event queue is full.
   if(RINGBUFFER_FULL(process_event_queue))
   {
#if defined(CONFIG_MYOS_STATISTICS)
      myos_stats.errflags.eventqueue = 1;
#endif
      return false;
   }

   // Get a pointer to the next free space in the event queue.
   evt = RINGBUFFER_TAIL_PTR(process_event_queue);

   // Fill in the event structure.
   evt->from = PROCESS_THIS();
   evt->to = to;
   evt->id = evtid;
   evt->data = data;

   DBG_PROCESS("post from %p to %p evtid=%d ...\n", (void*)evt->from, (void*)evt->to, evt->id);

   // Push the event onto the event queue.
   RINGBUFFER_PUSH(process_event_queue);

#if defined(CONFIG_MYOS_STATISTICS)
   // Update the maximum queue count for statistics.
   if(RINGBUFFER_COUNT(process_event_queue) > myos_stats.maxqueuecount)
   {
      myos_stats.maxqueuecount = RINGBUFFER_COUNT(process_event_queue);
   }
#endif

   return true;
}

/**
 * @brief Delivers an event to a process.
 *
 * @param evt Pointer to the process event to be delivered.
 * @return True if the event was successfully delivered, False otherwise.
 *
 * @details
 * This function is responsible for delivering an event to a specific process.
 * It first checks if the target process is running or if the event is a start
 * event. If so, it switches the context to the target process and then invokes
 * the process's thread function, passing the event to it.
 *
 * During the event handling, this function measures the execution time of the
 * process (slicetime) and updates the process's `maxslicetime` if the current
 * execution time is the longest one recorded. This feature is only active if
 * `CONFIG_MYOS_STATISTICS` is defined.
 *
 * If the process's state after handling the event is `PT_STATE_TERMINATED`, the
 * process is removed from the running processes list. The function then restores
 * the original process context.
 *
 * @note
 * This function is typically called internally by the process management system
 * and should not be called directly in application code.
 *
 * Debugging messages are printed with information about the event delivery if
 * the DEBUG_PROCESS macro is defined.
 *
 * @code
 * // Example usage within the process management system
 * process_event_t my_event;
 * // Initialize my_event...
 * process_deliver_event(&my_event);
 * @endcode
 */
bool process_deliver_event(process_event_t *evt)
{
   DBG_PROCESS("deliver_event from %p to %p evtid=%d ...\n", (void*)evt->from, (void*)evt->to, evt->id);

   if(PROCESS_IS_RUNNING(evt->to) || evt->id == PROCESS_EVENT_START)
   {
      PROCESS_CONTEXT_BEGIN(evt->to);

#if defined(CONFIG_MYOS_STATISTICS)
      rtimer_timespan_t slicetime = rtimer_now();
#endif

      int pstate = PROCESS_THIS()->thread(PROCESS_THIS(), evt);

#if defined(CONFIG_MYOS_STATISTICS)
      slicetime = rtimer_now() - slicetime;
      if(slicetime > PROCESS_THIS()->maxslicetime)
      {
         PROCESS_THIS()->maxslicetime = slicetime;
      }
#endif

      if(pstate == PT_STATE_TERMINATED)
      {
         plist_erase(&process_running_list, PROCESS_THIS());
         // Consider broadcasting exit to all processes if needed.
      }

      PROCESS_CONTEXT_END();
      return true;
   }

   return false;
}


bool process_post_sync(process_t *to, process_event_id_t evtid, void* data)
{
   process_event_t evt = {
      .from = PROCESS_THIS(),
      .to = to,
      .id = evtid,
      .data = data
   };

   DBG_PROCESS("post_sync from %p to %p evtid=%d ...\n", (void*)evt.from, (void*)evt.to, evt.id);

   // Deliver the event immediately.
   return process_deliver_event(&evt);
}

bool process_start(process_t *process, void* data)
{
   DBG_PROCESS("start %p ...\n", (void*)process);

   // Check if the process is already running.
   if(PROCESS_IS_RUNNING(process))
   {
      DBG_PROCESS("start %p failure\n", (void*)process);
      return false;
   }

   // Set the process data and initialize its protothread.
   process->data = data;
   PT_INIT(&process->pt);

   // Add the process to the running processes list.
   plist_push_front(&process_running_list, process);

   // Post the PROCESS_EVENT_START event to the process.
   process_post_sync(process, PROCESS_EVENT_START, data);

   DBG_PROCESS("start %p success\n", (void*)process);

   return true;
}


bool process_exit(process_t *process)
{
   DBG_PROCESS("exit %p ...\n", (void*)process);

   // Check if the process is currently running.
   if(!PROCESS_IS_RUNNING(process))
   {
      DBG_PROCESS("exit %p failure\n", (void*)process);
      return false;
   }

   // Post the PROCESS_EVENT_EXIT event to the process to trigger its termination.
   process_post_sync(process, PROCESS_EVENT_EXIT, NULL);

   DBG_PROCESS("exit %p success\n", (void*)process);

   return true;
}



void process_poll(process_t *process)
{
   DBG_PROCESS("polling %p \n", (void*)process);
   process->pollreq = true;
   process_global_pollreq = true;
}


inline int process_run(void)
{
   // Process polling requests.
#if defined(CONFIG_MYOS_STATISTICS)
   rtimer_timespan_t proctime = rtimer_now();
#endif

   while(process_global_pollreq)
   {
      process_t *process;

      process_global_pollreq = false;

      plist_foreach(&process_running_list, process)
      {
         if(process->pollreq)
         {
            process->pollreq = false;
            process_post_sync(process, PROCESS_EVENT_POLL, NULL);
         }
      }
   }

   // Process protothread timers if enabled.

   ptimer_processing();


   // Process events from the event queue.
   if(RINGBUFFER_COUNT(process_event_queue))
   {
      process_deliver_event(RINGBUFFER_HEAD_PTR(process_event_queue));
      RINGBUFFER_POP(process_event_queue);
   }

   // Update processing time statistics.
#if defined(CONFIG_MYOS_STATISTICS)
   proctime = rtimer_now() - proctime;
   if(proctime > myos_stats.maxproctime)
   {
      myos_stats.maxproctime = proctime;
   }
#endif

   // Return the count of remaining events and poll requests.
   return RINGBUFFER_COUNT(process_event_queue) + process_global_pollreq;
}


