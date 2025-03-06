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