#include <stdio.h>
#include <stdlib.h>
#include <printf.h>


static void f3(void) {
    printf("f3 begin");
    char a;
    a = 3;
    yield();
    printf("3 = %d", a);
    printf("f3 end");
}

static void f2(void) {
    printf("f2 begin");
    void *ptr = f3;
    yield();
    f3();
    printf("0x%p = 0x%p", f3, ptr);
    printf("f2 end");
}

static void f1(void) {
    int a;
    printf("f1 begin");
    a = 1;
    yield();
    f2();
    printf("1 = %d", a);
    printf("f1 end");
}

int main(void) {
    printf("Starting 'yield' process");
    long a = 2;
    f1();
    // To validate that we preserve the right values between context switches
    printf("2 = %d", a);
    return 0;
}
