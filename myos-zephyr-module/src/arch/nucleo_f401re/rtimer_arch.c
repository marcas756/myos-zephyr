#include <zephyr/kernel.h>
#include <zephyr/drivers/counter.h> // use TIM9 on STM32
#include "debug.h"
#include "rtimer_arch.h"

#if defined(CONFIG_MYOS_DEBUG_RTIMER)
#define DBG(...) DBG_FUNC(__VA_ARGS__)
#else
#define DBG(...) do {} while (0)
#endif

/* Das ist der "counter"-Subnode unter timers9 (TIM9 General-Purpose Timer) */
#define TIM9_COUNTER_NODE DT_CHILD(DT_NODELABEL(timers9), counter)

const struct device *const tim9_counter_dev =
    DEVICE_DT_GET(TIM9_COUNTER_NODE);

void rtimer_arch_init(void)
{
    if (!device_is_ready(tim9_counter_dev)) 
    {
        DBG("rtimer: TIM9 counter device not ready!\n");
        return;
    }

    if (counter_start(tim9_counter_dev) != 0) 
    {
        DBG("rtimer: counter_start(TIM9) failed!\n");
        return;
    }
}

rtimer_arch_timestamp_t rtimer_arch_now(void)
{
    uint32_t timestamp;
    counter_get_value(tim9_counter_dev, &timestamp);
    return (rtimer_arch_timestamp_t)timestamp;
}