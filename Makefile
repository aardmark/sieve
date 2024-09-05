CFLAGS = -Wall -Wextra -Ofast -march=native -mtune=native -funroll-all-loops

all: sieve sieve_nobits sieve_stolen sieve_odds original

sieve: sieve.o
	cc -o sieve sieve.o

sieve_odds: sieve_odds.o
	cc -o sieve_odds sieve_odds.o

sieve_nobits: sieve_nobits.o
	cc -o sieve_nobits sieve_nobits.o

sieve_stolen: sieve_stolen.o
	cc -o sieve_stolen sieve_stolen.o

original: original.cpp
	clang++ -Ofast -std=c++17 original.cpp -ooriginal

.PHONY: clean

clean:
	rm -f *.o sieve sieve_nobits sieve_odds sieve_stolen original
