#include "basic.h"
#include <stdio.h>

CYCLES main_memory_access_latency()
{
    int* tmp_for_address = (int*) malloc(sizeof(int));
	CYCLES cur_med = 0, prev_med = 0, prev_prev_med = 0;
	CYCLES latencies[LATENCIES_CHECK_ITERATIONS] = {0}; 

	int count = 1;
    for (; count <= LATENCIES_CHECK_ITERATIONS; count++)
    {
        clflush((ADDR_PTR)tmp_for_address);
        latencies[count] = measure_access_time_to_addr((ADDR_PTR)tmp_for_address);

        if(count == 1)
            continue;

        array_sort(latencies, count);
        cur_med = find_median(latencies, count);

        if(cur_med == prev_med && prev_med == prev_prev_med)
        {
            printf("med = %u, max = %u, min = %u \n", cur_med, latencies[count-1], latencies[0]);
            free(tmp_for_address);
            return cur_med;
        }
        prev_prev_med = prev_med;
        prev_med = cur_med;
    }
    printf("wrong values: med = %u, max = %u, min = %u \n", cur_med, latencies[count-1], latencies[0]);
    free(tmp_for_address);
    return cur_med; // won't get here
}

CYCLES last_level_cache_access_latency()
{
	return 0;
}

CYCLES first_level_cache_access_latency()
{
	return 0;
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
