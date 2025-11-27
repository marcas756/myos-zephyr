/*
 * Copyright (c) 2004-2005, Swedish Institute of Computer Science.
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


/**
 * @file
   @copyright  Copyright (c) 2004-2005, Swedish Institute of Computer Science.
 * @license    This file is released under the 3-Clause BSD License.\n
 *             https://opensource.org/license/bsd-3-clause
 *
 * @author     Adam Dunkels <adam@sics.se>
 * @authors    Marco Bacchi <marco@bacchi.at>,
 *
 *
 * @brief Protothread definitions for MyOs.
 *
 * @details
 * Protothreads are a lightweight, stackless threading mechanism, offering
 * a way to implement non-blocking code in a blocking manner, which is
 * particularly useful in resource-constrained systems like embedded devices.
 * This implementation is based on Contiki 2.7 protothread but has been adapted
 * and extended for use in myos.
 *
 * Protothreads provide a way to perform tasks in threads without using multiple
 * stacks. This is achieved through the use of local continuations that save
 * and restore the execution state of a function at specific points.
 * Local continuations in this implementation are based on Duff's device, a
 * technique for creating a coroutine in C.
 *
 * Adaptations from the original Contiki implementation:
 * - Removal of the local auto variable PT_YIELD_FLAG required for suspending the task.
 * - Addition of the LC_SET_YIELD macro to properly suspend and resume protothreads.
 * - Modifications to enhance portability and efficiency in different environments.
 *
 * Protothreads are particularly effective in event-driven systems where tasks do
 * not need to be executed simultaneously but rather wait for specific events or
 * conditions to proceed. They offer a simple and memory-efficient mechanism
 * for implementing cooperative multitasking.
 *
 * @note Protothreads do not save the call stack, thus local variables are not
 * preserved across blocking calls. Care must be taken to preserve the state
 * across yields or waits if needed.
 */


#ifndef PT_H_
#define PT_H_

#include <stdint.h>


/**
 * @brief Typedef for local continuation state.
 *
 * @details This type is used to store the state of a protothread. It should
 *          be capable of storing at least as many different values as there
 *          are lines in the function that uses the protothread.
 */
typedef uint16_t lc_t;


/**
 * @brief Definitions and macros for local continuations used in protothreads.
 *
 * @details Local continuations provide a lightweight mechanism to preserve the
 *          state of a function such that it can be resumed later. This is
 *          particularly useful in implementing protothreads for cooperative
 *          multitasking in a memory-constrained environment.
 */
/*!\{*/

#define LC_INIT(s) s = 0;
#define LC_DEFAULT ((lc_t)(~((lc_t)(0))))
#define LC_SET_DEFAULT(s) s = LC_DEFAULT
#define LC_RESUME(s) switch(s) { case 0:
#define LC_SET(s) s = __LINE__; case __LINE__:
#define LC_SET_YIELD(s,retval) s = __LINE__; return retval; case __LINE__:
#define LC_END(s) default:;}
/*!\}*/



/**
 * @brief Initialize a local continuation.
 *
 * @param s The local continuation state variable.
 *
 * @details This macro initializes the local continuation state, effectively
 *          setting the protothread to start from its beginning upon the next
 *          invocation.
 */
#define LC_INIT(s) s = 0;

/**
 * @brief Default value for an uninitialized or reset local continuation.
 *
 * @details This value is used to indicate that the local continuation is either
 *          uninitialized or has been reset to its initial state.
 */
#define LC_DEFAULT ((lc_t)(~((lc_t)(0))))

/**
 * @brief Set a local continuation to its default state.
 *
 * @param s The local continuation state variable.
 *
 * @details This macro resets the local continuation, setting it to a state that
 *          indicates the protothread is either uninitialized or completed.
 */
#define LC_SET_DEFAULT(s) s = LC_DEFAULT

/**
 * @brief Resume a local continuation from its last state.
 *
 * @param s The local continuation state variable.
 *
 * @details This macro is used at the beginning of a protothread function to
 *          resume execution from the point where the protothread was last
 *          suspended.
 */
