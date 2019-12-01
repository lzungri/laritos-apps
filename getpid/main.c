#include <stdio.h>
#include <stdlib.h>
#include <printf.h>

int main(void) {
    char buf[10];
    int pid = getpid();
    for (int i = 0; i < 5; i++) {
        buf[5 - i - 1] = '0' + pid % 10;
        pid = pid / 10;
    }
    puts("Process pid = ");
    puts(buf);
    return 0;
}
