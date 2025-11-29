#include "timestamp.h"
#include "debug.h"


#if defined(CONFIG_MYOS_DEBUG_TIMESTAMP)
#define DBG(...) DBG_FUNC(__VA_ARGS__)
#else
#define DBG(...) do{}while(0)
#endif

void timestamp_module_init(void)
{
      timestamp_arch_module_init();
      DBG("timestamp: initialized, ticks per sec: %d\n", TIMESTAMP_TICKS_PER_SEC);
}