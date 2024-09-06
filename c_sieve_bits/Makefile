CFLAGS = -Wall -Wextra -Ofast -march=native -mtune=native -funroll-all-loops

sieve_bits: sieve_bits.o
	cc -o sieve_bits sieve_bits.o

.PHONY: clean

clean:
	rm -f *.o sieve_bits
