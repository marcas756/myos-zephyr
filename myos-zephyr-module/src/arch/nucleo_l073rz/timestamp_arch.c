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


#include"timestamp_arch.h" 
#include <zephyr/kernel.h>
#include <zephyr/drivers/counter.h> // to access TIM21 on STM32	
#include "debug.h"


#if defined(CONFIG_MYOS_DEBUG_TIMESTAMP)
#define DBG(...) DBG_FUNC(__VA_ARGS__)
#else
#define DBG(...) do{}while(0)
#endif

static timestamp_arch_t timestamp_counter = 0;

#define TIM21_COUNTER_NODE DT_CHILD(DT_NODELABEL(timers21), counter)

const struct device *const tim21_counter_dev =
    DEVICE_DT_GET(TIM21_COUNTER_NODE);



timestamp_arch_t timestamp_arch_now(void)
{
      timestamp_arch_t t1,t2;

      do{
            t1 = timestamp_counter;
            t2 = timestamp_counter;
      }while(t1 != t2);

      return t1;
}

#define TIM21_TICKS_PER_MS 50U   /* 50 kHz -> 1 ms = 50 ticks */


void timestamp_arch_myos_tick(const struct device *dev,
                                uint8_t chan_id,
                                uint32_t ticks,
                                void *user_data);

static struct counter_alarm_cfg alarm_cfg = {
            .flags    = COUNTER_ALARM_CFG_ABSOLUTE,
            .ticks    = 0,
            .callback = timestamp_arch_myos_tick,
            .user_data = NULL,
      };

void timestamp_arch_myos_tick(const struct device *dev,
                                uint8_t chan_id,
                                uint32_t ticks,
                                void *user_data)
{

      ARG_UNUSED(user_data);

      timestamp_counter++;






       /* Re-arm same channel (1) for the next 1 ms */
      alarm_cfg.ticks = (timestamp_arch_t)(ticks + TIM21_TICKS_PER_MS);
     
      if (counter_set_channel_alarm(dev, chan_id, &alarm_cfg) != 0)
      {
         DBG("timestamp: set channel alarm failed %ul!\n",alarm_cfg.ticks);  
      }

}


void timestamp_arch_module_init(void)
{

      uint32_t now;
      int err;
      
      if (!device_is_ready(tim21_counter_dev)) 
	{
    	DBG("timestamp: counter device not ready!\n");
		return;	
	}

      if (counter_start(tim21_counter_dev) != 0) 
            {
            DBG("timestamp: counter_start failed: \n");
            return;
      }   

      err = counter_get_value(tim21_counter_dev, &now);
      if (err < 0) {
            DBG("timestamp: counter_get_value failed: %d\n", err);
            return;
      }

      alarm_cfg.ticks = (timestamp_arch_t)(now + TIM21_TICKS_PER_MS);

      counter_set_channel_alarm(tim21_counter_dev, 1, &alarm_cfg);
      
      DBG("timetamp: initialized: \n");
}