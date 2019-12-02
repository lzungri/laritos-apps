#include <stdint.h>
#include <stdio.h>
#include <printf.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    time_t cur, prev;
    time(&prev);
    while (1) {
        time(&cur);
        if (cur.secs != prev.secs) {
            printf("pid=%u, time=%lu", getpid(), (uint32_t) cur.secs);
            prev = cur;
        }
    }
    return 0;
}
