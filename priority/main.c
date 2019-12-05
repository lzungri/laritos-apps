#include <stdio.h>
#include <printf.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>

int main(void) {
    switch (getpid()) {
    case 0:
        // Pid 0 will always be READY
        set_priority(102);
        while(1);
        break;
    case 1:
        // [CONFIG_SCHED_PRIORITY_MAX_KERNEL, SCHED_PRIORITY_MAX_USER) can only
        // be used by kernel processes
        if (set_priority(0) < 0) {
            printf("OK: a user process should not be able to set a priority of 0");
        } else {
            printf("ERROR: a user process should not be able to set a priority of 0");
        }
        set_priority(100);
        break;
    case 2:
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

        printf("Sleeping for %u seconds", i);
        sleep(i);
        printf("Waking up");
    }

    // PID 2 should never finish (PID 0 is always ready and with higher priority)
    if (getpid() == 2) {
        set_priority(255);
    }

    printf("Finishing 'sleep' process");
    return 0;
}
