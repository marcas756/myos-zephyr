#include "hash.h"

uint32_t hash_sdbm(uint32_t seed, void *data, size_t size)
{
	int idx;

	for(idx = 0; idx < size; idx++)
	{
		seed = hash_sdbm_acc(seed,((uint8_t*)data)[idx]);		
	}

    	return seed;
}








