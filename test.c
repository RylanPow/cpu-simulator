#include <stdio.h>
#include "registers.h"
#include "alu.h"
#include "memory.h"

void test_alu(uint32_t a, uint32_t b, ALUOp op, const char *name) {
    uint32_t zero;
    uint32_t y = alu_execute(a, b, op, &zero);
    printf("%s: a=0x%08X, b=0x%08X -> y=0x%08X, zero=%u\n", name, a, b, y, zero);
}


int main() {
    // RegisterFile rf;
    // reg_init(&rf);

    // reg_write(&rf, 0, 123);
    // printf("$0 = %u (should be 0)\n", reg_read(&rf, 0));

    // reg_write(&rf, 8, 0xDEADBEEF);
    // printf("$t0 = 0x%X\n", reg_read(&rf, 8)); 

    // //REMINDER: OUTPUT IN HEX
    // test_alu(3, 5, ALU_ADD, "ADD");
    // test_alu(10, 7, ALU_SUB, "SUB");
    // test_alu(3, 10, ALU_SLT, "SLT");
    // test_alu(0, 1, ALU_AND, "AND");

    // // memory tests
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

    return 0;
}