// KEY SPECIFICATIONS:
// instruction memory: read only, should start at 0x00400000, word aligned
// data memory: goes both ways, starts at 0x10010000, byte addressable

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    // instruction memory
    uint32_t text_base; //starts at 0x00400000
    uint32_t *text_mem; 
    size_t text_size;

    // data memory
    uint32_t data_base; //starts at 0x10010000
    uint8_t *data_mem;
    size_t data_size;
} Memory;

void mem_init(Memory *mem, size_t text_size, size_t data_size);

void mem_free(Memory *mem);

void mem_load_program(Memory *mem, const uint8_t *program, size_t program_bytes);

uint32_t mem_fetch_instruction(const Memory *mem, uint32_t pc);

// read and write bytes, half-words, and word
uint32_t mem_read_word(const Memory *mem, uint32_t addr);
void mem_write_word(Memory *mem, uint32_t addr, uint32_t value);

// explicit byte and half-word access for convenience
uint16_t mem_read_half(const Memory *mem, uint32_t addr);
uint8_t mem_read_byte(const Memory *mem, uint32_t addr);


#endif