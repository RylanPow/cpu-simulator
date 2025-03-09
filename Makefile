CC = gcc
CFLAGS = -Wall -Wextra -g

all: test

# Assemble MIPS .asm -> .bin
%.bin: %.asm
	python3 assemble.py $< $@

# Build the test executable
test: registers.o test.o alu.o memory.o loader.o control.o cpu.o
	$(CC) $(CFLAGS) $^ -o test

# Object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the test program with program.bin
run: test program.bin
	./test

clean:
	rm -f *.o test program.bin

rebuild: clean all