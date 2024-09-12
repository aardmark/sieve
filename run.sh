#!/bin/sh
for x in sieve_nobits sieve_bits sieve_odds; do
    echo $x
    ./$x
done
