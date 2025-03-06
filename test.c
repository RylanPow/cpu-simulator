#include <stdio.h>
#include "registers.h"
#include "alu.h"

void test_alu(uint32_t a, uint32_t b, ALUOp op, const char *name) {
    uint32_t zero;
    uint32_t y = alu_execute(a, b, op, &zero);
    printf("%s: a=0x%08X, b=0x%08X -> y=0x%08X, zero=%u\n", name, a, b, y, zero);
}


int main() {
    RegisterFile rf;
    reg_init(&rf);

    reg_write(&rf, 0, 123);
    printf("$0 = %u (should be 0)\n", reg_read(&rf, 0));

    reg_write(&rf, 8, 0xDEADBEEF);
    printf("$t0 = 0x%X\n", reg_read(&rf, 8)); 

    //REMINDER: OUTPUT IN HEX
    test_alu(3, 5, ALU_ADD, "ADD");
    test_alu(10, 7, ALU_SUB, "SUB");
    test_alu(3, 10, ALU_SLT, "SLT");
    test_alu(0, 1, ALU_AND, "AND");

    return 0;
}