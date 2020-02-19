#include <stdio.h>
#include <printf.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

int main(void) {
    char buf[128];
    while (readline(buf, sizeof(buf)) > 0) {
        printf("$ %s", buf);
    }
    printf("Exiting...");
    return 0;
}
