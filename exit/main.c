#include <stdio.h>
#include <stdlib.h>

static void f1(void) {
    puts("f1 begin\n");
    exit(1);
    puts("f1 end (should not be printed)\n");
}

int main(void) {
    puts("Starting 'exit' process\n");
    f1();
    puts("This message should not be printed\n");
    return 0;
}
