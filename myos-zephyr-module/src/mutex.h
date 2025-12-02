#ifndef MUTEX_H_
#define MUTEX_H_

#include <stdbool.h>

typedef bool mutex_t;

bool mutex_lock(mutex_t *mutex);
void mutex_release(mutex_t *mutex);
bool mutex_is_locked(mutex_t *mutex);

#endif /* MUTEX_H_ */