#define LC_RESUME(s) switch(s) { case 0:

/**
 * @brief Set the local continuation to the current line number.
 *
 * @param s The local continuation state variable.
 *
 * @details This macro is used within a protothread to mark the point to which
 *          control should return the next time the protothread is resumed.
 */
#define LC_SET(s) s = __LINE__; case __LINE__:

/**
 * @brief Set the local continuation to the current line number and yield with a return value.
 *
 * @param s The local continuation state variable.
 * @param retval The return value when yielding.
 *
 * @details This macro is similar to LC_SET but also allows the protothread to
 *          yield execution and return a value. This is useful for implementing
 *          blocking operations within a protothread.
 */
#define LC_SET_YIELD(s,retval) s = __LINE__; return retval; case __LINE__:

/**
 * @brief End the local continuation block.
 *
 * @param s The local continuation state variable.
 *
 * @details This macro ends the block of code started by LC_RESUME. It should be
 *          placed at the end of the protothread function.
 */
#define LC_END(s) default:;}



/**
 * @brief Struct for managing protothreads.
 *
 * @details
 * The `pt_t` structure is used to represent a protothread. Protothreads
 * provide a lightweight, stackless means of managing concurrency,
 * particularly useful in memory-constrained systems such as embedded devices.
 *
 * Each protothread has its own execution context, represented by an instance
 * of this structure. The context does not include stack data, making
 * protothreads an extremely memory-efficient form of multitasking.
 *
 * @see lc_t for the type of the local continuation state variable.
 *
 * Example Usage:
 * \code{.c}
 * pt_t my_protothread;
 * PT_INIT(&my_protothread);
 *
 * // In a function implementing a protothread:
 * PT_BEGIN(&my_protothread);
 * // Protothread's code here...
 * PT_END(&my_protothread);
 * \endcode
 */
typedef struct {
    /**
     * @brief The local continuation state.
     *
     * This variable is used to store the position within the protothread,
     * allowing the protothread to yield and later resume execution from
     * the point where it left off.
     */
    lc_t lc;
} pt_t;

typedef uint8_t ptstate_t;


/**
 * @brief Protothread state definitions and macros.
 *
 * @details
 * This set of macros define various states and conditions of protothreads
 * used to control the flow of execution in a protothread.
 */

/**
 * @brief Indicates that the protothread is waiting for an event or condition.
 *
 * @details
 * This state is used to signify that a protothread is in a waiting state,
 * typically used in conjunction with PT_WAIT_* macros to yield control
 * until a specific condition is met or an event occurs.
 */
#define PT_STATE_WAITING        1

/**
 * @brief Represents a terminated state of a protothread.
 *
 * @details
 * This state is used to indicate that a protothread has completed its execution.
 * It is the end state for a protothread and signifies that no further execution
 * will take place in this protothread.
 */
#define PT_STATE_TERMINATED     ((ptstate_t)(~((ptstate_t)(0))))

/**
 * @brief Macro to check if a protothread is currently running.
 *
 * @param pt A pointer to the protothread control structure.
 *
 * @details
 * This macro evaluates to true if the protothread is currently active and
 * has not reached its terminated state. It checks if the local continuation
 * state (lc) is neither in its initial value nor in the default termination
 * value, indicating that the protothread is in the midst of its execution.
 *
 * Example Usage:
 * \code{.c}
 * pt_t my_protothread;
 * if (PT_IS_RUNNING(&my_protothread)) {
 *     // Perform operations while the protothread is running
 * }
 * \endcode
 */
#define PT_IS_RUNNING(pt) ( ((pt)->lc > 0) && ((pt)->lc != LC_DEFAULT) )




/**
 * @brief Initialize a protothread.
 *
 * @param pt A pointer to the protothread control structure.
 *
 * @details This macro initializes the protothread's local continuation state.
 *          It should be called before the protothread is first scheduled to run.
 *
 * Example Usage:
 * \code{.c}
 * pt_t my_protothread;
 * PT_INIT(&my_protothread);
 * \endcode
 */
#define PT_INIT(pt) LC_INIT((pt)->lc)

