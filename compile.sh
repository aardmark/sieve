#!/bin/sh
CC="gcc -Wall -Wextra -Ofast -march=native -mtune=native -funroll-all-loops"
for x in sieve_bytes sieve_bits sieve_odd_bits; do
    $CC -o $x $x.c -lm
done
