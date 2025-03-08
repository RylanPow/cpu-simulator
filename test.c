#include <stdio.h>
#include "registers.h"
#include "alu.h"
#include "memory.h"
#include "loader.h"
#include "control.h"

void test_alu(uint32_t a, uint32_t b, ALUOp op, const char *name) {
    uint32_t zero;
    uint32_t y = alu_execute(a, b, op, &zero);
    printf("%s: a=0x%08X, b=0x%08X -> y=0x%08X, zero=%u\n", name, a, b, y, zero);
}

void print_control_signals(ControlSignals ctrl) {
    printf("RegWrite: %u\n", ctrl.RegWrite);
    printf("ALUSrc: %u\n", ctrl.ALUSrc);
    printf("MemtoReg: %u\n", ctrl.MemtoReg);
    printf("MemRead: %u\n", ctrl.MemRead);
    printf("MemWrite: %u\n", ctrl.MemWrite);
    printf("Branch: %u\n", ctrl.Branch);
    printf("Jump: %u\n", ctrl.Jump);
    printf("ALUOp: %d\n\n", ctrl.alu_op);
}


int main() {
    // registers tests
    RegisterFile rf;
    reg_init(&rf);

    reg_write(&rf, 0, 123);
    printf("$0 = %u (should be 0)\n", reg_read(&rf, 0));

    reg_write(&rf, 8, 0xDEADBEEF);
    printf("$t0 = 0x%X\n", reg_read(&rf, 8)); 

    // ALU tests
    // REMINDER: OUTPUT IN HEX
    test_alu(3, 5, ALU_ADD, "ADD");
    test_alu(10, 7, ALU_SUB, "SUB");
    test_alu(3, 10, ALU_SLT, "SLT");
    test_alu(0, 1, ALU_AND, "AND");

    // memory tests
    Memory mem;
    mem_init(&mem, 1024, 65536);
    uint8_t program[] = {0x12, 0x34, 0x56, 0x78};
    mem_load_program(&mem, program, sizeof(program));

    uint32_t pc = 0x00400000;
    uint32_t instr = mem_fetch_instruction(&mem, pc);
    printf("Instruction at 0x%08X: 0x%08X\n", pc, instr);
    mem_write_word(&mem, 0x10010000, 0xDEADBEEF);

    uint32_t data = mem_read_word(&mem, 0x10010000);
    printf("Data at 0x10010000: 0x%08X\n", data);  // Should print 0xDEADBEEF
    mem_free(&mem);
    
    // loader tests
    // IMPORTANT: test.bin is raw binary.  use "hexdump -C test.bin" to see hex values
    Memory mem2;
    mem_init(&mem2, 1024, 65536);
    
    if (!load_program(&mem, "test.bin")) {
        return 1;
    }

 
    pc = 0x00400000;
    uint32_t instr2 = mem_fetch_instruction(&mem2, pc);
    printf("Instruction 1: 0x%08X\n", instr2);

    pc = 0x00400004;
    instr2 = mem_fetch_instruction(&mem2, pc);
    printf("Instruction 2: 0x%08X\n", instr2);

    mem_free(&mem2);


    // controls tests
    // R-type ADD (opcode 0x00, funct 0x20)
    uint32_t add_instr = 0x012A4020;  // add $t0, $t1, $t2
    ControlSignals add_ctrl = control_decode(add_instr);
    printf("ADD (R-type):\n");
    print_control_signals(add_ctrl);

    // lw (opcode 0x23)
    uint32_t lw_instr = 0x8D280004;  // lw $t0, 4($t1)
    ControlSignals lw_ctrl = control_decode(lw_instr);
    printf("LW:\n");
    print_control_signals(lw_ctrl);

    // beq (opcode 0x04)
    uint32_t beq_instr = 0x1122000A;  // beq $t1, $v0, 10
    ControlSignals beq_ctrl = control_decode(beq_instr);
    printf("BEQ:\n");
    print_control_signals(beq_ctrl);    

    return 0;
}