/**
 * @brief Declaration of a protothread.
 *
 * @param name_args The name and arguments of the C function implementing the protothread.
 *
 * @details This macro declares a new protothread. The protothread is implemented as a C function
 *          that returns a `ptstate_t`. This macro should be placed at the start of the function
 *          that defines the protothread.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(my_protothread_thread(pt_t *pt))
 * {
 *     // Protothread implementation
 * }
 * \endcode
 */
#define PT_THREAD(name_args) ptstate_t name_args

/**
 * @brief Start a protothread.
 *
 * @param pt A pointer to the protothread control structure.
 *
 * @details This macro marks the beginning of a protothread. It should be placed
 *          at the start of the protothread function. It sets up the protothread
 *          to resume execution at the correct position using the local continuation state.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(my_protothread_thread(pt_t *pt))
 * {
 *     PT_BEGIN(pt);
 *     // Protothread code here...
 * }
 * \endcode
 */
#define PT_BEGIN(pt) LC_RESUME((pt)->lc)

/**
 * @brief End a protothread.
 *
 * @param pt A pointer to the protothread control structure.
 *
 * @details This macro marks the end of a protothread. It should be placed at the end
 *          of the protothread function. It sets the protothread's state to terminated
 *          and returns the termination state.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(my_protothread_thread(pt_t *pt))
 * {
 *     PT_BEGIN(pt);
 *     // Protothread code here...
 *     PT_END(pt);
 * }
 * \endcode
 */
#define PT_END(pt) LC_SET_DEFAULT((pt)->lc); LC_END((pt)->lc); return PT_STATE_TERMINATED




/**
 * @brief Block and wait until a condition becomes true.
 *
 * @param pt A pointer to the protothread control structure.
 * @param condition The condition on which the protothread should wait.
 *
 * @details
 * This macro is used to block the execution of a protothread until a specified
 * condition is met. The protothread yields and will be resumed at this point when
 * it is next scheduled for execution. If the condition is false, the protothread
 * returns `PT_STATE_WAITING`, indicating it is waiting for the condition to become true.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(my_protothread_thread(pt_t *pt))
 * {
 *     PT_BEGIN(pt);
 *
 *     // Wait until the condition 'data_ready' becomes true
 *     PT_WAIT_UNTIL(pt, data_ready);
 *     // Code here executes once 'data_ready' is true
 *
 *     PT_END(pt);
 * }
 * \endcode
 *
 * @note The condition is checked each time the protothread is scheduled.
 */
#define PT_WAIT_UNTIL(pt, condition)    \
  do {                                  \
    LC_SET((pt)->lc);                   \
    if(!(condition)) {                  \
      return PT_STATE_WAITING;          \
    }                                   \
  } while(0)

/**
 * @brief Block and wait while a condition is true.
 *
 * @param pt A pointer to the protothread control structure.
 * @param cond The condition on which the protothread should continue waiting.
 *
 * @details
 * This macro is used to block the execution of a protothread as long as the specified
 * condition is true. The protothread yields and will resume at this point when it is next
 * scheduled for execution. The protothread remains in the `PT_STATE_WAITING` state until
 * the condition becomes false.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(my_protothread_thread(pt_t *pt))
 * {
 *     PT_BEGIN(pt);
 *
 *     // Wait while the condition 'busy_flag' is true
 *     PT_WAIT_WHILE(pt, busy_flag);
 *     // Code here executes once 'busy_flag' is false
 *
 *     PT_END(pt);
 * }
 * \endcode
 *
 * @note This macro is essentially the inverse of PT_WAIT_UNTIL, causing the protothread to
 *       wait as long as the condition is true rather than waiting for it to become true.
 */
#define PT_WAIT_WHILE(pt, cond)  PT_WAIT_UNTIL((pt),!(cond))


