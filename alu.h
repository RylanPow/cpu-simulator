#ifndef ALU_H
#define ALU_H

#include <stdint.h>

typedef enum {
    ALU_AND     = 3'b000;
    ALU_OR      = 3'b001;
    ALU_ADD     = 3'b010;
    ALU_ADD_NOT = 3'b100; //REMEMBER: no funciton code for 3 in MIPS!
    ALU_OR_NOT  = 3'b101;
    ALU_SUB     = 3'b110;
    ALU_SLT     = 3'b111;
} ALUOp;

uint32_t alu_execute(uint32_t a, uint32_t b, ALUOp f, uint32_t *zero);

#endif