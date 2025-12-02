/*
 * Copyright (c) 2004, Swedish Institute of Computer Science.
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
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*!
 * @file etimer.h
 *
 * @copyright  Copyright (c) 2004, Swedish Institute of Computer Science.
 * @license    This file is released under the 3-Clause BSD License.\n
 *             https://opensource.org/license/bsd-3-clause
 *
 * @author     Adam Dunkels <adam@sics.se>
 * @authors    Marco Bacchi <marco@bacchi.at>,
 *
 * @brief Event Timer (etimer) Management in MyOS.
 * @details etimers in MyOS, derived from the abstract ptimer class, provide functionality for scheduling and handling timed events.
 *          They are closely integrated with the cooperative and non-preemptive nature of MyOS, relying on the operating system's event
 *          handling and process scheduling. etimers are used to trigger events after specified time intervals, facilitating time-driven
 *          task execution. Unlike ptimers, etimers focus on event delivery, making them more suited for scenarios requiring timed interaction
 *          within the MyOS environment. Note that etimers depend on the OS and are not suitable for bare-metal programming.
 *
 * Usage Example:
 * @code
 *     etimer_t my_etimer;
 *     etimer_start(&my_etimer, 1000, &my_process, MY_EVENT, NULL); // Schedule an event for 1000 time units
 * @endcode
 */



#ifndef ETIMER_H_
#define ETIMER_H_

#include "myos.h"

/*!
 * @struct etimer_t
 * @brief Event Timer (etimer) structure in MyOS.
 * @details This structure defines an event timer (etimer) in MyOS, extending the functionality of ptimer_t.
 *          It includes a process timer (ptimer) for timing mechanisms and a process_event_t for event handling.
 *          Etimers are used to schedule events to be posted to processes after a specified time,
 *          making them integral for event-driven tasks in MyOS.
 *
 * @var etimer_t::ptimer
 *      The underlying process timer.
 * @var etimer_t::evt
 *      The event to be posted when the timer expires.
 *
 * Usage Example:
 * @code
 *     etimer_t my_etimer;
 *     etimer_set(&my_etimer, 1000); // Set etimer for 1000 time units
 *     if (etimer_expired(&my_etimer)) {
 *         // Handle the expiration event
 *     }
 * @endcode
 */
typedef struct {
    ptimer_t ptimer;
    process_event_t evt;
} etimer_t;



/*!
 * @brief Initializes the event timer module.
 * @details This macro is a wrapper for initializing the process timer (ptimer) module, which is a prerequisite for etimers in MyOS.
 *          Invoking this macro ensures that the underlying ptimer system is set up, thereby enabling the correct functionality
 *          of etimers. This initialization should be called at the start of the system to ensure that event timers are ready for use.
 *
 * Usage Example:
 * @code
 *     // At system initialization
 *     etimer_module_init(); // Initialize the event timer module
 * @endcode
 */
#define etimer_module_init() ptimer_module_init()


/*!
 * @brief Starts an event timer with a specified duration and event details.
 * @details This function initializes an event timer (etimer) in MyOS. It sets up the event details such as event ID, data, source,
 *          and destination process. The etimer is then started with the specified timespan using ptimer_start. This is crucial
 *          for scheduling events that are to be delivered to processes after the timer expires.
 * @param[in] etimer Pointer to the event timer.
 * @param[in] span Duration for the timer.
 * @param[in] to Destination process for the event.
 * @param[in] evtid Event ID.
 * @param[in] data Data to be passed along with the event.
 *
 * Usage Example:
 * @code
 *     etimer_t my_etimer;
 *     etimer_start(&my_etimer, 1000, &my_process, MY_EVENT, NULL); // Start etimer for 1000 units
 * @endcode
 */
void etimer_start(etimer_t *etimer, timespan_t span, process_t *to, process_event_id_t evtid, void *data);

