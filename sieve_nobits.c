#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define SIZE 1000000

// 4189 iterations in 5.000000 seconds - using ints

static inline void __attribute__((always_inline)) clear_multiples(uint8_t* sieve, int factor) {
    for(int ix = factor * factor; ix < SIZE; ix+=factor * 2) {
        sieve[ix] = 0;
    }
}

static inline int __attribute__((always_inline)) get_next_prime(uint8_t* sieve, int factor) {
    for(int ix = factor + 2; ix < SIZE; ix+=2) {
        if (sieve[ix]) {
            return ix;
        }
    }
    return 0;
}

static inline int __attribute__((always_inline)) get_primes(int* primes) {
    uint8_t sieve[SIZE];
    
    memset(sieve, 1, sizeof(sieve));

    int factor = 1;
    while (1) {
        factor = get_next_prime(sieve, factor);
        if (factor >= 1000) break;
        clear_multiples(sieve, factor);
    }

    int* prime_ptr = primes;
    *prime_ptr++ = 2;
    int prime_count = 1;

    for(int ix = 3; ix < SIZE; ix+=2)
    {
        if (sieve[ix]) {
            *prime_ptr++ = ix;
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

    printf("%d iterations in %f seconds\n", iterations, duration);

    return 0;
}
