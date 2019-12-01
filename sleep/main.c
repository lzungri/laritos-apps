#include <stdio.h>
#include <printf.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

int main(void) {
    printf("Starting 'sleep' process");

    unsigned char i;
    for (i = 3; i < 6; i++) {
        time_t t;
        time(&t);
        uint64_t deadline = t.secs + 6;
        while (time(&t) >= 0 && t.secs < deadline);

        printf("Sleeping for %u seconds", i);
        sleep(i);
        printf("Waking up");
    }
    printf("Finishing 'sleep' process");
    return 0;
}