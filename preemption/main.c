#include <stdint.h>
#include <stdio.h>
#include <printf.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    time_t cur, prev, deadline;
    time(&prev);
    deadline = prev;
    deadline.secs += 10 + 10 * getpid();
    do {
        time(&cur);
        if (cur.secs >= prev.secs) {
            printf("time=%lu\n", (uint32_t) cur.secs);
            prev = cur;
        }
    } while(deadline.secs > cur.secs);
    printf("Finishing process\n");
    return 0;
}
