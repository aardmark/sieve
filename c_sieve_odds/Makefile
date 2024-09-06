CFLAGS = -Wall -Wextra -Ofast -march=native -mtune=native -funroll-all-loops

sieve_odds: sieve_odds.o
	cc -o sieve_odds sieve_odds.o

.PHONY: clean

clean:
	rm -f *.o sieve_odds
