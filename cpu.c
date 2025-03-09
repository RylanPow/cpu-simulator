#include "cpu.h"
#include <stdio.h>

void cpu_init(CPU *cpu, size_t text_size, size_t data_size) {
    reg_init(&cpu->regs);
    mem_init(&cpu->mem, text_size, data_size);
    cpu->pc = 0x00400000; //text segment base
}

void cpu_free(CPU *cpu) {
    mem_free(&cpu->mem);
}

void cpu_step(CPU *cpu) {
    // fetch
    uint32_t instr = mem_fetch_instruction(&cpu->mem, cpu->pc);
    
    // decode
    cpu->ctrl = control_decode(instr);

    // extract fields from instructions, bits...
    uint8_t rs = (instr >> 21) & 0x1F;    // 25-21
    uint8_t rt = (instr >> 16) & 0x1F;    // 20-16
    uint8_t rd = (instr >> 11) & 0x1F;    // 15-11
    uint8_t imm = instr & 0xFFFF;         // bits 15-0 (I-type instr)
    uint32_t target = instr & 0x03FFFFFF; // 25-0 (J-type instr)

    // need sign extension for immediates
    int32_t imm_ext = (int32_t)((int16_t)imm);    
    // read register values
    uint32_t a = reg_read(&cpu->regs, rs);
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
        // For I-type: write to rt. For R-type: write to rd.
        uint8_t dest_reg = cpu->ctrl.ALUSrc ? rt : rd;
        reg_write(&cpu->regs, dest_reg, alu_result);
        printf("RegWrite: %d → $%d = 0x%08X\n\n", cpu->ctrl.RegWrite, dest_reg, alu_result);

    }

    // update program counter
    uint32_t next_pc = cpu->pc + 4; // default for sequential instrs

    if (cpu->ctrl.Branch && !zero_flag) {  // bne branches when NOT zero
        int16_t offset = (int16_t)(imm & 0xFFFF);
        next_pc = cpu->pc + 4 + (offset << 2);
    } else if (cpu->ctrl.Jump) {
        // jump target (world aligned) : (pc + 4 upper 4-bits) | target << 2
        next_pc = (cpu->pc + 4) & 0xF0000000;
        next_pc |= (target << 2);
    } 
    cpu->pc = next_pc;

    // In cpu_step():
printf("PC=0x%08X: Instr=0x%08X\n", cpu->pc, instr);
printf("Decoded: rs=%d, rt=%d, imm=0x%04X (ext=0x%08X)\n", rs, rt, imm, imm_ext);
printf("ALU: a=0x%08X, b=0x%08X, op=%d → result=0x%08X, zero=%d\n", a, alu_b, cpu->ctrl.alu_op, alu_result, zero_flag);
printf("Branch: %d, Jump: %d, Next PC=0x%08X\n", cpu->ctrl.Branch, cpu->ctrl.Jump, next_pc);
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