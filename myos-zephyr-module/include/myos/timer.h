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
 * @file       timer.h
 * @brief      Comprehensive Timer Management in MyOS.
 * @details    This file is integral to MyOS for managing various types of timers,
 *             acting as a bridge between low-level time tracking (timestamps) and
 *             higher-level scheduling (process timers). It provides a unified interface
 *             for timer operations, suitable for both embedded systems with or without an OS.
 *
 *             Timers in this context are built on top of the timestamp functionality,
 *             offering more complex timing mechanisms such as expiration tracking and periodic updates.
 *             While basic timers provide generic timing capabilities, in an OS context, specialized
 *             timer types like process timers (ptimers), callback timers (ctimers), and event timers (etimers)
 *             are more commonly used. These specialized timers cater to different needs, such as
 *             asynchronous event handling and callback scheduling, crucial in multitasking environments.
 *
 * Usage Example:
 * @code
 *     timer_t my_timer;
 *     timer_start(&my_timer, 1000); // Start the timer for 1000 units
 *     // In an OS context, using a process timer (ptimer)
 *     ptimer_t my_ptimer;
 *     ptimer_start(&my_ptimer, 500); // Start the process timer for 500 units
 *     if (timer_expired(&my_timer)) {
 *         // Code to execute when the timer expires
 *     }
 * @endcode
 */

#ifndef  TIMER_H_
#define  TIMER_H_

#include "timestamp.h"
#include <stdbool.h>


/**
 * @struct timer_t
 * @brief Structure representing a timer.
 * @details This structure is used to define a timer in MyOS. It includes a start point
 *          (`start`) and a duration (`span`). This architecture-independent structure
 *          allows for flexible timer management, suitable for both OS-based and bare-metal
 *          environments.
 *          For more details, refer to mba_myos_timer_system.pdf and the MyOS Timer System documentation.
 *
 * @var timer_t::start
 *      The start point of the timer.
 * @var timer_t::span
 *      The duration of the timer.
 *
 * Usage Example:
 * @code
 *     timer_t my_timer;
 *     my_timer.start = timestamp_now(); // Set start to current time
 *     my_timer.span = 500; // Set duration of the timer
 *     // Use my_timer in application logic
 * @endcode
 */
typedef struct {
    timestamp_t start;
    timespan_t span;
}timer_t ;


/*!
 * @brief      Starts a timer with a specified duration.
 * @details    Initializes or reinitializes a timer to expire after a given time span.
 *             If the timer is already running, it will be reset with the new span.
 *             Previous timer settings will be overwritten.
 * @param[in]  timer Pointer to the timer instance to start.
 * @param[in]  span Duration after which the timer should expire.
 *
 * Usage Example:
 * @code
 *     timer_t my_timer;
 *     timer_start(&my_timer, 1000); // Start timer for 1000 time units
 * @endcode
 */
void timer_start(timer_t *timer, timespan_t span);


/*!
 * @brief      Resets a timer for another span.
 * @details    Adjusts the start time of the timer by adding the span to the
 *             current start time, effectively resetting it for another cycle.
 * @param[in]  timer Pointer to the timer instance to reset.
 *
 * Usage Example:
 * @code
 *     timer_reset(&my_timer); // Reset timer for another span
 * @endcode
 */
void timer_reset(timer_t *timer);

/*!
 * @brief      Restarts a timer from the current time.
 * @details    Resets the start time of the timer to the current timestamp,
 *             keeping the existing span unchanged.
 * @param[in]  timer Pointer to the timer instance to restart.
 *
 * Usage Example:
 * @code
 *     timer_restart(&my_timer); // Restart timer from now
 * @endcode
 */
void timer_restart(timer_t *timer);

/**
 * @def timer_timestamp_stop(timerptr)
 * @brief Calculates the stop timestamp of a timer.
 * @details This macro computes the end point of a timer's active period. It adds
 *          the span of the timer to its start time, resulting in the time at which
 *          the timer will expire.
 * @param timerptr Pointer to the timer instance.
 * @return The timestamp at which the timer will stop.
 *
 * Usage Example:
 * @code
 *     timer_t my_timer;
 *     // Initialize and start my_timer
 *     timestamp_t stop_time = timer_timestamp_stop(&my_timer);
 *     // Now, stop_time holds the time when my_timer will expire
 * @endcode
 */
#define timer_timestamp_stop(timerptr) \
      ((timerptr)->start + (timerptr)->span)

/*!
 * @def timer_expired(timerptr)
 * @brief Checks if a timer has expired.
 * @details Determines whether the specified timer has reached its expiration time.
 *          The expiration is based on the span provided during the timer's start.
 *          This check is essential for timing operations in tasks and event management.
 * @param[in] timerptr Pointer to the timer instance to check.
 * @return Boolean value: true if the timer has expired, false otherwise.
 *
 * Usage Example:
 * @code
 *     timer_t my_timer;
 *     // Initialize and start my_timer
 *     if (timer_expired(&my_timer)) {
 *         // Code to execute when the timer has expired
 *     }
 * @endcode
 */
#define timer_expired(timerptr) \
      timestamp_passed(timer_timestamp_stop(timerptr))


/*!
 * @def timer_module_init
 * @brief Initializes the timer module.
 * @details This macro is used to initialize the timer module in MyOS. It's an alias
 *          for timestamp_module_init, ensuring that the timestamp system is ready
 *          for timer operations. This initialization step is crucial in both OS-based
 *          and bare-metal programming environments for accurate timer functionalities.
 *          For more information, refer to mba_myos_timer_system.pdf and MyOS Timer System documentation.
 *
 * Usage Example:
 * @code
 *     // At system start-up or before using timers
 *     timer_module_init();
 * @endcode
 */
#define timer_module_init timestamp_module_init


#endif /*  TIMER_H_ */
