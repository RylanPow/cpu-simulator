#include "registers.h"

void reg_init(RegisterFile *rf) {
    for (int i = 0; i < 32; i++) {
        rf->regs[i] = 0;
    }
}

uint32_t reg_read(RegisterFile *rf, uint8_t reg) {
    if (reg >= 32) return 0; //invalid register index
    return rf->regs[reg];
}

void reg_write(RegisterFile *rf, uint8_t reg, uint32_t value) {
    if (reg == 0) return; //$0 is READ ONLY
    if (reg >= 32) return;
    rf->regs[reg] = value;
}