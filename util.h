#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/time.h>
#include <assert.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t
#define CYCLES uint32_t
#define implement_me (void)0
#define BYTE_SIZE 8
#define ITERATIONS 1
#define MAX_ITERATIONS 100000

typedef char bit;

#ifndef MISS_LATENCY
#define MISS_LATENCY 130
#endif

// Measure access time to the given address
CYCLES measure_access_time_to_addr(ADDR_PTR addr);
// Flushes address from all cache levels
void clflush(ADDR_PTR addr);

// Translatea byte to bits array
void byte_to_bits(char input, char* bits);
// Translatea bits array to byte
void bits_to_byte(char* bits, char* output);


// Wakes up process waiting for receiver notification
int notify_receiver();
// Wakes up process waiting for sender notification
int notify_sender();

// Blocks until notify_sender is called
int sender_wait_for_notification();
// Blocks until notify_receiver is called
int receiver_wait_for_notification();

void array_sort(CYCLES *array , int n);
CYCLES find_median(CYCLES array[] , int n);
void maccess(ADDR_PTR p);
uint64_t rdtsc();

#endif
