#include "util.h"

ADDR_PTR address;

void init_covert_channel()
{
    address = (ADDR_PTR)strcpy;
    sender_wait_for_notification();
}

inline void send_bit_over_covert_channel(bit val)
{
    for(int i = 0; i < ITERATIONS_PER_BIT; i++)
    {
        if(val == 1)
        {
            clflush(address);
        }

        notify_receiver();
        sender_wait_for_notification();
    }
}

void send_byte_over_covert_channel(int val)
{
    char* bits_array = (char*)malloc(BYTE * sizeof(char));
    byte_to_bits(val, bits_array);

    for(int i = 0; i < BYTE; i++)
    {
        send_bit_over_covert_channel(bits_array[i]);
    }

    free(bits_array);
}

int main(int argc, char **argv)
{
    int val;
    init_covert_channel();

    do 
    {
        val = getchar();
	    send_byte_over_covert_channel(val);
    } 
    while(val != EOF);

    notify_receiver();
    return 0;
}
