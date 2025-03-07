CC = gcc
CFLAGS = -Wall -Wextra -g

all: test

test: registers.o test.o alu.o memory.o loader.o
	$(CC) $(CFLAGS) registers.o test.o alu.o memory.o loader.o -o test

registers.o: registers.c registers.h
	$(CC) $(CFLAGS) -c registers.c -o registers.o

memory.o: memory.c memory.h
	$(CC) $(CFLAGS) -c memory.c -o memory.o

loader.o: loader.c loader.h memory.h
	$(CC) $(CFLAGS) -c loader.c -o loader.o

alu.o: alu.c alu.h
	$(CC) $(CFLAGS) -c alu.c -o alu.o

test.o: test.c registers.h alu.h memory.h loader.h
	$(CC) $(CFLAGS) -c test.c -o test.o


run: test
	./test

clean:
	rm -f registers.o test.o alu.o memory.o loader.o test

rebuild: clean all