/**
 * @brief Block and wait until a child protothread completes.
 *
 * @param pt A pointer to the protothread control structure.
 * @param thread The child protothread to be waited on.
 *
 * @details
 * This macro is used within a protothread to wait for the completion of a child protothread.
 * The parent protothread (the one using this macro) is blocked until the specified child
 * protothread has terminated. The child protothread is scheduled and executed in the meantime.
 *
 * The macro `PT_SCHEDULE` is used to schedule and run the child protothread. The parent
 * protothread remains in the `PT_STATE_WAITING` state as long as the child protothread
 * is running.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(parent_thread(pt_t *pt))
 * {
 *     static pt_t child;
 *     PT_BEGIN(pt);
 *
 *     PT_INIT(&child);
 *     PT_WAIT_THREAD(pt, child_thread(&child));
 *     // Code here executes once 'child_thread' has terminated
 *
 *     PT_END(pt);
 * }
 *
 * PT_THREAD(child_thread(pt_t *pt))
 * {
 *     PT_BEGIN(pt);
 *     // Child protothread's code...
 *     PT_END(pt);
 * }
 * \endcode
 *
 * @note It is important to ensure that the child protothread is properly initialized
 *       before using this macro to wait for it.
 */
#define PT_WAIT_THREAD(pt, thread) PT_WAIT_WHILE((pt), PT_SCHEDULE(thread) == PT_STATE_TERMINATED)

/**
 * @brief Spawn a child protothread and wait until it exits.
 *
 * @param pt A pointer to the protothread control structure of the parent protothread.
 * @param child A pointer to the protothread control structure of the child protothread.
 * @param thread The child protothread to be executed.
 *
 * @details
 * This macro is used to start (or spawn) a child protothread within the context of a parent
 * protothread. The macro initializes the child protothread and then waits for it to complete
 * its execution. The parent protothread is blocked until the child protothread terminates.
 *
 * The macro combines `PT_INIT` to initialize the child protothread, and `PT_WAIT_THREAD` to
 * wait for the child protothread to finish. This is commonly used for structured concurrency
 * where the parent protothread needs to perform some action after the child protothread
 * completes.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(parent_thread(pt_t *pt))
 * {
 *     static pt_t child;
 *     PT_BEGIN(pt);
 *
 *     PT_SPAWN(pt, &child, child_thread(&child));
 *     // Code here executes once 'child_thread' has terminated
 *
 *     PT_END(pt);
 * }
 *
 * PT_THREAD(child_thread(pt_t *pt))
 * {
 *     PT_BEGIN(pt);
 *     // Child protothread's code...
 *     PT_END(pt);
 * }
 * \endcode
 *
 * @note The child protothread is automatically initialized by this macro. Ensure that
 *       the child protothread function is correctly defined and can be executed.
 */
#define PT_SPAWN(pt, child, thread)     \
  do {                                  \
    PT_INIT((child));                   \
    PT_WAIT_THREAD((pt), (thread));     \
  } while(0)



/**
 * @brief Restart the protothread.
 *
 * @param pt A pointer to the protothread control structure.
 *
 * @details
 * This macro causes the protothread to restart its execution from the beginning.
 * It reinitializes the protothread's state and returns `PT_STATE_WAITING`, indicating
 * that the protothread is ready to be scheduled again from its start.
 *
 * This is useful in situations where the protothread needs to be reset and run again
 * from the beginning, typically in response to certain events or conditions.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(my_protothread(pt_t *pt))
 * {
 *     PT_BEGIN(pt);
 *
 *     // Protothread's code...
 *
 *     if(need_to_restart) {
 *         PT_RESTART(pt);
 *     }
 *
 *     PT_END(pt);
 * }
 * \endcode
 */
#define PT_RESTART(pt)          \
  do {                      \
    PT_INIT(pt);            \
    return PT_STATE_WAITING;      \
  } while(0)

/**
 * @brief Exit the protothread.
 *
 * @param pt A pointer to the protothread control structure.
 *
 * @details
 * This macro causes the protothread to exit. It sets the local continuation state
 * to its default termination state and returns `PT_STATE_TERMINATED`, indicating that
 * the protothread has completed its execution.
 *
 * This macro is typically used to voluntarily terminate a protothread from within its
 * code, often as a response to certain conditions being met, or when the protothread has
 * completed its intended task.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(my_protothread(pt_t *pt))
 * {
 *     PT_BEGIN(pt);
 *
 *     // Protothread's code...
 *
 *     if(need_to_exit) {
 *         PT_EXIT(pt);
 *     }
 *
 *     PT_END(pt);
 * }
 * \endcode
 */
