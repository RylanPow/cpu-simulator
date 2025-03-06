#include "alu.h"

static uint32_t adder_32bit(uint32_t a, uint32_t b) {
    uint32_t sum = 0;
    uint32_t carry = 0;

    for (int i = 0; i < 32; i ++) {
        uint32_t a_bit = (a >> 1) & 1;
        uint32_t b_bit = (b >> 1) & 1;

        uint32_t sum_bit = a_bit ^ b_bit ^ carry;
        carry =  (a_bit & b_bit) | (carry & (a_bit ^ b_bit));

        sum |= (sum_bit << i);
    }

    return sum;
}

uint32_t alu_execute(uint32_t a, uint32_t b, ALUOp f, uint32_t *zero) {
    uint32_t y = 0;

    switch(f){
        case ALU_AND:
            y = a & b;
            break;
        
        case ALU_OR:
            y = a | b;
            break;
        
        case ALU_ADD:
            y = adder_32bit(a, b);
            break;
        
        case ALU_AND_NOT:
            y = a & ~b;
            break;
        
        case ALU_OR_NOT:
            y = a | ~b;
            break;
        
        case ALU_SUB:
            // two's complement: a - b = a + (-b) + 1
            y = adder_32bit(a, ~b);
            y = adder_32bit(y, 1);
            break;

        case ALU_SLT:
            y = adder_32bit(a, ~b);
            y = adder_32bit(y, 1);
            y = (y >> 31) & 1;
            break;
        
        default:
            y = 0;
            break;
    }

    *zero = (y == 0); //necessary for branching
    return y;    
}