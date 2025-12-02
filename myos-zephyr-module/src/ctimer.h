/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
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

/*!
 * @file ctimer.h
 *
 * @copyright  Copyright (c) 2006, Swedish Institute of Computer Science.
 * @license    This file is released under the 3-Clause BSD License.\n
 *             https://opensource.org/license/bsd-3-clause
 *
 * @author     Adam Dunkels <adam@sics.se>
 * @authors    Marco Bacchi <marco@bacchi.at>,
 *
 * @brief Callback Timer (ctimer) Management in MyOS.
 * @details ctimers in MyOS, derived from the abstract ptimer class, are specialized for executing callback functions after a specified time interval.
 *          They are integral to MyOS's cooperative and non-preemptive operating system, enabling timed callbacks within the context of specific tasks.
 *          Unlike ptimers and etimers, ctimers directly invoke user-defined functions, making them essential for scenarios requiring precise timing control
 *          in task execution. Due to their reliance on MyOS's task scheduling and event handling, ctimers are not suitable for bare-metal programming.
 *
 * Usage Example:
 * @code
 *     ctimer_t my_ctimer;
 *     ctimer_start(&my_ctimer, 1000, my_callback, my_data); // Schedule callback for 1000 time units
 * @endcode
 */



#ifndef CTIMER_H_
#define CTIMER_H_

#include "myos.h"

typedef struct ctimer_t ctimer_t;

/*!
 * @typedef ctimer_callback_t
 * @brief Callback function type for ctimers.
 * @details This type definition specifies the function signature for callback functions used with callback timers (ctimers) in MyOS.
 *          A ctimer_callback_t function is called when a ctimer expires. It provides a mechanism for executing custom
 *          actions at specific time intervals. The function receives a pointer to the ctimer that triggered it, allowing
 *          access to the ctimer's context and any associated data.
 *
 * Usage Example:
 * @code
 *     void my_ctimer_callback(ctimer_t *ctimer) {
 *         // Custom callback logic
 *     }
 * @endcode
 */
typedef void (*ctimer_callback_t)(ctimer_t *ctimer);

/*!
 * @struct ctimer_t
 * @brief Callback Timer structure in MyOS.
 * @details This structure defines a callback timer (ctimer) in MyOS, extending the ptimer's functionality.
 *          It is used for scheduling callback functions to be executed in a specific context after a defined
 *          time interval. The ctimer combines a process timer (ptimer) with additional fields specifying the
 *          callback function and its execution context, making it versatile for various timing and callback scenarios.
 *
 * @var ctimer_t::ptimer
 *      The underlying process timer.
 * @var ctimer_t::context
 *      The context in which the callback function will be invoked.
 * @var ctimer_t::callback
 *      The callback function to be called when the timer expires.
 * @var ctimer_t::data
 *      Additional data to be passed to the callback function.
 *
 * Usage Example:
 * @code
 *     ctimer_t my_ctimer;
 *     ctimer_set(&my_ctimer, 1000, my_callback, my_context, my_data); // Set ctimer for 1000 time units
 * @endcode
 */
struct ctimer_t {
   ptimer_t ptimer;     /*!< Instance of process timer control structure */
   process_t *context;  /*!< Context in which to invoke the callback function */
   ctimer_callback_t callback; /*!< Callback function to be called when process timer expires */
   void* data;
};

/*!
 * @brief Starts a callback timer with specified parameters.
 * @details This function initializes and starts a callback timer (ctimer) in MyOS. It sets the callback function,
 *          associated data, and the context in which the callback will execute. The ctimer, upon expiration, will
 *          invoke the specified callback function. This is ideal for tasks that need to execute a callback after a
 *          delay, within the context of the current process.
 * @param[in] ctimer Pointer to the ctimer to start.
 * @param[in] span Duration for the timer.
 * @param[in] callback Function to call when the timer expires.
 * @param[in] data Data to pass to the callback function.
 *
 * Usage Example:
 * @code
 *     ctimer_t my_ctimer;
 *     ctimer_start(&my_ctimer, 1000, my_callback_function, my_data); // Start ctimer for 1000 time units
 * @endcode
 */
