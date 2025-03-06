CC = gcc
CFLAGS = -Wall -Wextra -g

all: test

test: registers.o test.o alu.o
	$(CC) $(CFLAGS) registers.o test.o alu.o -o test

registers.o: registers.c registers.h
	$(CC) $(CFLAGS) -c registers.c -o registers.o

alu.o: alu.c alu.h
	$(CC) $(CFLAGS) -c alu.c -o alu.o

test.o: test.c registers.h alu.h
	$(CC) $(CFLAGS) -c test.c -o test.o

run: test
	./test

clean:
	rm -f registers.o test.o alu.o test

rebuild: clean all