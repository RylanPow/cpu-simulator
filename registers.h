#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

typedef struct {
    uint32_t regs[32]; // registers 0-31
} RegisterFile;

void reg_init(RegisterFile *rf);                                // init all registers to 0
uint32_t reg_read(RegisterFile *rf, uint8_t reg);               // read from reg
void reg_write(RegisterFile *rf, uint8_t reg, uint32_t value);  // write to reg

#endif
