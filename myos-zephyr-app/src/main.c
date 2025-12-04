/*
 * Copyright (c) 2025 Marco Bacchi
 */


#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>

#include "myos.h"


LOG_MODULE_REGISTER(myos, LOG_LEVEL_INF);

static int my_early_init(void)
{
	printk("Early init \n");
	return 0;
}

SYS_INIT(my_early_init, POST_KERNEL, 50);

void rtimer_callback(void* data)
{
	static int cnt = 0;

	if( cnt++ < 10 )
	{
		rtimer_t* rt = (rtimer_t*)data;	
		rtimer_reset(rt);
		rtimer_lock();
	}

	LOG_INF("Rtimer callback fired %d",cnt);
}


PROCESS(counter,counter);
PROCESS_THREAD(counter)
{
	static etimer_t et;
	static rtimer_t rt;

	static uint32_t cnt = 0;

    PROCESS_BEGIN();

	LOG_INF("Started counter process");

	PROCESS_RTIMER_ACQUIRE();
	rtimer_start(&rt, RTIMER_TICKS_PER_SEC/4 , rtimer_callback, &rt);
	PROCESS_RTIMER_JOIN();
	
	while(1)
	{
		LOG_INF("Process loop %d",cnt++);
		PROCESS_SLEEP(&et,TIMESTAMP_TICKS_PER_SEC);
	}


	PROCESS_END();
}


void myos_scheduler(void)
{

	LOG_INF("MyOS thread running");
	myos_init();

	process_start(&counter,NULL);

	for(;;)
	{
		process_run();
	}
}
 

K_THREAD_DEFINE(
	myos_scheduler_id, 
	CONFIG_MYOS_STACK_SIZE, 
	myos_scheduler, 
	NULL, 
	NULL, 
	NULL, 		
	CONFIG_MYOS_THREAD_PRIORITY,
	0, 
	0);
