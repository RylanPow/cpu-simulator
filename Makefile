CC = gcc
CFLAGS = -Wall -Wextra -g

all: test

# assemble MIPS .asm -> .bin
%.bin: %.asm
	python3 assemble.py $< $@

# build the test executable
test: registers.o test.o alu.o memory.o loader.o control.o cpu.o
	$(CC) $(CFLAGS) $^ -o test

# object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# run the test program with program.bin
run: test program.bin
	./test

clean:
	rm -f *.o test program.bin

rebuild: clean all