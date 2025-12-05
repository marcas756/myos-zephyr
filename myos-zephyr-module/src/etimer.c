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



#include "etimer.h"
#include "debug.h"


#if defined(CONFIG_MYOS_DEBUG_ETIMER)
#define DBG(...) DBG_FUNC(__VA_ARGS__)
#else
#define DBG(...) do{}while(0)
#endif


extern bool process_deliver_event(process_event_t *evt);


/*!
 * @brief Handler for event timer expiration.
 * @details This function serves as the timeout handler for event timers (etimers) in MyOS. It's invoked when an etimer expires.
 *          The function casts the provided ptimer to its derived etimer type and then checks if the destination process for
 *          the event is running. If so, it delivers the scheduled event to the target process using process_deliver_event.
 *          This mechanism is key to the event-driven architecture of MyOS, allowing timers to trigger specific actions in processes.
 * @param[in] ptimer Pointer to the process timer associated with the etimer.
 *
 * Usage Example:
 * @code
 *     // This handler is used internally by etimers and not typically called directly by user code.
 * @endcode
 */
static void etimer_timeout_handler(ptimer_t* ptimer)
{  
   DBG("etimer : timeout handler called\n");
   process_event_t *evt = &((etimer_t*)ptimer)->evt;

#if defined(CONFIG_MYOS_ETIMER_DEFER_EVENTS)
   PROCESS_CONTEXT_BEGIN(evt->from);
   process_post(evt->to, evt->id, evt->data);
   PROCESS_CONTEXT_END();
#else
   process_deliver_event(evt);
#endif

}



void etimer_start(etimer_t *etimer, timespan_t span, process_t *to, process_event_id_t evtid, void *data)
{
   etimer->evt.id = evtid;
   etimer->evt.data = data;
   etimer->evt.from = PROCESS_THIS();
   etimer->evt.to = to;
   ptimer_start(&(etimer->ptimer), span, etimer_timeout_handler);
}


