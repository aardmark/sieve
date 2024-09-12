#!/bin/sh
CC="gcc -Wall -Wextra -Ofast -march=native -mtune=native -funroll-all-loops"
for x in sieve_nobits sieve_bits sieve_odds; do
    $CC -o $x $x.c -lm
done
