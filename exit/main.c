#include <stdio.h>
#include <stdlib.h>

static void f1(void) {
    puts("f1 begin");
    exit(1);
    puts("f1 end (should not be printed)");
}

int main(void) {
    puts("Starting 'exit' process");
    f1();
    puts("This message should not be printed");
    return 0;
}
