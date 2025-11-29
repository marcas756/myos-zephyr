/*
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
   \file       critical.h
   \copyright  Copyright 2013-2023 Marco Bacchi <marco@bacchi.at>
   \license    This file is released under the MIT License.\n
               https://opensource.org/license/mit/

   \brief   Critical section handling

   \details In concurrent programming, concurrent accesses to shared resources
            can lead to unexpected or erroneous behavior, so parts of the
            program where the shared resource is accessed need to be protected
            in ways that avoid the concurrent access.

            One way to do so is known as a critical section or critical region.
            This protected section cannot be entered by more than one process
            or thread at a time; others are suspended until the first leaves
            the critical section. Typically, the critical section accesses a
            shared resource, such as a data structure, a peripheral device, or
            a network connection, that would not operate correctly in the
            context of multiple concurrent accesses.

            The simplest method to prevent any change of processor control
            inside the critical section is implementing a semaphore. In
            uniprocessor systems, this can be done by disabling interrupts on
            entry into the critical section, avoiding system calls that can
            cause a context switch while inside the section, and restoring
            interrupts to their previous state on exit.

            https://en.wikipedia.org/wiki/Critical_section

            Critical sections should be kept as short as possible to minimize
            the time during which interrupts are disabled. This helps reduce
            the impact on the overall system responsiveness.
*/


#ifndef CRITICAL_H_
#define CRITICAL_H_
#include "critical_arch.h"

/*!
   \brief   Begin of a critical section in code
   \details Inside a critical section, it's common to disable interrupts
            temporarily to prevent other interrupt sources from preempting the
            current ISR. This ensures that the critical section is executed
            atomically without interruption.

            This macro disables the interrupts before entering the critical section.
*/
#define CRITICAL_SECTION_BEGIN  CRITICAL_ARCH_SECTION_BEGIN

/*!
   \brief   End of a critical section in code
   \details Once the critical section is completed, interrupts are re-enabled
            to allow the processor to respond to other interrupt sources.

            This macro enables the interrupts before leaving the critical section.
*/
#define CRITICAL_SECTION_END    CRITICAL_ARCH_SECTION_END

/*!
   \brief   Critical section for one statement
   \details This macro disables interrupts before entering the critical section,
            executes the statement provided and enables interrupts again before
            leaving the critical section.

            Usage example : CRITICAL_STATEMENT( a = b );
*/
#define CRITICAL_STATEMENT(x)    \
   do{                           \ 
      CRITICAL_SECTION_BEGIN();  \
      x;                         \
      CRITICAL_SECTION_END();    \
   }while(0)


#endif /* CRITICAL_H_ */
