#ifndef CONTROL_H
#define CONTROL_H

#include <stdint.h>
#include "alu.h"

// note: these are all flags
typedef struct {
    uint8_t RegWrite; // write to register file?
    uint8_t ALUSrc;   // ALU source - 0: register, 1: immediate
    uint8_t MemtoReg; // memroy to register file?
    uint8_t MemRead;  // read from memory?
    uint8_t MemWrite; // write to mem?
    uint8_t Branch;   // branch taken?
    uint8_t Jump;     // jump taken?
    ALUOp alu_op;     // ALU operation from opcode
} ControlSignals;

ControlSignals control_decode(uint32_t instruction);

#endif