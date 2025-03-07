#include "memory.h"
#include <stdlib.h>
#include <string.h>

void mem_init(Memory *mem, size_t text_size, size_t data_size) {
    mem->text_base = 0x00400000;
    mem->text_mem = (uint32_t*)calloc(text_size, sizeof(uint32_t));
    mem->text_size = text_size;

    mem->data_mem = 0x100100000;
    mem->data_mem = (uint8_t*)calloc(data_size, sizeof(uint8_t));
    mem->data_size = data_size;
}

void mem_free(Memory *mem) {
    free(mem->text_mem);
    free(mem->data_mem);
}