void ctimer_start(ctimer_t *ctimer, timespan_t span, ctimer_callback_t callback, void *data);

/*!
 * @brief Initializes the callback timer module.
 * @details This macro serves as an initializer for the callback timer (ctimer) system in MyOS.
 *          It's essentially a wrapper for initializing the underlying process timer (ptimer) module,
 *          which is a prerequisite for the operation of ctimers. This initialization step is crucial and
 *          should be called during system startup to ensure that ctimers are ready for use.
 *
 * Usage Example:
 * @code
 *     // At system initialization
 *     ctimer_module_init(); // Initialize the callback timer module
 * @endcode
 */
#define ctimer_module_init()        ptimer_module_init()

/*!
 * @brief Restarts a callback timer.
 * @details This macro restarts an existing callback timer (ctimer) in MyOS. It resets the start time of the
 *          underlying process timer, effectively restarting the ctimer's countdown while maintaining its original
 *          callback and duration. This is useful for re-triggering a callback without needing to reconfigure the ctimer.
 *          The macro delegates the restart operation to ptimer_restart, ensuring consistent behavior.
 * @param[in] ctimerptr Pointer to the ctimer to be restarted.
 *
 * Usage Example:
 * @code
 *     ctimer_t my_ctimer;
 *     // Initialize and start my_ctimer
 *     ctimer_restart(&my_ctimer); // Restart the ctimer
 * @endcode
 */
#define ctimer_restart(ctimerptr)   ptimer_restart((ptimer_t*)ctimerptr)

/*!
 * @brief Resets a callback timer for another cycle.
 * @details This macro resets an existing callback timer (ctimer) in MyOS, adjusting its start time
 *          for another cycle based on its current duration. This functionality is particularly useful
 *          for periodic callback operations, allowing the ctimer to be reused with the same callback and duration.
 *          The macro delegates the reset operation to ptimer_reset, ensuring the timer is correctly set for another round.
 * @param[in] ctimerptr Pointer to the ctimer to be reset.
 *
 * Usage Example:
 * @code
 *     ctimer_t my_ctimer;
 *     // Initialize and start my_ctimer
 *     ctimer_reset(&my_ctimer); // Reset the ctimer for another cycle
 * @endcode
 */
#define ctimer_reset(ctimerptr)     ptimer_rreset((ptimer_t*)ctimerptr)

/*!
 * @brief Stops a callback timer.
 * @details This macro stops an active callback timer (ctimer) in MyOS. It halts the underlying process timer,
 *          thereby deactivating the ctimer. This action is crucial for discontinuing a ctimer's operation, especially
 *          when the timer's callback is no longer needed or the timer needs to be reconfigured.
 *          The macro delegates the stop operation to ptimer_stop, ensuring a proper halt of the timer.
 * @param[in] ctimerptr Pointer to the ctimer to be stopped.
 *
 * Usage Example:
 * @code
 *     ctimer_t my_ctimer;
 *     // Initialize and start my_ctimer
 *     ctimer_stop(&my_ctimer); // Stop the ctimer
 * @endcode
 */
#define ctimer_stop(ctimerptr)      ptimer_stop((ptimer_t*)ctimerptr)


/*!
 * @brief Checks if a callback timer has expired.
 * @details This macro is used to determine whether a specified callback timer (ctimer) has expired in MyOS.
 *          It checks the expiration status of the underlying process timer (ptimer). This check is essential
 *          for triggering callback functions at the right time and managing timer-based activities efficiently.
 *          The macro simplifies the interface for checking the expiration of ctimers by calling ptimer_expired.
 * @param[in] ctimerptr Pointer to the ctimer being checked.
 *
 * Usage Example:
 * @code
 *     ctimer_t my_ctimer;
 *     // Initialize and start my_ctimer
 *     if (ctimer_expired(&my_ctimer)) {
 *         // Perform actions for the expired ctimer
 *     }
 * @endcode
 */
#define ctimer_expired(ctimerptr)   ptimer_expired((ptimer_t*)ctimerptr)


#endif /* CTIMER_H_ */