/*!
 * @brief Restarts an event timer.
 * @details This macro is used to restart an existing event timer (etimer) in MyOS. It resets the underlying
 *          process timer's start time to the current time, maintaining its original duration and event details.
 *          This is useful for reusing etimers without needing to reconfigure their event properties. The macro
 *          effectively delegates the restart operation to ptimer_restart.
 * @param[in] etimerptr Pointer to the etimer to be restarted.
 *
 * Usage Example:
 * @code
 *     etimer_t my_etimer;
 *     // Initialize and start my_etimer
 *     etimer_restart(&my_etimer); // Restart my_etimer, resetting its countdown
 * @endcode
 */
#define etimer_restart(etimerptr)                       ptimer_restart((ptimer_t*)etimerptr)

/*!
 * @brief Resets an event timer for another cycle.
 * @details This macro is used to reset an existing event timer (etimer) in MyOS. It adjusts the start time of the
 *          underlying process timer for another cycle based on its existing duration. This is useful for periodic
 *          tasks where the etimer needs to be reused regularly with the same duration and event configuration.
 *          The macro essentially calls ptimer_reset for the associated ptimer.
 * @param[in] etimerptr Pointer to the etimer to be reset.
 *
 * Usage Example:
 * @code
 *     etimer_t my_etimer;
 *     // Initialize and start my_etimer
 *     etimer_reset(&my_etimer); // Reset my_etimer for another cycle
 * @endcode
 */
#define etimer_reset(etimerptr)                         ptimer_reset((ptimer_t*)etimerptr)

/*!
 * @brief Stops an event timer.
 * @details This macro is utilized to stop an active event timer (etimer) in MyOS. It effectively halts the
 *          associated process timer, thereby deactivating the etimer. This action is crucial for discontinuing
 *          an etimer's operation when its functionality is no longer required or when the timer needs to be
 *          reconfigured. The macro delegates the stop operation to ptimer_stop, ensuring a proper shutdown of the timer.
 * @param[in] etimerptr Pointer to the etimer to be stopped.
 *
 * Usage Example:
 * @code
 *     etimer_t my_etimer;
 *     // Initialize and start my_etimer
 *     etimer_stop(&my_etimer); // Stop the etimer
 * @endcode
 */
#define etimer_stop(etimerptr)                          ptimer_stop((ptimer_t*)etimerptr)

/*!
 * @brief Checks if an event timer has expired.
 * @details This macro is used to determine whether a specified event timer (etimer) in MyOS has expired.
 *          It checks the status of the underlying process timer to see if its designated time span has elapsed.
 *          This is essential for triggering events or actions upon timer completion in an event-driven system.
 *          The macro essentially invokes ptimer_expired to assess the timer's status.
 * @param[in] etimerptr Pointer to the etimer being checked.
 *
 * Usage Example:
 * @code
 *     etimer_t my_etimer;
 *     // Initialize and start my_etimer
 *     if (etimer_expired(&my_etimer)) {
 *         // Actions to perform when etimer expires
 *     }
 * @endcode
 */
#define etimer_expired(etimerptr)                       ptimer_expired((ptimer_t*)etimerptr)


/*!
 * @brief Suspends the calling process for a specified duration.
 * @details This macro is used to pause the execution of the current process in MyOS for a given timespan.
 *          It starts an event timer (etimer) for the specified span, and then waits for a PROCESS_EVENT_CONTINUE event
 *          to resume execution. This macro is particularly useful for creating delays or timeouts in process execution,
 *          without blocking the entire system. It encapsulates the pattern of starting an etimer and waiting for its completion.
 * @param[in] etimerptr Pointer to an etimer used for the sleep operation.
 * @param[in] span Duration for the sleep period.
 *
 * Usage Example:
 * @code
 *     etimer_t my_etimer;
 *     PROCESS_SLEEP(&my_etimer, 1000); // Suspend process for 1000 time units
 * @endcode
 */
#define PROCESS_SLEEP(etimerptr,span) \
   do{ \
      etimer_start(etimerptr, span, PROCESS_THIS(), PROCESS_EVENT_CONTINUE, NULL); \
      PROCESS_WAIT_EVENT(PROCESS_EVENT_CONTINUE); \
   }while(0)

#endif /* ETIMER_H_ */
