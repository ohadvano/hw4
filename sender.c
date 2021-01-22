#include "util.h"

ADDR_PTR addr_send;

void init_covert_channel()
{
    addr_send = (ADDR_PTR)strtok;
    sender_wait_for_notification();
}

inline void send_bit_over_covert_channel(bit val)
{
    // Send each bit ITERATION times
    for(int i = 0; i <ITERATIONS; i++)
    {
        if(val == 1)
        {
            clflush(addr_send);
        }
        else if (val == 0)
        {
            maccess(addr_send);
        }

        notify_receiver();
        sender_wait_for_notification();
    }
}

void send_byte_over_covert_channel(int val)
{
    char* bits = (char*)calloc(BYTE_SIZE, sizeof(char));
    byte_to_bits(val, bits);

    for(int i = 0; i < BYTE_SIZE; i++)
    {
        send_bit_over_covert_channel(bits[i]);
    }

    free(bits);
}

int main(int argc, char **argv)
{
    init_covert_channel();
    int val;
    // Read the file from stdin, byte-by-byte
    //
    do { 
	 val = getchar();

	 send_byte_over_covert_channel(val);
    } while(val != EOF);
    
    return 0;
}
