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

	//printf("r8");
	for(int i = 0; i < BYTE_SIZE ; i++)
	{
		//printf("r9");
		b = receive_bit_over_covert_channel();
		//printf("r10");
		bits[i] = b;
		// If last bit, finish to write down the full byte before notifying
		if(i < BYTE_SIZE - 1)
        {
			//printf("r11");
            notify_sender();
			//printf("r12");
            receiver_wait_for_notification();
			//printf("r13");
        }

		//printf("r14");
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
		//printf("r4");
        receiver_wait_for_notification();
		//printf("r5");
		val = receive_val_over_covert_channel();
		//printf("r6");
		if (val != EOF) {
			putchar(val);
		}
		// Finish the whole byte, now ask for a new byte/finish
		//printf("r7");
        notify_sender();
	} while (val != EOF);
	printf("\n");
}

int main(int argc, char **argv)
{
	//printf("r1");
	init_covert_channel();
	//printf("r2");
	receive_over_covert_channel();
	//printf("r3");

	return 0;
}

