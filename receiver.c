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
	printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr15\n");
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

	//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr8\n");
	for(int i = 0; i < BYTE_SIZE ; i++)
	{
		//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr9\n");
		b = receive_bit_over_covert_channel();
		//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr10\n");
		bits[i] = b;
		// If last bit, finish to write down the full byte before notifying
		if(i < BYTE_SIZE - 1)
        {
			//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr11\n");
            notify_sender();
			//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr12\n");
            receiver_wait_for_notification();
			//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr13\n");
        }

		//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr14\n");
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
		//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr4\n");
        receiver_wait_for_notification();
		//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr5\n");
		val = receive_val_over_covert_channel();
		//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr6\n");
		if (val != EOF) {
			putchar(val);
			printf("\n");
		}
		// Finish the whole byte, now ask for a new byte/finish
		//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr7\n");
        notify_sender();
	} while (val != EOF);
	//printf("\n");
}

int main(int argc, char **argv)
{
	//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr1\n");
	init_covert_channel();
	//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr2\n");
	receive_over_covert_channel();
	//printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr3\n");

	return 0;
}

