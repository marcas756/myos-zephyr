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


#include "myos.h"
#include "debug.h"

#if defined(CONFIG_MYOS_DEBUG_PTIMER)
#define DBG(...) DBG_FUNC(__VA_ARGS__)
#else
#define DBG(...) do{}while(0)
#endif


/*!
 * @var ptimer_running_list
 * @brief List of currently running process timers.
 * @details This static variable is a list that holds all the currently active process timers (ptimers) in MyOS.
 *          It's a central component in the ptimer system, used for tracking and managing timers that are currently in use.
 *          The list is updated whenever a ptimer is started or stopped, ensuring accurate and efficient timer management.
 *          The type of the list (double-linked or single-linked) depends on the MyOS configuration.
 *
 * Usage Example:
 * @code
 *     // Example of iterating over the ptimer_running_list
 *     ptlist_node_t *node;
 *     for (node = ptlist_begin(&ptimer_running_list); node != ptlist_end(&ptimer_running_list); node = ptlist_next(&ptimer_running_list, node)) {
 *         ptimer_t *ptimer = (ptimer_t *)node;
 *         // Perform operations with ptimer
 *     }
 * @endcode
 */
static ptlist_t ptimer_running_list;

/*!
 * @var ptimer_next_stop
 * @brief Next stop time for the earliest expiring process timer.
 * @details This global variable in MyOS represents the timestamp at which the next process timer (ptimer) is set to expire.
 *          It plays a key role in optimizing the timer processing mechanism by keeping track of the nearest future event.
 *          This allows the system to efficiently manage and wake up for the next ptimer event without unnecessary checks.
 *          The variable is updated whenever a new ptimer is added or an existing one is modified.
 *
 * Usage Example:
 * @code
 *     // Example of checking the next stop time
 *     if (timestamp_now() >= ptimer_next_stop) {
 *         // Process the next expiring ptimer
 *     }
 * @endcode
 */
timestamp_t ptimer_next_stop = 0;


/*!
 * @var ptimer_pending
 * @brief Flag indicating the presence of pending process timers.
 * @details This global variable in MyOS serves as a flag to indicate whether there are any process timers (ptimers)
 *          currently pending processing. It is set to true when a new ptimer is added to the ptimer_running_list
 *          and there were previously no timers pending. This flag is essential for optimizing timer checks within
 *          the system, ensuring that timer-related operations are only performed when necessary.
 *          It helps in efficient power and resource management, especially in embedded systems where such optimizations
 *          are crucial.
 *
 * Usage Example:
 * @code
 *     // Example of checking for pending ptimers
 *     if (ptimer_pending) {
 *         // Handle pending ptimers
 *         // This might involve checking the ptimer_running_list and processing expired ptimers
 *     }
 * @endcode
 */
bool ptimer_pending = false;

#if defined(CONFIG_MYOS_STATISTICS)
/*!
 * @var ptimer_list_size
 * @brief Count of active process timers for system statistics.
 * @details This variable, available when CONFIG_MYOS_STATISTICS is enabled, keeps track of the number of active process timers in MyOS.
 *          It's integral for collecting system statistics and kernel instrumentation, allowing for performance monitoring and analysis.
 *          The count increases with each new ptimer added and decreases when a ptimer is removed. This statistical tracking
 *          is vital in scenarios where understanding the load and performance of the system is crucial.
 *
 * Usage Example:
 * @code
 *     // Assuming CONFIG_MYOS_STATISTICS is enabled
 *     size_t active_timers = ptimer_list_size;
 *     // The variable active_timers can be used for logging or performance analysis.
 * @endcode
 */
size_t ptimer_list_size;
#endif


/*!
 * @var ptimer_poll_evt
 * @brief Constant process event for polling process timers.
 * @details This constant defines a process event specifically used for polling process timers in MyOS.
 *          It's set to trigger the ptimer_process function, which is responsible for handling and updating
 *          process timers. The event is identified by PROCESS_EVENT_POLL and doesn't carry additional data.
 *          This event is pivotal in the MyOS event-driven architecture, ensuring timely and efficient
 *          processing of ptimers.
 *
 * Usage Example:
 * @code
 *     // Example of using ptimer_poll_evt in a process context
 *     process_post(&ptimer_process, ptimer_poll_evt.id, ptimer_poll_evt.data);
 *     // This will trigger the ptimer processing mechanism in ptimer_process
 * @endcode
 */
const process_event_t ptimer_poll_evt = {
      .to = &ptimer_process,
      .id = PROCESS_EVENT_POLL,
      .data = NULL
};

