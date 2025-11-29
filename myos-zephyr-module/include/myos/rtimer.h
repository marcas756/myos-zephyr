/*
 * Copyright (c) 2005, Swedish Institute of Computer Science
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



#ifndef RTIMER_H_
#define RTIMER_H_

#include "rtimer_arch.h"
#include "stdbool.h"
#include "mutex.h"

typedef rtimer_arch_timestamp_t rtimer_timestamp_t;
typedef rtimer_timestamp_t rtimer_timespan_t;

#define RTIMER_TICKS_PER_SEC                RTIMER_ARCH_TICKS_PER_SEC
#define rtimer_now()                        rtimer_arch_now()
#define rtimer_init()                       rtimer_arch_init()
#define RTIMER_TIMESTAMP_DIFF               RTIMER_TIMESTAMP_ARCH_DIFF
#define rtimer_timestamp_less_than(a,b)    (RTIMER_TIMESTAMP_DIFF((a),(b)) < 0)
typedef void(*rtimer_callback_t)(void* data);

typedef struct {
   rtimer_timestamp_t start;
   rtimer_timespan_t span;
   rtimer_callback_t callback;
   void* data;
} rtimer_t;


#define PROCESS_RTIMER_ACQUIRE()          \
   do{                                    \
      while( rtimer_lock() == false )     \
      {                                   \
         PROCESS_YIELD();               \
      }                                   \
   }while(0)


void rtimer_start(rtimer_t *rtimer, rtimer_timespan_t span, rtimer_callback_t callback, void* data);
void rtimer_reset(rtimer_t *rtimer);
rtimer_timespan_t rtimer_left(rtimer_t *rtimer);
#define rtimer_expired(rtimerptr) (rtimer_left(rtimerptr) == 0)
#define rtimer_timestamp_stop(rtimerptr) ((rtimerptr)->start+(rtimerptr)->span)
bool rtimer_lock(void);

#define  rtimer_module_init  rtimer_arch_module_init

#endif /* SOURCE_OS_RTIMER_H_ */
