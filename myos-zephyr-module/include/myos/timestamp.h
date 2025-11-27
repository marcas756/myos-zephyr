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
 * @file       timestamp.h
 *
 * @copyright  Copyright (c) 2004, Swedish Institute of Computer Science.
 * @license    This file is released under the 3-Clause BSD License.\n
 *             https://opensource.org/license/bsd-3-clause
 *
 * @author     Adam Dunkels <adam@sics.se>
 * @authors    Marco Bacchi <marco@bacchi.at>,
 *
 *
 * @brief      Architecture-independent system timestamp management.
 * @details    This file provides the architecture-independent part for accessing and evaluating
 *             the system's timestamp counter. It forms the core of the timing mechanism in MyOS,
 *             independent of the underlying hardware or operating system. This independence allows
 *             for its use in both OS-based and bare-metal programming environments.
 *
 *             The file interfaces with architecture-specific implementations defined in
 *             "timestamp_arch.h" of the corresponding architectural part. Timestamps are crucial
 *             for all non-real-time timers used by the OS, offering functionalities like
 *             timestamp comparison, duration calculation, and blocking operations until a specific
 *             time point.
 *
 *             Usage examples include creating time delays, measuring time intervals, and
 *             implementing timeouts. The file's functions and macros offer flexibility and precision
 *             for various timing-related tasks in embedded systems.
 *
 * Usage Example:
 * @code
 *     // Initializing the timestamp module
 *     timestamp_module_init();
 *
 *     // Getting the current timestamp
 *     timestamp_t now = timestamp_now();
 *
 *     // Blocking for a specific duration
 *     timestamp_block_for(100); // Block for 100 time units
 *
 *     // Comparing two timestamps
 *     if (timestamp_less_than(start_time, end_time)) {
 *         // Perform an action
 *     }
 * @endcode
 */
#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include "timestamp_arch.h"

/**
 * @def TIMESTAMP_TICKS_PER_SEC
 * @brief Number of timestamp ticks per second.
 * @details This macro defines the number of ticks that occur in one second.
 *          This is an essential constant for time calculations in MyOS.
 *          It is an alias for TIMESTAMP_ARCH_TICKS_PER_SEC, which is platform-specific.
 *
 * @example
 * // Calculate time in seconds
 * timestamp_t ticks = 5000;
 * double seconds = ticks / (double)TIMESTAMP_TICKS_PER_SEC;
 */
#define TIMESTAMP_TICKS_PER_SEC TIMESTAMP_ARCH_TICKS_PER_SEC

/**
 * @def TIMESTAMP_DIFF(a, b)
 * @brief Calculates the difference between two timestamps.
 * @details This macro calculates the difference between two timestamps 'a' and 'b'.
 *          It is used for time comparisons and duration calculations in MyOS.
 *          It is an alias for TIMESTAMP_ARCH_DIFF, providing platform-specific implementation.
 * @param a First timestamp.
 * @param b Second timestamp.
 * @return The difference between 'a' and 'b'.
 *
 * @example
 * timestamp_t start, end;
 * // Initialize start and end with different values
 * timestamp_t duration = TIMESTAMP_DIFF(end, start);
 * // 'duration' now holds the time difference between 'end' and 'start'
 */
#define TIMESTAMP_DIFF TIMESTAMP_ARCH_DIFF

/**
 * @typedef timestamp_arch_t timestamp_t
 * @brief Represents a point in time.
 *
 * This type is used to represent specific points in time. It's typically
 * used in conjunction with timer functions and for time comparisons.
 * The actual implementation is platform-specific, aliased via `timestamp_arch_t`.
 *
 * @example
 * timestamp_t current_time;
 * current_time = timestamp_now(); // Get the current time
 * // Use current_time for time comparisons or timer setup
 */
typedef timestamp_arch_t timestamp_t;

/**
 * @typedef timestamp_t timespan_t
 * @brief Represents a duration or span of time.
 *
 * This type is used for specifying durations or intervals of time.
 * It is often used in timer operations, like specifying timeouts or delays.
 * Defined as an alias to `timestamp_t` to maintain consistency in time-related operations.
 *
 * @example
 * timespan_t interval;
 * interval = 1000; // Specify an interval of 1000 time units
 * // Use interval in timer setups or delays
 */
