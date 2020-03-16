#include <stdio.h>
#include <printf.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>

int main(void) {
    printf("Starting 'sleep' process\n");

    unsigned char i;
    for (i = 3; i < 6; i++) {
        printf("Sleeping for %u seconds\n", i);
        sleep(i);
        printf("Waking up\n");
    }
    printf("Finishing 'sleep' process\n");
    return 0;
}
