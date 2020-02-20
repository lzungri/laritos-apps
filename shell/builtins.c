#include <stdio.h>
#include <stdbool.h>
#include <printf.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"


int builtin_exit(char *fullcmd, int argc, char **argv) {
    return STATUS_TERMINATE;
}

int builtin_cd(char *fullcmd, int argc, char **argv) {
    printf("Entering dir %s\n", argv[1]);
    return 0;
}

int builtin_pwd(char *fullcmd, int argc, char **argv) {
    return 0;
}

int builtin_backdoor(char *fullcmd, int argc, char **argv) {
    void *param = argc > 2 ? argv[2] : NULL;
    printf("Sending command '%s 0x%x' to the kernel\n", argv[1], param);
    return backdoor(argv[1], param);
}
