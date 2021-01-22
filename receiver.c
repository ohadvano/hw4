#include "util.h"

ADDR_PTR addr_rec;

void init_covert_channel()
{
	addr_rec = (ADDR_PTR)strtok;
	notify_sender();
}

inline bit receive_bit_over_covert_channel()
{
	uint64_t start, end;
	uint64_t sum = 0;
	int count = ITERATIONS;
    // Check each bit ITERATION times before determining if 0 or 1
	for(int i = 0; i <ITERATIONS; i++)
	{
		start = rdtsc();
		maccess(addr_rec);
		end = rdtsc();

        if((end - start) > 500 || (end - start) < 15) //edge cases - filter out
        {
            count--;
        }
        else
        {
            sum += end - start;
        }

        // If last ITERATION for this bit, finish to write down the final 0 or 1 value before notifying
		if (i < ITERATIONS - 1)
        {
            notify_sender();
            receiver_wait_for_notification();
        }
	}

    if((sum/count) <= MISS_LATENCY)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int receive_val_over_covert_channel()
{
	bit b;
	char ret_val;
	char* bits = (bit*)calloc(8, sizeof(char));

	for(int i = 0; i < BYTE_SIZE ; i++)
	{
		b = receive_bit_over_covert_channel();
		bits[i] = b;
		// If last bit, finish to write down the full byte before notifying
		if(i < BYTE_SIZE - 1)
        {
            notify_sender();
            receiver_wait_for_notification();
        }
	}

	bits_to_byte(bits, &ret_val);
	free(bits);

	return ret_val;
}

void receive_over_covert_channel()
{
	int val;

	do {
	    // Wait for new data from sender
        receiver_wait_for_notification();
		val = receive_val_over_covert_channel();
		if (val != EOF) {
			putchar(val);
		}
		// Finish the whole byte, now ask for a new byte/finish
        notify_sender();
	} while (val != EOF);
	printf("\n");
}

int main(int argc, char **argv)
{
	init_covert_channel();
	receive_over_covert_channel();

	return 0;
}

