#ifndef ALU_H
#define ALU_H

#include <stdint.h>

typedef enum {
    ALU_AND     = 0,
    ALU_OR      = 1,
    ALU_ADD     = 2,
    ALU_AND_NOT = 4, //REMEMBER: no funciton code for 3 in MIPS!
    ALU_OR_NOT  = 5,
    ALU_SUB     = 6,
    ALU_SLT     = 7,
} ALUOp;

uint32_t alu_execute(uint32_t a, uint32_t b, ALUOp f, uint32_t *zero);

#endif