.PHONY: clean constructor mutators algebra

CC=g++
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c++17 -g
TESTFLAGS=-lgtest -lgtest_main -lpthread

all: constructor mutators algebra

constructor:
	$(CC) $(CFLAGS) -o constructors constructors.cc $(TESTFLAGS)
	./constructors

mutators:
	$(CC) $(CFLAGS) -o mutators mutators.cc $(TESTFLAGS)
	./mutators

algebra:
	$(CC) $(CFLAGS) -o algebra algebra.cc $(TESTFLAGS)
	./algebra

clean:
	rm -f constructors mutators algebra

