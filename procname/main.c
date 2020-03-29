#include <stdio.h>
#include <printf.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>

int main(void) {
    switch (getpid() % 2) {
    case 0:
        set_process_name("Proc even");
        break;
    case 1:
        set_process_name("Proc odd");
        break;
    default:
        break;
    }

    printf("Infinite loop...\n");
    while (1);

    return 0;
}
