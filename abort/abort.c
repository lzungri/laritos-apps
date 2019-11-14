#include <stdio.h>

int main(void) {
    // Set up some registers to check whether the dump
    // message matches the register values
    asm("mov r0, #0");
    asm("mov r1, #1");
    asm("mov r2, #2");
    asm("mov r3, #3");
    asm("mov r4, #4");
    asm("mov r5, #5");
    asm("mov r6, #6");
    asm("mov r7, #7");

    // Undefined instruction
    asm(".word 0xffffffff");

    // Abort exception
//    asm("movw r7, #0xffff");
//    asm("movt r7, #0xffff");
//    asm("str r6, [r7]");

    // Prefetch exception
//    asm("movw r7, #0xffff");
//    asm("movt r7, #0xffff");
//    asm("mov pc, r7");
    return 0;
}
