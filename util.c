#include "util.h"
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/* Measure the time it takes to access a block with virtual address addr. */
CYCLES measure_access_time_to_addr(ADDR_PTR addr)
{
    CYCLES cycles;

    asm volatile(
        "mov %1, %%r8\n\t"
        "lfence\n\t"
        "rdtsc\n\t"             /* returns the time (t1) in eax */
        "mov %%eax, %%edi\n\t"  /* moves eax (t1) into edi */
        "mov (%%r8), %%r8\n\t"  /* accesses addr */
        "lfence\n\t"
        "rdtsc\n\t"             /* returns the time (t2) in eax */
        "sub %%edi, %%eax\n\t"  /* eax = eax (t2) - edi (t1) */
    : "=a"(cycles) /*output*/
    : "r"(addr)
    : "r8", "edi");

    return cycles;
}

/* Flushes the given address from all cache levels. */
void clflush(ADDR_PTR addr)
{
    asm volatile ("clflush (%0)"::"r"(addr));
}

/* Extracts 8 bits from a byte  */
void byte_to_bits(char input, char* bits)
{
	for(int i = 0; i < BYTE; i++) 
	{
		bits[i] = ((input >> i) & 0x01);
	}
}

/* Extract byte to bit */
void bits_to_byte(char* bits, char* output)
{
	char res = 0;
	for(int i = 0; i < BYTE; i++) 
	{
		res += bits[i] << i;
	}
	
	*output = res;
}

static char volatile* sender_lock = NULL;
static char volatile* receiver_lock = NULL;

static int create_shared_lock()
{
    if (sender_lock != NULL || receiver_lock != NULL)
    {
	return 0;
    }

    int res;
    int fd;

    fd = shm_open("/SENDER_RECEIVER_SHARED_LOCK", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
	    perror("Failed to init sync var's memory");
	    return -1;
    }

    // Extend memory object as by default it's initialized with size 0
    res = ftruncate(fd, 32);    
    if (res == -1)
    {
	    perror("Failed to init sync var's memory");
	    return -1;
    }

    sender_lock = (char volatile*) mmap(NULL, 132, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (sender_lock == MAP_FAILED)
    {
	    perror("Failed to init sync var's memory");
	    return -1;
    }

    receiver_lock = sender_lock;
    receiver_lock++;

    *sender_lock = 1;
    *receiver_lock = 1;

    return 0;
}

int notify_receiver()
{
   assert (create_shared_lock() == 0);
   __asm__ volatile ("");
    *receiver_lock = 0;
}

int notify_sender()
{
   assert (create_shared_lock() == 0);
   __asm__ volatile ("");
    *sender_lock = 0;
}

int sender_wait_for_notification()
{
   assert (create_shared_lock() == 0);
    while(!__sync_bool_compare_and_swap(sender_lock, 0, 1))
    {
        while(*sender_lock) __asm__("pause");
    }
}

int receiver_wait_for_notification()
{
   assert (create_shared_lock() == 0);
    while(!__sync_bool_compare_and_swap(receiver_lock, 0, 1))
    {
        while(*receiver_lock) __asm__("pause");
    }
}

void swap(int *p, int *q) 
{
    int t = *p; 
    *p = *q; 
    *q = t;
}

void sort(CYCLES *a, int n) 
{ 
    int i , j, temp;
    for(i = 0; i < n - 1; i++) 
    {
        for(j = 0; j < n - i - 1; j++) 
        {
            if(a[j] > a[j + 1])
            swap(&a[j], &a[j + 1]);
        }
    }
}

void array_sort(CYCLES *array , int n)
{ 
    int i, j, temp=0;
    for(i=0 ; i<n ; i++)
    {
        for(j=0 ; j<n-1 ; j++)
        {
            if(array[j]>array[j+1])
            {
                temp        = array[j];
                array[j]    = array[j+1];
                array[j+1]  = temp;
            }
        }
    }
}

CYCLES find_median(CYCLES array[] , int n)
{
    CYCLES median=0;
    if(n%2 == 0)
        median = (array[(n-1)/2] + array[n/2])/2.0;
    else
        median = array[n/2];

    return median;
}

void access_by_address(ADDR_PTR addr)
{
    asm volatile ("movq (%0), %%rax\n"
    :
    : "c" (addr)
    : "rax");
}

uint64_t rdtsc_with_fence() 
{
    uint64_t a, d;
    asm volatile ("mfence");
    asm volatile ("rdtsc" : "=a" (a), "=d" (d));
    a = (d << 32) | a;
    asm volatile ("mfence");
    return a;
}
