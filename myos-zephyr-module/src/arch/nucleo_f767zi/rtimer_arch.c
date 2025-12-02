
#include <zephyr/kernel.h>
#include <zephyr/drivers/counter.h> // to access TIM6 on STM32	
#include "debug.h"
#include "rtimer_arch.h"

#if defined(CONFIG_MYOS_DEBUG_RTIMER)
#define DBG(...) DBG_FUNC(__VA_ARGS__)
#else
#define DBG(...) do{}while(0)
#endif

/* Das ist der "counter" Subnode unter timers6 */
#define TIM6_COUNTER_NODE DT_CHILD(DT_NODELABEL(timers6), counter)

const struct device *const tim6_counter_dev =
    DEVICE_DT_GET(TIM6_COUNTER_NODE);

void rtimer_arch_init()
{
    if (!device_is_ready(tim6_counter_dev)) 
	{
    	DBG("rtimer: TIM6 counter device not ready!\n");
		return;	
	}

    if (counter_start(tim6_counter_dev) != 0) 
	{
        DBG("rtimer: counter_start failed: \n");
        return;
    }    
}

rtimer_arch_timestamp_t rtimer_arch_now(void)
{
    uint32_t timestamp;
    counter_get_value(tim6_counter_dev, &timestamp);
    return (rtimer_arch_timestamp_t)timestamp;
}