/*!
 * @brief      Updates the next stop time for process timers.
 * @details    This internal function is crucial for efficient ptimer management in MyOS. It calculates
 *             and updates the next expiration time for a given ptimer. The function first determines the
 *             stop time of the provided ptimer. If there are already pending ptimers, it compares the
 *             new stop time with the current earliest stop time (ptimer_next_stop) and updates it if
 *             the new time is earlier. This ensures that the system can efficiently process the next
 *             expiring ptimer. If no ptimers are pending, it sets the ptimer_next_stop to this ptimer's
 *             stop time and marks ptimers as pending.
 *
 * @param[in]  ptimer Pointer to the process timer for which the next stop time is updated.
 *
 * Usage Example:
 * @code
 *     ptimer_t my_ptimer;
 *     // Initialize and start my_ptimer
 *     ptimer_next_stop_update(&my_ptimer); // Update the next stop time for my_ptimer
 * @endcode
 */
static void ptimer_next_stop_update(ptimer_t *ptimer)
{
   timestamp_t this_stop = timer_timestamp_stop(&ptimer->timer);

   if( ptimer_pending )
   {
      if( timestamp_less_than(this_stop,ptimer_next_stop) )
      {
         ptimer_next_stop = this_stop;
      }
   }
   else
   {
     ptimer_next_stop = this_stop;
     ptimer_pending = true;
   }
}

/*!
 * @brief      Adds a process timer to the active list in MyOS.
 * @details    This function is crucial for managing process timers in MyOS. It's responsible for adding a new ptimer
 *             to the ptimer_running_list, which is a list of all currently active ptimers. The function first checks
 *             if the ptimer is already running to avoid duplicates. If it's not running, the ptimer is added to the
 *             front of the ptimer_running_list, and its running status is set to true. This is important for subsequent
 *             timer operations that rely on this list to determine active timers.
 *
 *             Additionally, if MyOS is configured to collect statistics (CONFIG_MYOS_STATISTICS), the function updates the
 *             ptimer_list_size and checks if it exceeds the maximum size recorded (myos_stats.ptlist_size_max), updating
 *             the maximum size if necessary. This statistical data can be vital for performance tuning and system monitoring.
 *
 *             Finally, the function calls ptimer_next_stop_update to ensure the system accurately tracks the next
 *             expiration time across all timers, optimizing timer management and processing.
 *
 * @param[in]  ptimer Pointer to the process timer to add to the running list.
 *
 * Usage Example:
 * @code
 *     ptimer_t my_ptimer;
 *     // Initialize my_ptimer
 *     ptimer_add_to_list(&my_ptimer); // Add the ptimer to the running list and update system state
 * @endcode
 */
void ptimer_add_to_list(ptimer_t *ptimer)
{
   if ( !ptimer->running )
   {
      ptlist_push_front(&ptimer_running_list,ptimer);

#if defined(CONFIG_MYOS_STATISTICS)
      if(++ptimer_list_size > myos_stats.ptlist_size_max)
      {
          myos_stats.ptlist_size_max = ptimer_list_size;
      }
#endif
   }

   ptimer->running = true;
   ptimer_next_stop_update(ptimer);
}


/*!
 * @brief      Removes a process timer from the active list.
 * @details    This function handles the removal of a process timer from the ptimer_running_list in MyOS.
 *             It's a critical part of the ptimer management system, ensuring that timers no longer needed
 *             or expired are not kept in the active list, optimizing system resources.
 *
 *             The function first checks if the ptimer is running. If so, it sets the running status to false,
 *             indicating that the timer is no longer active. It then removes the ptimer from the
 *             ptimer_running_list using the ptlist_erase function. This removal is vital to prevent the system
 *             from processing an inactive timer.
 *
 *             If MyOS is configured with CONFIG_MYOS_STATISTICS, the function decrements the ptimer_list_size counter.
 *             This decrement is crucial for accurate system monitoring and statistics, particularly in systems
 *             where resource usage and performance are critical.
 *
 * @param[in]  ptimer Pointer to the process timer to remove from the running list.
 *
 * Usage Example:
 * @code
 *     ptimer_t my_ptimer;
 *     // Initialize and start my_ptimer
 *     ptimer_remove_from_list(&my_ptimer); // Remove the ptimer from the running list
 * @endcode
 */
