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


#ifndef RTIMER_ARCH_H_
#define RTIMER_ARCH_H_

#include <stdint.h>  
#include "timestamp_arch.h"

typedef uint16_t rtimer_arch_timestamp_t;
typedef uint16_t rtimer_arch_timespan_t;

#define RTIMER_TIMESTAMP_ARCH_DIFF(a,b)         ((int16_t)((a)-(b)))
#define RTIMER_ARCH_TICKS_PER_SEC               1000// to access TIM6 on STM32	


#define rtimer_arch_now()  timestamp_arch_now()    
void rtimer_arch_timer_set(rtimer_arch_timestamp_t stop);
#define rtimer_arch_init()  do{}while(0)

#endif /* RTIMER_ARCH_H_ */
