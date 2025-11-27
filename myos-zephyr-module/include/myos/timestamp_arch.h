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
    \file       timestamp_arch.h

    \brief      Architecture depending part of time stamp implementation

    \details    Provides the architecture dependent part of time stamp implementation
                for architecture hosted.

                Hosted means that the myos runs in an own process on top of a host
                operating system.

*/


#ifndef TIMESTAMP_ARCH_H_
#define TIMESTAMP_ARCH_H_

#include <stdint.h>
#include <zephyr/kernel.h>




#ifdef CONFIG_ARCH_POSIX
#include <time.h>
#define TIMESTAMP_ARCH_TICKS_PER_SEC CLOCKS_PER_SEC
#else
#define TIMESTAMP_ARCH_TICKS_PER_SEC CONFIG_SYS_CLOCK_TICKS_PER_SEC
#endif



typedef uint32_t timestamp_arch_t;
#define TIMESTAMP_ARCH_DIFF(a,b)         ((int32_t)((a)-(b)))
#define timestamp_arch_module_init() do{}while(0)
timestamp_arch_t timestamp_arch_now(void);

#endif /* TIMESTAMP_ARCH_H_ */
