#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <printf.h>
#include <time.h>

int main(void) {
    time_t t;
    printf("ret=%d, time secs=%lu, nsecs=%lu\n", time(&t), (uint32_t) t.secs, t.ns);
    return 0;
}
