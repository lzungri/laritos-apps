#include <stdio.h>
#include <stdbool.h>
#include <printf.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"


int builtin_exit(char *cmd, int argc, char **argv) {
    return STATUS_TERMINATE;
}

int builtin_cd(char *cmd, int argc, char **argv) {
    printf("Entering dir %s\n", argv[1]);
    return 0;
}

int builtin_pwd(char *cmd, int argc, char **argv) {
    return 0;
}