#define PT_EXIT(pt)             \
  do {                          \
    LC_SET_DEFAULT((pt)->lc);   \
    return PT_STATE_TERMINATED; \
  } while(0)

/**
 * @brief Schedule a protothread for execution.
 *
 * @param f The call to the C function implementing the protothread.
 *
 * @details
 * This macro is used to schedule a protothread for execution. It calls the
 * function that implements the protothread and returns its state. The state
 * indicates whether the protothread is still running or has terminated.
 *
 * The macro is a simple wrapper that directly calls the protothread function.
 * It is used to make the code that schedules protothreads more readable and
 * consistent.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(my_protothread(pt_t *pt))
 * {
 *     // Implementation of the protothread
 * }
 *
 * void scheduler()
 * {
 *     static pt_t pt;
 *     PT_INIT(&pt);
 *
 *     while(1) {
 *         // Schedule the protothread
 *         PT_SCHEDULE(my_protothread(&pt));
 *     }
 * }
 * \endcode
 *
 * @note This macro does not provide any mechanism to control when or how often
 *       the protothread is scheduled. It simply calls the protothread function,
 *       leaving the scheduling logic to the programmer.
 */
#define PT_SCHEDULE(f) (f)

/**
 * @brief Yield from the current protothread.
 *
 * @param pt A pointer to the protothread control structure.
 *
 * @details
 * This macro yields the execution of the current protothread, allowing other
 * processing to take place in the system. The protothread pauses its execution
 * and can be resumed later from this point. When the protothread is resumed,
 * it continues execution from the statement following the `PT_YIELD`.
 *
 * This is particularly useful for cooperative multitasking, where each
 * protothread voluntarily yields control periodically to allow other
 * protothreads to run.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(my_protothread(pt_t *pt))
 * {
 *     PT_BEGIN(pt);
 *
 *     while(1) {
 *         // Perform some action
 *         // ...
 *
 *         // Yield control to other protothreads
 *         PT_YIELD(pt);
 *     }
 *
 *     PT_END(pt);
 * }
 * \endcode
 *
 * @note After yielding, the protothread is still in a running state, but it
 *       gives up the CPU to allow other tasks or protothreads to execute.
 */
#define PT_YIELD(pt)                            \
    do {                                        \
        LC_SET_YIELD((pt)->lc, PT_STATE_WAITING);     \
    } while(0)


/**
 * @brief Yield from the protothread until a condition is true.
 *
 * @param pt A pointer to the protothread control structure.
 * @param cond The condition that must become true to resume the protothread.
 *
 * @details
 * This macro yields the protothread, allowing other processing to take place,
 * and will only resume the protothread when the specified condition becomes true.
 * The condition is evaluated each time the protothread is scheduled to run. If
 * the condition is false, the protothread yields again.
 *
 * This is useful when a protothread needs to wait for an external event or
 * condition without blocking the entire system. The protothread will continue
 * executing from the point of yield once the condition is met.
 *
 * Example Usage:
 * \code{.c}
 * PT_THREAD(my_protothread(pt_t *pt))
 * {
 *     PT_BEGIN(pt);
 *
 *     // Wait until 'data_ready' is true
 *     PT_YIELD_UNTIL(pt, data_ready);
 *     // Code here executes once 'data_ready' is true
 *
 *     PT_END(pt);
 * }
 * \endcode
 *
 * @note This macro is a convenient way to make the protothread wait for specific
 *       conditions without entering a blocking state, thus allowing other
 *       protothreads or tasks to run.
 */
#define PT_YIELD_UNTIL(pt, cond)                    \
    do {                                            \
        LC_SET_YIELD((pt)->lc, PT_STATE_WAITING);   \
    } while(!(cond))



#endif /* PT_H_ */


