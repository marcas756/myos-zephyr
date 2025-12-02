#include "mutex.h"
#include "critical.h"

bool mutex_lock(mutex_t *mutex)
{
   bool status = false;

   CRITICAL_SECTION_BEGIN();

   if (*mutex == false)
   {
      *mutex = true;
      status = true;
   }    

   CRITICAL_SECTION_END();

   return status;
}


void mutex_release(mutex_t *mutex)
{
   CRITICAL_STATEMENT(*mutex = false);
}   


bool mutex_is_locked(mutex_t *mutex)
{
   bool status;
   CRITICAL_STATEMENT(status = *mutex);
   return status;
}