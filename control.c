#include "control.h"
#include <stdio.h>

ControlSignals control_decode(uint32_t instruction) {
    ControlSignals ctrl = {0};
    uint8_t opcode = (instruction >> 26) & 0x3F; // opcode is bits 26-31

    switch (opcode) {
        // see John Hennessy's legendary architecture textbook for reference
        // it is his architecture after all!

        case 0x00: {
            ctrl.RegWrite = 1;
            ctrl.ALUSrc = 0;
            ctrl.MemtoReg = 0;
            ctrl.MemRead = 0;
            ctrl.MemWrite = 0;
            ctrl.Branch = 0;
            ctrl.Jump = 0;

            // determine ALU operation from funct field (bits 0-5)
            uint8_t funct = instruction & 0x3F;
            switch (funct) {
                case 0x20: ctrl.alu_op = ALU_ADD; break;  // add
                case 0x22: ctrl.alu_op = ALU_SUB; break;  // sub
                case 0x24: ctrl.alu_op = ALU_AND; break;  // and
                case 0x25: ctrl.alu_op = ALU_OR;  break;  // or
                case 0x2A: ctrl.alu_op = ALU_SLT; break;  // slt
                default:   ctrl.alu_op = ALU_ADD; break;  // default
            }
            break;
        }

        // lw 
        case 0x23: {
            ctrl.RegWrite = 1;
            ctrl.ALUSrc = 1;
            ctrl.MemtoReg = 1;
            ctrl.MemRead = 1;
            ctrl.MemWrite = 0;
            ctrl.Branch = 0;
            ctrl.Jump = 0;
            ctrl.alu_op = ALU_ADD;  // compute address (base + offset)
            break;
        }

        // sw 
        case 0x2B: {
            ctrl.RegWrite = 0;
            ctrl.ALUSrc = 1;
            ctrl.MemtoReg = 0;  // don't care
            ctrl.MemRead = 0;
            ctrl.MemWrite = 1;
            ctrl.Branch = 0;
            ctrl.Jump = 0;
            ctrl.alu_op = ALU_ADD;  // compute address (base + offset)
            break;
        }

        // beq 
        case 0x04: {
            ctrl.RegWrite = 0;
            ctrl.ALUSrc = 0;
            ctrl.MemtoReg = 0;  // don't care
            ctrl.MemRead = 0;
            ctrl.MemWrite = 0;
            ctrl.Branch = 1;
            ctrl.Jump = 0;
            ctrl.alu_op = ALU_SUB;  // compare registers (a - b)
            break;
        }

        // bne
        case 0x05: {
            ctrl.RegWrite = 0;
            ctrl.ALUSrc = 0;
            ctrl.MemtoReg = 0;  // don’t care
            ctrl.MemRead = 0;
            ctrl.MemWrite = 0;
            ctrl.Branch = 1;
            ctrl.Jump = 0;
            ctrl.alu_op = ALU_SUB; 
            break;
        }

        // addi
        case 0x08: {
            ctrl.RegWrite = 1;
            ctrl.ALUSrc = 1;
            ctrl.MemtoReg = 0;
            ctrl.MemRead = 0;
            ctrl.MemWrite = 0;
            ctrl.Branch = 0;
            ctrl.Jump = 0;
            ctrl.alu_op = ALU_ADD;  // add immediate to register
            break;
        }

        // j
        case 0x02: {
            ctrl.RegWrite = 0;
            ctrl.ALUSrc = 0;  // don’t care
            ctrl.MemtoReg = 0; // don’t care
            ctrl.MemRead = 0;
            ctrl.MemWrite = 0;
            ctrl.Branch = 0;
            ctrl.Jump = 1;
            ctrl.alu_op = ALU_ADD;  // don’t care
            break;
        }

        // default: Assume NOP (no operation)
        default: {
            ctrl.RegWrite = 0;
            ctrl.ALUSrc = 0;
            ctrl.MemtoReg = 0;
            ctrl.MemRead = 0;
            ctrl.MemWrite = 0;
            ctrl.Branch = 0;
            ctrl.Jump = 0;
            ctrl.alu_op = ALU_ADD;
            break;
        }
    }

    return ctrl;
}