
#include <zephyr/kernel.h>
#include <zephyr/drivers/counter.h> // to access TIM6 on STM32	
#include "debug.h"
#include "rtimer_arch.h"

#if defined(CONFIG_MYOS_DEBUG_RTIMER)
#define DBG(...) DBG_FUNC(__VA_ARGS__)
#else
#define DBG(...) do{}while(0)
#endif

extern const struct device *const tim9_counter_dev;

void rtimer_arch_init()
{

}

rtimer_arch_timestamp_t rtimer_arch_now(void)
{
    uint32_t timestamp;
    counter_get_value(tim9_counter_dev, &timestamp);
    return (rtimer_arch_timestamp_t)timestamp;
}


extern void rtimer_scheduler (void);
void rtimer_arch_timer_callback(const struct device *dev,
                                uint8_t chan_id,
                                uint32_t ticks,
                                void *user_data)
{
    ARG_UNUSED(dev);
    ARG_UNUSED(chan_id);
    ARG_UNUSED(user_data);

    rtimer_scheduler(); 
}




void rtimer_arch_timer_set(rtimer_arch_timestamp_t stop)
{
    struct counter_alarm_cfg alarm_cfg = {0};
    alarm_cfg.flags = COUNTER_ALARM_CFG_ABSOLUTE;
    alarm_cfg.ticks = stop;
    alarm_cfg.callback = rtimer_arch_timer_callback;  
    alarm_cfg.user_data = NULL;

    counter_set_channel_alarm(tim9_counter_dev, 0, &alarm_cfg);
}




