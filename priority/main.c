#include <stdio.h>
#include <printf.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>

int main(void) {
    switch (getpid() % 2) {
    case 0:
        // [CONFIG_SCHED_PRIORITY_MAX_KERNEL, SCHED_PRIORITY_MAX_USER) can only
        // be used by kernel processes
        if (set_priority(0) < 0) {
            printf("OK: a user process should not be able to set a priority of 0\n");
        } else {
            printf("ERROR: a user process should not be able to set a priority of 0\n");
        }
        set_priority(100);
        break;
    case 1:
        set_priority(101);
        break;
    default:
        break;
    }

    unsigned char i;
    for (i = 3; i < 7; i++) {
        time_t t;
        time(&t);
        uint64_t deadline = t.secs + 6;
        while (time(&t) >= 0 && t.secs < deadline);

        printf("Sleeping for %u seconds\n", i);
        sleep(i);
        printf("Waking up\n");
    }

    printf("Finishing 'sleep' process\n");
    return 0;
}
