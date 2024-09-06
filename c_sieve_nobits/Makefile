CFLAGS = -Wall -Wextra -Ofast -march=native -mtune=native -funroll-all-loops

sieve_nobits: sieve_nobits.o
	cc -o sieve_nobits sieve_nobits.o

.PHONY: clean

clean:
	rm -f *.o sieve_nobits
