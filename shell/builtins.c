#include <stdio.h>
#include <stdbool.h>
#include <printf.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

extern int cmdstatus;

int builtin_exit(char *fullcmd, int argc, char **argv) {
    return STATUS_TERMINATE;
}

int builtin_cd(char *fullcmd, int argc, char **argv) {
    if (chdir(argv[1]) < 0) {
        printf("cd: %s: No such directory\n", argv[1]);
        return -1;
    }
    return 0;
}

int builtin_pwd(char *fullcmd, int argc, char **argv) {
    char cwd[128];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
    return 0;
}

int builtin_backdoor(char *fullcmd, int argc, char **argv) {
    void *param = argc > 2 ? argv[2] : NULL;
    printf("Sending command '%s 0x%x' to the kernel\n", argv[1], param);
    return backdoor(argv[1], param);
}

int builtin_status(char *fullcmd, int argc, char **argv) {
    printf("%d\n", cmdstatus);
    return 0;
}
