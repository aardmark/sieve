#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define BITS 64

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
   unsigned int size;
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

void print_results(Sieve *sieve, unsigned int passes, double elapsed_time, int show_results) {
    if (show_results) printf("2, ");
    unsigned int count = (sieve->size >= 2);
    unsigned int half_size = sieve->size >> 1;

    for(unsigned int x = 1; x < half_size; x++) {
        if (!(sieve->bits[x>>SHIFT] & ((TYPE)1<<(x&MASK)))) {
            count++;
            if (show_results) printf("%d, ", (x<<1)+1);
        }
    }

    if (show_results) printf("\n");

    if (count != 78498) {
        printf("INVALID number_of_primes: %d\n", count);
    }
    printf("%d passes in %f seconds.\n", passes, elapsed_time);
}

void calculate_primes(Sieve *sieve) {
    unsigned int max_factor = usqrt(sieve->size);
    unsigned int half_size = sieve->size >> 1;
    unsigned int factorh = 1U;
    unsigned int qh = max_factor>>1U;
    while (factorh <= qh)
    {
        unsigned int factor, x;
        // get next prime
        for(x = factorh; x < half_size; x++)
            if (!(sieve->bits[x>>SHIFT] & ((TYPE)1<<(x & MASK)))) break;

        factor = (x<<1U)+1U;
        factorh=x+1;
        // clear multiples
        x = (factor * factor)>>1U;
        for(; x < half_size; x+=factor)
            sieve->bits[x >> SHIFT] |= (TYPE)1 << (x & MASK);
    }
}

int main()
{
    struct timespec t,t2;
    unsigned int passes = 0;
    unsigned int max = 1000000;
    unsigned int sieve_size = (max / BITS) / 2; // 15625

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

// for i in {1..10}; do ./sieve_odds; done

/*
64bit
11415 passes in 5.000351 seconds.
11403 passes in 5.000010 seconds.
11360 passes in 5.000165 seconds.
11365 passes in 5.000328 seconds.
11416 passes in 5.000027 seconds.
11416 passes in 5.000351 seconds.
11342 passes in 5.000190 seconds.
11088 passes in 5.000357 seconds.
11414 passes in 5.000292 seconds.
10744 passes in 5.000172 seconds.

32bit
12852 passes in 5.000299 seconds.
12762 passes in 5.000071 seconds.
12763 passes in 5.000114 seconds.
12797 passes in 5.000291 seconds.
12817 passes in 5.000233 seconds.
12838 passes in 5.000266 seconds.
12833 passes in 5.000348 seconds.
12752 passes in 5.000022 seconds.
12755 passes in 5.000215 seconds.
12750 passes in 5.000128 seconds.

*/
