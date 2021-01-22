#include "util.h"

void init_covert_channel()
{
    implement_me;
}

inline void send_bit_over_covert_channel(bit val)
{
    implement_me;
}

void send_byte_over_covert_channel(int val)
{
    implement_me;
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
