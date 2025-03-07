#include "memory.h"
#include <stdlib.h>
#include <string.h>

void mem_init(Memory *mem, size_t text_size, size_t data_size) {
    mem->text_base = 0x00400000;
    mem->text_mem = (uint32_t*)calloc(text_size, sizeof(uint32_t));
    mem->text_size = text_size;

    mem->data_base = 0x10010000;
    mem->data_mem = (uint8_t*)calloc(data_size, sizeof(uint8_t));
    mem->data_size = data_size;
}

void mem_free(Memory *mem) {
    free(mem->text_mem);
    free(mem->data_mem);
}

// load program into text memory (big endian)
void mem_load_program(Memory *mem, const uint8_t *program, size_t program_bytes) {
    size_t words_needed = program_bytes / 4;
    if (words_needed > mem->text_size) return;

    // copy bytes into text memory (convert to words)
    for (size_t i = 0; i < words_needed; i++) {
        // remember: big endian so MSB at lowest addr
        uint32_t word = (program[i * 4] << 24) |
                        (program[i * 4 + 1] << 16) |
                        (program[i * 4 + 2] << 8) |
                        program[i * 4 + 3];
        mem->text_mem[i] = word;
    }
}


uint32_t mem_fetch_instruction(const Memory *mem, uint32_t pc) {
    if (pc < mem->text_base || pc >= mem->text_base + mem->text_size * 4)
        return 0;
    uint32_t index = (pc - mem->text_base) / 4;
    return mem->text_mem[index];
    
}

uint32_t mem_read_word(const Memory *mem, uint32_t addr) {
    if (addr < mem->data_base || addr + 3 >= mem->data_base + mem->data_size)
        return 0; // addr + 3 because 0, 1, 2, 3 are starts of byte sections and 4 bytes = 32 bits

    int index = addr - mem->data_base;
    //remember: big endian order
    //shift each byte section
    return (mem->data_mem[index] << 24) |
           (mem->data_mem[index + 1] << 16) |
           (mem->data_mem[index + 2] << 8) |
           mem->data_mem[index + 3];        
}

void mem_write_word(Memory *mem, uint32_t addr, uint32_t value) {
    if (addr < mem->data_base || addr + 3 >= mem->data_base + mem->data_size)
        return;

    uint32_t index = addr - mem->data_base;
    mem->data_mem[index] = (value >> 24) & 0xFF;
    mem->data_mem[index + 1] = (value >> 16) & 0xFF;
    mem->data_mem[index + 2] = (value >> 8) & 0xFF;
    mem->data_mem[index + 3] = value & 0xFF;
}