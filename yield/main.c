#include <stdio.h>
#include <stdlib.h>
#include <printf.h>


static void f3(void) {
    printf("f3 begin\n");
    char a;
    a = 3;
    yield();
    printf("3 = %d\n", a);
    printf("f3 end\n");
}

static void f2(void) {
    printf("f2 begin\n");
    void *ptr = f3;
    yield();
    f3();
    printf("0x%p = 0x%p\n", f3, ptr);
    printf("f2 end\n");
}

static void f1(void) {
    int a;
    printf("f1 begin\n");
    a = 1;
    yield();
    f2();
    printf("1 = %d\n", a);
    printf("f1 end\n");
}

int main(void) {
    printf("Starting 'yield' process\n");
    long a = 2;
    f1();
    // To validate that we preserve the right values between context switches
    printf("2 = %d\n", a);
    return 0;
}
