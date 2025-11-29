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


PROCESS(counter,counter);
PROCESS_THREAD(counter)
{
	static etimer_t et;

	static uint32_t cnt = 0;

    PROCESS_BEGIN();

	LOG_INF("Started counter process");
	
	while(1)
	{
		LOG_INF("Counter Process : %d", cnt++);
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
