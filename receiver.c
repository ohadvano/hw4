#include "util.h"

ADDR_PTR address;

void init_covert_channel()
{
	address = (ADDR_PTR)strcpy;
	notify_sender();
}

inline bit receive_bit_over_covert_channel()
{
	uint64_t start, end, sum = 0;
	int count = ITERATIONS_PER_BIT;
	for(int i = 0; i < ITERATIONS_PER_BIT; i++)
	{
		start = rdtsc_with_fence();
		maccess(address);
		end = rdtsc_with_fence();

		uint64_t diff = end - start;
        if(diff > UPPER_BOUND || diff < LOWER_BOUND)
            count--;
        else
            sum += diff;

		if (i < ITERATIONS_PER_BIT - 1)
        {
            notify_sender();
            receiver_wait_for_notification();
        }
	}

    if((sum / count) <= CACHE_MISS_LATENCY)
        return 0;

	return 1;
}

int receive_val_over_covert_channel()
{
	bit b;
	char byte_value;
	char* bits_array = (bit*)calloc(BYTE, sizeof(char));

	for(int i = 0; i < BYTE; i++)
	{
		bits_array[i] = receive_bit_over_covert_channel();

		if(i < BYTE - 1)
        {
            notify_sender();
            receiver_wait_for_notification();
        }
	}

	bits_to_byte(bits_array, &byte_value);
	free(bits_array);

	return byte_value;
}

void receive_over_covert_channel()
{
	int val;

	do 
	{
        receiver_wait_for_notification();

		val = receive_val_over_covert_channel();
		if (val != EOF)
		{
			putchar(val);
		}

        //notify_sender();
	} 
	while (val != EOF);
}

int main(int argc, char **argv)
{
	init_covert_channel();
	receive_over_covert_channel();
	return 0;
}

