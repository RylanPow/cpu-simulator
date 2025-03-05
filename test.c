#include <stdio.h>
#include "registers.h"

int main() {
    RegisterFile rf;
    reg_init(&rf);

    reg_write(&rf, 0, 123);
    printf("$0 = %u (should be 0)\n", reg_read(&rf, 0));

    reg_write(&rf, 8, 0xDEADBEEF);
    printf("$t0 = 0x%X\n", reg_read(&rf, 8)); 

    return 0;
}