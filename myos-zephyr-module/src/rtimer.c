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


#include "rtimer.h"
#include <stdlib.h>
#include "mutex.h"


rtimer_t *rtimer_next = NULL;
mutex_t rtimer_mutex = false;

bool rtimer_lock(void)
{
   return mutex_lock(&rtimer_mutex);
}

void rtimer_release()
{   
   rtimer_next = NULL;
   mutex_release(&rtimer_mutex);
}

bool rtimer_is_locked()
{
   return mutex_is_locked(&rtimer_mutex);
}


void rtimer_scheduler (void)
{
   rtimer_t *rtimer = rtimer_next;
   rtimer_release();

   if( rtimer->callback )
   {
      rtimer->callback(rtimer->data);
   }
}

rtimer_timespan_t rtimer_left(rtimer_t *rtimer)
{
    rtimer_timestamp_t now = rtimer_now();
    rtimer_timestamp_t stop = rtimer_timestamp_stop(rtimer);

    if( rtimer_timestamp_less_than(now,stop) )
    {
       return stop - now;
    }

    return 0;
}

void rtimer_start(rtimer_t *rtimer, rtimer_timespan_t span, rtimer_callback_t callback, void* data)
{
   rtimer->start = rtimer_now();
   rtimer->span = span;
   rtimer->callback = callback;
   rtimer->data = data;
   rtimer_next = rtimer;
   rtimer_arch_timer_set(rtimer_timestamp_stop(rtimer));
}

void rtimer_restart(rtimer_t *rtimer)
{
   rtimer->start = rtimer_now();
   rtimer_next = rtimer;
   rtimer_arch_timer_set(rtimer_timestamp_stop(rtimer));
}

void rtimer_reset(rtimer_t *rtimer)
{
   rtimer->start += rtimer->span;
   rtimer_next = rtimer;
   rtimer_arch_timer_set(rtimer_timestamp_stop(rtimer));
}