typedef timestamp_t timespan_t;

/**
 * @def timestamp_module_init
 * @brief Initializes the timestamp module.
 * @details This macro initializes the timestamp module of MyOS. It is typically
 *          called at the start of the system to prepare the timestamp functionality.
 *          It's an alias for timestamp_arch_module_init, which is platform-specific.
 *
 * @example
 * // At system initialization
 * timestamp_module_init();
 */
#define timestamp_module_init  timestamp_arch_module_init

/**
 * @def timestamp_now
 * @brief Retrieves the current system timestamp.
 * @details This macro returns the current timestamp. It's used for time tracking and
 *          operations in MyOS. It's an alias for timestamp_arch_now, which provides
 *          platform-specific implementation.
 * @return The current timestamp.
 *
 * @example
 * timestamp_t current_time;
 * current_time = timestamp_now(); // Get the current time
 */
#define timestamp_now timestamp_arch_now


/**
 * @def timestamp_less_than(a, b)
 * @brief Compares two timestamps to determine if the first is less than the second.
 *
 * Evaluates to true if timestamp 'a' is less than timestamp 'b'.
 * Utilizes TIMESTAMP_DIFF to calculate the difference.
 *
 * @param a First timestamp.
 * @param b Second timestamp.
 * @return Non-zero if 'a' is less than 'b', otherwise 0.
 *
 * @example
 * timestamp_t time1, time2;
 * // Initialize time1 and time2 with different values
 * if(timestamp_less_than(time1, time2)) {
 *   // Do something if time1 is less than time2
 * }
 */
#define timestamp_less_than(a,b)    (TIMESTAMP_DIFF((a),(b)) < 0)

/**
 * @def timestamp_lessequal_than(a, b)
 * @brief Compares two timestamps to determine if the first is less or equal to the second.
 *
 * Evaluates to true if timestamp 'a' is less than or equal to timestamp 'b'.
 * Utilizes TIMESTAMP_DIFF to calculate the difference.
 *
 * @param a First timestamp.
 * @param b Second timestamp.
 * @return Non-zero if 'a' is less than or equal to 'b', otherwise 0.
 *
 * @example
 * timestamp_t time1, time2;
 * // Initialize time1 and time2 with different values
 * if(timestamp_lessequal_than(time1, time2)) {
 *   // Do something if time1 is less than or equal to time2
 * }
 */
#define timestamp_lessequal_than(a,b)    (TIMESTAMP_DIFF((a),(b)) <= 0)

/**
 * @def timestamp_passed(timestamp)
 * @brief Checks if a given timestamp is in the past.
 * @details This macro checks if the current time has passed the given timestamp.
 * @param[in] timestamp The timestamp to check.
 * @return Integer. Returns 1 if the timestamp is in the past, otherwise returns 0.
 *
 * @example
 * timestamp_t my_timestamp;
 * // Initialize my_timestamp to some future time
 * if(timestamp_passed(my_timestamp)) {
 *     // Code to execute if the timestamp is passed
 * }
 */
#define timestamp_passed(timestamp) \
      timestamp_lessequal_than(timestamp,timestamp_now())


/**
 * @def timestamp_block_until(timestamp)
 * @brief Blocks execution until a specified timestamp is reached.
 * @param[in] timestamp The timestamp to wait for.
 * @details This macro blocks the execution of the program until the current time reaches the specified timestamp.
 *
 * @example
 * timestamp_t wait_until;
 * // Set wait_until to some future time
 * timestamp_block_until(wait_until);
 * // Execution resumes once the timestamp is reached
 */
#define timestamp_block_until(timestamp) \
    while(!timestamp_passed(timestamp)){};

/**
 * @def timestamp_block_for(timespan)
 * @brief Blocks execution for a specified duration.
 * @param[in] timespan Duration to block in timestamp units.
 * @details Blocks the current thread of execution for the specified duration.
 *
 * @example
 * timestamp_t duration = 500; // Duration to block
 * timestamp_block_for(duration);
 * // Code here will execute after the duration has passed
 */
#define timestamp_block_for(timespan)                     \
    do{                                                   \
        timestamp_t stop = timespan+timestamp_now();      \
        timestamp_block_until(stop);                      \
    }while(0)



#endif /* TIMESTAMP_H_ */

