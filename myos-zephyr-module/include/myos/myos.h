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

#ifndef MYOS_H_
#define MYOS_H_



#include "critical.h"
#include "pt.h"
#include "process.h"

#include "timestamp.h"
#include "timer.h"
#include "ptimer.h"
#include "ctimer.h"
#include "etimer.h"
#include "rtimer.h"

#include <zephyr/kernel.h>

#if defined(CONFIG_MYOS_STATISTICS)

typedef struct {
   unsigned realtime : 1;
   unsigned eventqueue : 1;
}myos_errflags_t;

typedef struct {
   myos_errflags_t errflags;
   rtimer_timespan_t maxlaptime;
   rtimer_timespan_t maxproctime;
   uint8_t maxqueuecount;
   uint8_t ptlist_size_max;
}myos_stats_t;

extern myos_stats_t myos_stats;

#endif

void myos_init(void);


#endif /* MYOS_H_ */
