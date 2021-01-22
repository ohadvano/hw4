#include "util.h"

void init_covert_channel()
{
	implement_me;
}

inline bit receive_bit_over_covert_channel()
{
	implement_me;
}

int receive_val_over_covert_channel()
{
	implement_me;
}

void receive_over_covert_channel()
{
	int val;
       	do {
		// Read the file over the covert-channel bit-by-bit.
		// 
		val = receive_val_over_covert_channel();
		if (val != EOF) {
			putchar(val);
		}		
	} while (val != EOF);
}

int main(int argc, char **argv)
{
	init_covert_channel();
	receive_over_covert_channel();

	return 0;
}

