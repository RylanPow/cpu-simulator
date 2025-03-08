#include "cpu.h"
#include <stdio.h>

void cpu_init(CPU *cpu, size_t text_size, size_t data_size) {
    reg_init(&cpu->regs);
    mem_init(&cpu->mem, size_t text_size, size_t data_size);
    cpu->pc = 0x00400000;
}

void cpu_free(CPU, *cpu) {
    mem_free(&cpu->mem);
}

void cpu_step(CPU *cpu) {
    // fetch
    uint32_t instr = mem_fetch_instruction(&cpu->mem, &cpu->pc);
    
    // decode
    cpu->ctrl = control_decode(instr);

    // extract fields from instructions, bits...
    uint8_t rs = (instr >> 21) & 0x1F;    // 25-21
    uint8_t rt = (instr >> 16) & 0x1F;    // 20-16
    uint8_t rd = (instr >> 11) & 0x1F;    // 15-11
    uint8_t imm = instr & 0xFFFF;         // bits 15-0 (I-type instr)
    uint32_t target = instr & 0x03FFFFFF; // 25-0 (J-type instr)

    // need sign extension for immediates
    int32_t imm_ext = (imm & 0x8000) ? (0xFFFF0000 | imm) : imm;
    
    // read register values
    uint32_t a = req_read(&cpu->regs, rs);
    uint32_t b = reg_read(&cpu->regs, rt);

    // execute
    uint32_t alu_b = cpu->ctrl.ALUSrc ? imm_ext : b; // if imm need extend else take b
    uint32_t alu_result;
    uint32_t zero_flag;
    alu_result = alu_execute(a, alu_b, cpu->ctrl.alu_op, &zero_flag);
    
    // memory
    if (cpu->ctrl.MemRead) {
        uint32_t read_data = mem_read_word(&cpu->mem, alu_result);
        // for LW, write mem data to rt in writeback stage
        alu_result = read_data; // pass to writeback
    } else if (cpu->ctrl.MemWrite) {
        mem_write_word(&cpu->mem, alu_result, b);
    }

    // writeback to update register file
    if (cpu->ctrl.RegWrite) {
        uint8_t dest_reg = (cpu->ctrl.MemtoReg || cpu->ctrl.alu_op == ALU_SLT) ? rt : rd;
        reg_write(&cpu->regs, dest_reg, alu_result);
    }

    // update program counter
    uint32_t next_pc = cpu->pc + 4; // default for sequential instrs

    if (cpu->ctrl.Branch && zero_flag) {
        next_pc = cpu->pc + 4 + (imm_ext << 2); // branch offset is imm << 2 word aligned

    } else if (cpu->ctrl.Jump) {
        // jump target (world aligned) : (pc + 4 upper 4-bits) | target << 2
        next_pc = (cpu->pc + 4) & 0xF0000000;
        next_pc |= (target << 2);
    } 
    cpu->pc = next_pc;
}   

void cpu_run(CPU *cpu) {
    while (1) {
        // break on invalid pc or halt instruction
        if (cpu->pc < 0x00400000 || cpu->pc >= 0x00400000 + cpu->mem.text_size * 4) {
            break;
        }
        cpu_step(cpu);
    }
}