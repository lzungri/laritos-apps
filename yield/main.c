#include <stdio.h>
#include <stdlib.h>


static void f3(void) {
    puts("f3 begin");
    yield();
    puts("f3 end");
}

static void f2(void) {
    puts("f2 begin");
    yield();
    f3();
    puts("f2 end");
}

static void f1(void) {
    puts("f1 begin");
    yield();
    f2();
    puts("f1 end");
}

int main(void) {
    puts("Starting 'yield' process");
    f1();
    return 0;
}
