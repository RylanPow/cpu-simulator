#include "loader.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_program(Memory *mem, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return 0;
    }
    
    // get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // check if empty or not word aligned
    if (file_size == 0 || file_size % 4 != 0) {
        fprintf(stderr, "Invalid program: size must be non-zero and 4-byte word-aligned\n");
        fclose(file);
        return 0;
    }

    size_t words_needed = file_size / 4;
    if (words_needed > mem->text_size) {
        fprintf(stderr, "Program too large: %zu words (max %zu)\n", words_needed, mem->text_size);
        fclose(file);
        return 0;
    }

    uint8_t *buffer = (uint8_t*)malloc(file_size);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return 0;
    }

    size_t bytes_read = fread(buffer, 1, file_size, file);
    fclose(file);

    if (bytes_read != (size_t)file_size) {
        fprintf(stderr, "Failed to read entire file\n");
        free(buffer);
        return 0;
    }

    mem_load_program(mem, buffer, file_size);
    
    free(buffer);
    return 1;
}