#include "basic.h"
#include <stdio.h>

CYCLES main_memory_access_latency()
{
    int* memory_address = (int*)malloc(sizeof(int));
	CYCLES curr_med = 0, prev_med = 0, prev_prev_med = 0;
	CYCLES latencies[LATENCIES_CHECK_ITERATIONS] = {0}; 

	int count = 1;
    for (; count <= LATENCIES_CHECK_ITERATIONS; count++)
    {
        clflush((ADDR_PTR)memory_address);
        latencies[count - 1] = measure_access_time_to_addr((ADDR_PTR)memory_address);

        if(count == 1)
            continue;

        sort(latencies, count);
        curr_med = find_median(latencies, count);

        if(curr_med == prev_med && prev_med == prev_prev_med)
        {
            printf("Median: %u\n", curr_med);
			printf("Max latency: %u\n", latencies[count - 1]);
			printf("Min latency: %u\n", latencies[0]);
            free(memory_address);
            return curr_med;
        }

        prev_prev_med = prev_med;
        prev_med = curr_med;
    }

    free(memory_address);
    return -1;
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
