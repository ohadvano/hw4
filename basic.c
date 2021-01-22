#include "basic.h"
#include <stdio.h>

CYCLES main_memory_access_latency()
{
    int* tmp_for_address = (int*) malloc(sizeof(int));
	int elem_num = 0;
	CYCLES curr_med = 0, prev_med = 0, prev_prev_med = 0;
	CYCLES latencies[MAX_ITERATIONS] = {0}; 

    for (int i = 0; i < MAX_ITERATIONS; i++)
    {
        clflush((ADDR_PTR)tmp_for_address);
        latencies[i] = measure_access_time_to_addr((ADDR_PTR)tmp_for_address);
        elem_num++;
        if(elem_num == 1)
            continue;
        sort(latencies, elem_num);
        curr_med = find_median(latencies, elem_num);

        if(curr_med == prev_med && prev_med == prev_prev_med)
        {
            printf("med = %u, max = %u, min = %u \n", curr_med, latencies[elem_num-1], latencies[0]);
            free(tmp_for_address);
            return curr_med;
        }
		
        prev_prev_med = prev_med;
        prev_med = curr_med;
    }

    printf("wrong values: med = %u, max = %u, min = %u \n", curr_med, latencies[elem_num-1], latencies[0]);
    free(tmp_for_address);

    return curr_med; // won't get here
}

CYCLES last_level_cache_access_latency()
{
	int* tmp_for_address2 = (int*) malloc(sizeof(int));;
    int elem_num = 0;
    CYCLES curr_med = 0, prev_med = 0, prev_prev_med = 0;
    CYCLES latencies2[MAX_ITERATIONS] = {0}; 

    for (int i=0; i<MAX_ITERATIONS; i++)
    {
        maccess((ADDR_PTR)tmp_for_address2);
        latencies2[i] = measure_access_time_to_addr((ADDR_PTR)tmp_for_address2);
        elem_num++;
        if(elem_num == 1)
            continue;
        sort(latencies2, elem_num);
        curr_med = find_median(latencies2, elem_num);

        if(curr_med == prev_med && prev_med == prev_prev_med)
        {
            printf("med_cache = %u, max_cache = %u, min_cache = %u \n", curr_med, latencies2[elem_num-1], latencies2[0]);
            free(tmp_for_address2);
            return curr_med;
        }

        prev_prev_med = prev_med;
        prev_med = curr_med;
    }

    printf("wrong values: med_cache = %u, max_cache = %u, min_cache = %u \n", curr_med, latencies2[elem_num-1], latencies2[0]);
    free(tmp_for_address2);
    return curr_med; // won't get here
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