void ptimer_remove_from_list(ptimer_t *ptimer)
{
   if( ptimer->running )
   {
      ptimer->running = false;
      ptlist_erase(&ptimer_running_list,ptimer);

#if defined(CONFIG_MYOS_STATISTICS)
      ptimer_list_size--;
#endif
   }
}


/*!
 * @brief Process for handling process timers in MyOS.
 * @details This process manages the lifecycle of process timers (ptimers). It initializes the ptimer
 *          running list and continuously waits for PROCESS_EVENT_POLL events. Upon receiving an event,
 *          it iterates over the list of running ptimers, handles expired timers by calling their respective
 *          handlers, and updates the next stop time for active ptimers. This process is a core component
 *          of the MyOS timer system, ensuring timely and efficient execution of timer-based tasks.
 *
 * Usage Example:
 * @code
 *     // The ptimer_process is typically defined as part of the MyOS system and doesn't require manual invocation.
 *     // It's automatically handled by the MyOS kernel.
 * @endcode
 */
PROCESS(ptimer_process, ptimer_process);
PROCESS_THREAD(ptimer_process)
{



   // Initialization of the process
   PROCESS_BEGIN();

#if defined(CONFIG_MYOS_PTIMER_LIST_TYPE_SLIST)
   DBG("ptimer_process: Using singly-linked list for ptimer management.\n");
#elif defined(CONFIG_MYOS_PTIMER_LIST_TYPE_DLIST)
   DBG("ptimer_process: Using doubly-linked list for ptimer management.\n");
#endif

   DBG("ptimer_process: started\n");
   
   // Initialize the list of running ptimers
   ptlist_init(&ptimer_running_list);

DBG("%p, %p\n",&ptimer_running_list, ptimer_running_list.next);
   // Process loop
   while(1)
   {
      // Wait for poll event
      PROCESS_WAIT_EVENT(PROCESS_EVENT_POLL);
      DBG("ptimer_process: PROCESS_EVENT_POLL received\n");

      ptimer_pending = false;

      // Iterate over running ptimers
      ptimer_t *curr = (ptimer_t*)ptlist_begin(&ptimer_running_list);
      while(curr != (ptimer_t*)ptlist_end(&ptimer_running_list))
      {
         ptimer_t *next = (ptimer_t*)ptlist_next(&ptimer_running_list, curr);
         
         // Process expired ptimers
         if(ptimer_expired(curr))
         {

            // Remove ptimer from list
            ptlist_erase(&ptimer_running_list, curr);
            curr->running = false;
#if defined(CONFIG_MYOS_STATISTICS)
            ptimer_list_size--;
#endif

            // Call the handler if it exists
            if(curr->handler)
            {
               curr->handler((void*)(curr));
            }
         }
         else
         {
            // Update the next stop time
            ptimer_next_stop_update(curr);
         }

         // Move to the next ptimer
         curr = next;
      }
   }

   // End of the process
   PROCESS_END();
}


void ptimer_start(ptimer_t* ptimer, timespan_t span, ptimer_handler_t handler)
{
   ptimer->handler = handler;
   timer_start(&ptimer->timer,span);
   ptimer_add_to_list(ptimer);
}


void ptimer_restart(ptimer_t* ptimer)
{
   timer_restart(&ptimer->timer);
   ptimer_add_to_list(ptimer);
}


void ptimer_reset(ptimer_t* ptimer)
{
   timer_reset(&ptimer->timer);
   ptimer_add_to_list(ptimer);
}



extern bool process_deliver_event(process_event_t *evt);



void ptimer_processing(void)
{
    // Check for CONFIG_MYOS_STATISTICS to enable statistics
#if defined(CONFIG_MYOS_STATISTICS)
    rtimer_timestamp_t start;
    rtimer_timespan_t slicetime;
#endif

    // Process if there are pending timers and the next stop time has passed
    if (ptimer_pending && timestamp_passed(ptimer_next_stop)) {
        ptimer_pending = false;

        // Record start time if statistics are enabled
#if defined(CONFIG_MYOS_STATISTICS)
        start = rtimer_now();
#endif

        // Deliver the ptimer poll event to process expired timers
        process_deliver_event((process_event_t*)&ptimer_poll_evt);

        // Calculate and record the processing time for performance stats
#if defined(CONFIG_MYOS_STATISTICS)
        slicetime = (rtimer_timespan_t)RTIMER_TIMESTAMP_DIFF(rtimer_now(), start);
        if (ptimer_process.maxslicetime < slicetime) {
            ptimer_process.maxslicetime = slicetime;
        }
#endif
    }
}



