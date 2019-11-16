#include <stdio.h>
#include <printf.h>

char data[3];
char datac[3] = { 1, 1, 1 };
char datac2[3] = { 2, 2, 2 };
char datac3[3] = { 3, 3, 3 };

void func(void) {
    data[0] = 5;
    datac[0] = 6;
    datac2[0] = 7;
    datac3[0] = 8;
}

void func2(void) {
    data[1] = 5;
    datac[1] = 6;
    datac2[1] = 7;
    func();
    datac3[1] = 8;
}

int main(void) {
    printf("original data[]   = { %u, %u, %u }", data[0], data[1], data[2]);
    printf("original datac[]  = { %u, %u, %u }", datac[0], datac[1], datac[2]);
    printf("original datac2[] = { %u, %u, %u }", datac2[0], datac2[1], datac2[2]);
    printf("original datac3[] = { %u, %u, %u }", datac3[0], datac3[1], datac3[2]);

    func2();
    data[2] = 5;
    datac[2] = 6;
    datac2[2] = 7;
    datac3[2] = 8;
    printf("final    data[]   = { %u, %u, %u }", data[0], data[1], data[2]);
    printf("final    datac[]  = { %u, %u, %u }", datac[0], datac[1], datac[2]);
    printf("final    datac2[] = { %u, %u, %u }", datac2[0], datac2[1], datac2[2]);
    printf("final    datac3[] = { %u, %u, %u }", datac3[0], datac3[1], datac3[2]);
    return 0;
}
