#include <stdio.h>
#include <printf.h>
#include <unistd.h>

int main(void) {
    printf("Starting 'sleep' process");
    unsigned char i;
    for (i = 3; i < 5; i++) {
        printf("Sleeping for %u seconds", i);
        sleep(i);
    }
    printf("Finishing 'sleep' process");
    return 0;
}
