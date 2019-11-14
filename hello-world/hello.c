#include <stdio.h>

char data[0xaa];
char datac[16] = { 1, 1, 1 };
char datac2[16] = { 2, 2, 2 };
char datac3[16] = { 3, 3, 3 };

void func(void) {
    data[2] = 5;
    datac3[2] = 4;

    asm("svc 1");
}

void func2(void) {
    data[1] = 5;
    func();
    datac3[1] = 5;
}

int main(void) {
//    puts("Hello World!");
    func2();
    data[0] = 4;
    datac[0] = 4;
    datac2[0] = 4;
    return 0;
}
