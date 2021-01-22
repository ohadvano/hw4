#include "util.h"

ADDR_PTR shared_covert_address;

void init_covert_channel()
{
	shared_covert_address = (ADDR_PTR)strcpy;
	notify_sender();
}

inline bit receive_bit_over_covert_channel()
{
	int successful_transmissions = 0;
	uint64_t start, end, sum = 0;
	for(int i = 0; i < ITERATIONS_PER_BIT; i++)
	{
		start = rdtsc_with_fence();
		access_by_address(shared_covert_address);
		end = rdtsc_with_fence();

		uint64_t diff = end - start;
        if(diff < UPPER_BOUND && diff > LOWER_BOUND)
		{
			successful_transmissions++;
            sum += diff;
		}

		if (i < ITERATIONS_PER_BIT - 1)
        {
            notify_sender();
            receiver_wait_for_notification();
        }
	}

    if((sum / successful_transmissions) <= MISS_LATENCY)
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
	int byte_as_int = 0;

	do 
	{
        receiver_wait_for_notification();

		byte_as_int = receive_val_over_covert_channel();
		if (byte_as_int != EOF)
		{
			putchar(byte_as_int);
		}

        notify_sender();
	} 
	while (byte_as_int != EOF);
}

int main(int argc, char **argv)
{
	init_covert_channel();
	receive_over_covert_channel();
	return 0;
}

