#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

struct sieve_t {
   int size;
   uint8_t* bits;
};

typedef struct sieve_t Sieve;

// around 9100 passes in 5.000000 seconds

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
        if (!sieve->bits[x]) {
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
            if (!sieve->bits[x]) {
                factor = x;
                break;
            }
        }
        // clear multiples
        for(int y = factor * factor; y < sieve->size; y+=factor * 2)
            sieve->bits[y] = 1;
        
        factor += 2;
    }
}

int main()
{
    struct timespec t,t2;
    int passes = 0;
    int max = 1000000;
    
    clock_gettime(CLOCK_MONOTONIC,&t);

    while (1)
    {
        Sieve sieve;
        sieve.size = max;
        sieve.bits = calloc(max, sizeof(uint8_t));
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
