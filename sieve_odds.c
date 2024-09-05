#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define SIZE 1000000
#define HALF_SIZE 1000000 / 2
#define BITS 32
#define SIEVE_SIZE SIZE / BITS / 2

#if BITS == 32
typedef __uint32_t sieve_ptr_t;
#define FILL (4294967295U)
#define TYPE uint32_t
#define MASK 0x1FU
#define SHIFT 5U
#elif BITS == 64
typedef __uint64_t sieve_ptr_t;
#define FILL (__UINT64_C(18446744073709551615))
#define TYPE uint64_t
#define MASK 0x3FUL
#define SHIFT 6UL
#else
#error Invalid BITS value.
#endif

static inline void __attribute__((always_inline)) clear_multiples(sieve_ptr_t* sieve, int factor) {
    for(int ix = (factor * factor)>>1U; ix < HALF_SIZE; ix+=factor) {
        sieve[ix >> SHIFT] &= ~((TYPE)1 << (ix & MASK));
    }
}

static inline int __attribute__((always_inline)) get_next_prime(sieve_ptr_t* sieve, int factor) {
    for(int ix = factor; ix < HALF_SIZE; ix++) {
        if (sieve[ix>>SHIFT] & ((TYPE)1<<(ix&MASK))) {
            return (ix<<1U)+1U;
        } 
    }
    return 0;
}

static inline int __attribute__((always_inline)) get_primes(int* primes) {
    sieve_ptr_t sieve[SIEVE_SIZE];
    for(int ix=0; ix<SIEVE_SIZE; ix++) {
        sieve[ix] = FILL;
    }

    unsigned int factor = 1;
    unsigned int factorh = 1;
    while (factorh <= 500) {
        factor = get_next_prime(sieve, factorh++);
        clear_multiples(sieve, factor);
    }

    int* prime_ptr = primes;
    *prime_ptr++ = 2;
    int prime_count = 1;
    for(int ix = 1; ix < HALF_SIZE; ix++)
    {
        if (sieve[ix>>SHIFT] & ((TYPE)1<<(ix&MASK))) {
            *prime_ptr++ = (ix<<1)+1;
            prime_count++;
        }
    }

    return prime_count;
}

int main()
{
    clock_t start = clock();
    int iterations = 0;
    double duration = 0.0;
    int primes[80000];
    int prime_count;

    while (1)
    {
        prime_count = get_primes(primes);
        iterations++;
        if (prime_count != 78498) {
            printf("INVALID number_of_primes: %d\n", prime_count);
            break;
        }
        clock_t end = clock();
        duration = (double)((end - start) / CLOCKS_PER_SEC);
        if (duration >= 5.0) break;
    }

    for(int ix=0; ix<6; ix++) {
        printf("%d\n", primes[ix]);
    }
    printf("%d\n", primes[prime_count-6]);
    printf("%d\n", primes[prime_count-5]);
    printf("%d\n", primes[prime_count-4]);
    printf("%d\n", primes[prime_count-3]);
    printf("%d\n", primes[prime_count-2]);
    printf("%d\n", primes[prime_count-1]);

    printf("%d iterations in %f seconds\n", iterations, duration);

    return 0;
}

// 999931,999953,999959,999961,999979,999983

// for i in {1..10}; do ./sieve; done

/*
64bit
3857 iterations in 5.000000 seconds
3859 iterations in 5.000000 seconds
3862 iterations in 5.000000 seconds
3863 iterations in 5.000000 seconds
3865 iterations in 5.000000 seconds
3859 iterations in 5.000000 seconds
3861 iterations in 5.000000 seconds
3862 iterations in 5.000000 seconds
3864 iterations in 5.000000 seconds
3859 iterations in 5.000000 seconds

32bit
3964 iterations in 5.000000 seconds
3973 iterations in 5.000000 seconds
3973 iterations in 5.000000 seconds
3968 iterations in 5.000000 seconds
3971 iterations in 5.000000 seconds
3970 iterations in 5.000000 seconds
3965 iterations in 5.000000 seconds
3969 iterations in 5.000000 seconds
3961 iterations in 5.000000 seconds
3976 iterations in 5.000000 seconds

*/

// 0x1F = 0101 1111
// 0x3F = 0011 1111
//
// 00000
// 00001
// 00010
// 00011
// 00100
// 00101
// 00110
// 00111

// 01000
// 01001
// 01010
// 01011
// 01100
// 01101
// 01110
// 01111

// 10000
