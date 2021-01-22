#include "util.h"

ADDR_PTR addr_send;

void init_covert_channel()
{
    addr_send = (ADDR_PTR)strtok;
    sender_wait_for_notification();
}

inline void send_bit_over_covert_channel(bit val)
{
    //printf("s10");
    for(int i = 0; i <ITERATIONS; i++)
    {
        if(val == 1)
        {
            printf("send bit 1\n");
            clflush(addr_send);
        }
        else if (val == 0)
        {
            printf("send bit 0\n");
            maccess(addr_send);
        }

        notify_receiver();
        //printf("s13");
        sender_wait_for_notification();
        //printf("s14");
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
    int val;

    //printf("s1");
    init_covert_channel();
    //printf("sx1");

    do {
        val = getchar();
        printf("\ngot val: %d\n", val);
        //printf("s2");
	    send_byte_over_covert_channel(val);
        //printf("s3");
    } while(val != EOF);
    //printf("s4");
    notify_receiver();
    //printf("s5");
    return 0;
}
