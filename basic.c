#include "basic.h"
#include <stdio.h>

CYCLES main_memory_access_latency()
{
	implement_me;
}

CYCLES last_level_cache_access_latency()
{
	implement_me;
}

CYCLES first_level_cache_access_latency()
{

	implement_me;
}

int main()
{
	CYCLES mem_access_latency;
       	mem_access_latency = main_memory_access_latency();
	CYCLES l1_access_latency;
       	l1_access_latency = first_level_cache_access_latency();
	CYCLES llc_access_latency;
       	llc_access_latency = last_level_cache_access_latency();

	printf("median main memory access latency: %u\n", mem_access_latency);
	printf("median first level cache access latency: %u\n", l1_access_latency);
	printf("median last level cache access latency: %u\n", llc_access_latency);
}
