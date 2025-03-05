CC = gcc
CFLAGS = -Wall -Wextra -g

all: test

test: registers.o test.o
	$(CC) $(CFLAGS) registers.o test.o -o test

registers.o: registers.c registers.h
	$(CC) $(CFLAGS) -c registers.c -o registers.o

test.o: test.c registers.h
	$(CC) $(CFLAGS) -c test.c -o test.o

run: test
	./test

clean:
	rm -f registers.o test.o test

rebuild: clean all