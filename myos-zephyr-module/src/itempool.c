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
    \file   itempool.c

    \brief	see itempool.h

    \details see itempool.h
	
*/

#include "itempool.h"

/*
 tmp = (int*)ITEMPOOL_ALLOC(pool);
 00442E    403F 0005          mov.w   #0x5,R15          !!!! pass param
 004432    432E               mov.w   #0x2,R14          !!!! pass param
 004434    403D 1C00          mov.w   #0x1C00,R13       !!!! pass param
 004438    403C 1C06          mov.w   #0x1C06,R12       !!!! pass param
 00443C    13B0 4482          calla   #itempool_alloc
 004440    4C0A               mov.w   R12,R10
 */
void* itempool_alloc(uint8_t* items, uint8_t* status, size_t itemsize, size_t poolsize)
{
    size_t tmp;
	
    for (tmp=0; tmp < poolsize; tmp++)
    {
        if (status[tmp] == ITEMPOOL_ITEM_FREE)
        {
            status[tmp] =  ITEMPOOL_ITEM_USED;
            return items;
        }
        items+=itemsize;
    }

    return NULL;
}


void* itempool_calloc(uint8_t* items, uint8_t* status, size_t itemsize, size_t poolsize)
{
    items = itempool_alloc(items,status,itemsize,poolsize);

    if (items)
    {
        memset(items,0x00,itemsize);
    }

    return items;
}

