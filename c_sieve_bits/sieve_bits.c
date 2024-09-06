#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define BITS 32

#if BITS == 32
typedef uint32_t sieve_ptr_t;
#define FILL (4294967295U)
#define TYPE uint32_t
#define MASK 0x1FU
#define SHIFT 5U
#elif BITS == 64
typedef uint64_t sieve_ptr_t;
#define FILL (__UINT64_C(18446744073709551615))
#define TYPE uint64_t
#define MASK 0x3FUL
#define SHIFT 6UL
#else
#error Invalid BITS value.
#endif

struct sieve_t {
   int size;
   TYPE* bits;
};

typedef struct sieve_t Sieve;

unsigned int usqrt(int n)
{
    unsigned int x;
    unsigned int xLast;

    xLast = 0;
    x = n / 2;

    while (x != xLast) {
        xLast = x;
        x = (x + n / x) / 2;
    }
    return x;
}

void print_results(Sieve *sieve, int passes, double elapsed_time, int show_results) {
    if (show_results) printf("2, ");
    int count = (sieve->size >= 2);

    for(int x = 3; x < sieve->size; x+=2) {
        if (!(sieve->bits[x>>SHIFT] & ((TYPE)1<<(x&MASK)))) {
            count++;
            if (show_results) printf("%d, ", x);
        }
    }

    if (show_results) printf("\n");

    if (count != 78498) {
        printf("INVALID number_of_primes: %d\n", count);
    }
    printf("%d passes in %f seconds.\n", passes, elapsed_time);
}

void calculate_primes(Sieve *sieve) {
    int max_factor = usqrt(sieve->size);
    int factor = 3;
    while (factor <= max_factor)
    {
        // get next prime
        for(int x = factor; x < sieve->size; x+=2) {
            if (!(sieve->bits[x>>SHIFT] & ((TYPE)1<<(x & MASK)))) {
                factor = x;
                break;
            }
        }
        // clear multiples
        for(int y = factor * factor; y < sieve->size; y+=factor * 2)
            sieve->bits[y >> SHIFT] |= (TYPE)1 << (y & MASK);
        
        factor += 2;
    }
}

int main()
{
    struct timespec t,t2;
    int passes = 0;
    int max = 1000000;
    int sieve_size = max / BITS;

    clock_gettime(CLOCK_MONOTONIC,&t);

    while (1)
    {
        Sieve sieve;
        sieve.size = max;
        sieve.bits = calloc(sieve_size, sizeof(TYPE));
        calculate_primes(&sieve);
        passes++;

        clock_gettime(CLOCK_MONOTONIC,&t2);
        double elapsed_time=t2.tv_sec+t2.tv_nsec*1e-9-t.tv_sec-t.tv_nsec*1e-9;
        if (elapsed_time >= 5.0) {
            print_results(&sieve, passes, elapsed_time, 0);
            free(sieve.bits);
            break;
        }
        free(sieve.bits);
    }

    return 0;
}

// 999931,999953,999959,999961,999979,999983

// for i in {1..10}; do ./sieve_bits; done

/*
64bit
10164 passes in 5.000450 seconds.
10172 passes in 5.000224 seconds.
10165 passes in 5.000307 seconds.
10066 passes in 5.000375 seconds.
10083 passes in 5.000408 seconds.
10163 passes in 5.000341 seconds.
10124 passes in 5.000091 seconds.
10148 passes in 5.000029 seconds.
9953 passes in 5.000100 seconds.
10118 passes in 5.000432 seconds.

32bit
10729 passes in 5.000224 seconds.
10780 passes in 5.000361 seconds.
10772 passes in 5.000128 seconds.
10776 passes in 5.000131 seconds.
10780 passes in 5.000413 seconds.
10764 passes in 5.000180 seconds.
10757 passes in 5.000076 seconds.
10778 passes in 5.000129 seconds.
10693 passes in 5.000343 seconds.
10757 passes in 5.000074 seconds.

*/
