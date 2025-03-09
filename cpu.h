#ifndef CPU_H
#define CPU_H

#include "registers.h"
#include "alu.h"
#include "memory.h"
#include "control.h"

typedef struct {
    RegisterFile regs;
    Memory mem;
    uint32_t pc;
    ControlSignals ctrl;
} CPU;

void cpu_init(CPU *cpu, size_t text_size, size_t data_size);
void cpu_free(CPU *cpu);
void cpu_step(CPU *cpu);
void cpu_run(CPU *cpu);

#endif
