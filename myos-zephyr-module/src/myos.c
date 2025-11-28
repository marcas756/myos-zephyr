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

#include "myos.h"
#include "process.h"
#include "debug.h"

#if defined(CONFIG_MYOS_DEBUG_CORE)
#define DBG(...) DBG_FUNC(__VA_ARGS__)
#else
#define DBG(...) do{}while(0)
#endif

#if defined(CONFIG_MYOS_STATISTICS)

myos_stats_t myos_stats;

PROCESS(idle_process,idle_process);
PROCESS_THREAD(idle_process)
{
   static timestamp_t start,stop;
   static rtimer_timestamp_t rtstart,rtstop;

   PROCESS_BEGIN();

   start = timestamp_now();
   rtstart = rtimer_now();

   while(1)
   {
      PROCESS_SUSPEND();

      rtstop = rtimer_now();
      stop = timestamp_now();


      if( TIMESTAMP_DIFF(stop,start) > 1 ) // more than one tick behind ?
      {
         myos_stats.errflags.realtime = 1;
      }

      rtstart = RTIMER_TIMESTAMP_DIFF(rtstop,rtstart);

      if ( rtstart > myos_stats.maxlaptime )
      {
         myos_stats.maxlaptime = rtstart;
      }

      start = stop;
      rtstart = rtstop;
   }

   PROCESS_END();
}
#endif


void myos_init(void)
{
   DBG("MyOS init\n");
   process_init();   
   timestamp_module_init();
   timer_module_init();
   ptimer_module_init();
   etimer_module_init();
   ctimer_module_init();
   rtimer_init();




#if defined(CONFIG_MYOS_STATISTICS)
    process_start(&idle_process,NULL);
#endif



}
