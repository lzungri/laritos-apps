#include <stdio.h>

char data[0xaa];
char datac[16] = { 1, 1, 1 };
char datac2[16] = { 2, 2, 2 };
char datac3[16] = { 3, 3, 3 };

void func(void) {
    data[2] = 3;
    datac3[2] = 4;
}

void func2(void) {
    data[1] = 2;
    datac3[1] = 3;
    func();
}

int main(void) {
//    puts("Hello World!");
    func2();
    data[0] = 5;
    datac[0] = 5;
    datac2[0] = 5;
    return 0;
}
