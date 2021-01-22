The skeleton files contains three executables: sender, receiver, and basic.
Feel free to change the the code as you wish - the given code is just a starting point.
However, make sure not to change method signatures as they appear in the exercise description and you should use the MISS_LATENCY definition (util.h) for the covert channel implementation.

Finally, your code for the receiver executable should print the transferred file to stdout.

Executables:
--------
basic: Used to implement measurments of access latency to data either in cache or not.
sender, receiever - the covert channel executables. Example usage and expected result:
./sender < demo.txt &
./receiver
Hello world!

Note, Both the sender and receiver should terminate after the file transmission is complete and free all dynamically allocated memory.
