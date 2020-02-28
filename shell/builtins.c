#include <stdio.h>
#include <stdbool.h>
#include <printf.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <utils/utils.h>
#include "shell.h"

static int builtin_exit(char *fullcmd, int argc, char **argv) {
    return STATUS_TERMINATE;
}

static int builtin_cat(char *fullcmd, int argc, char **argv) {
    file_t *f = open(argv[1], ACCESS_MODE_READ);
    if (f == NULL) {
        printf("No such file or directory\n");
        return -1;
    }

    char buf[32];
    int nbytes;
    while ((nbytes = read(f, buf, sizeof(buf) - 1)) > 0) {
        buf[nbytes] = '\0';
        printf("%s", buf);
        if (strchr(buf, CHAR_END_OF_TEXT) != NULL) {
            break;
        }
    }
    printf("\n");

    if (nbytes < 0) {
        printf("Couldn't read '%s'\n", argv[1]);
    }

    close(f);

    return 0;
}

static int builtin_xxd(char *fullcmd, int argc, char **argv) {
    file_t *f = open(argv[1], ACCESS_MODE_READ);
    if (f == NULL) {
        printf("No such file or directory\n");
        return -1;
    }

    char buf[16];
    int nbytes;
    uint32_t pos = 0;
    while ((nbytes = read(f, buf, sizeof(buf))) > 0) {
        printf("%06lu:", pos);
        pos += nbytes;

        int i;
        for (i = 0; i < sizeof(buf); i++) {
            if (i % 4 == 0) {
                printf(" ");
            }
            if (i < nbytes) {
                printf("%02x", buf[i]);
            } else {
                printf("  ");
            }
        }
        printf("  |  ");
        for (i = 0; i < sizeof(buf); i++) {
            if (i < nbytes) {
                printf("%c", is_printable(buf[i]) ? buf[i] : '.');
            }
        }
        printf("\n");

        if (strchr(buf, CHAR_END_OF_TEXT) != NULL) {
            break;
        }
    }
    printf("\n");

    if (nbytes < 0) {
        printf("Couldn't read '%s'\n", argv[1]);
    }

    close(f);

    return 0;
}

static int builtin_cd(char *fullcmd, int argc, char **argv) {
    if (chdir(argv[1]) < 0) {
        printf("cd: %s: No such directory\n", argv[1]);
        return -1;
    }
    return 0;
}

static int builtin_ls(char *fullcmd, int argc, char **argv) {
    int offset = 0;
    int nentries;
    listdir_t dirs[8];

    do {
        nentries = listdir(argc > 1 ? argv[1] : NULL, offset, dirs, ARRAYSIZE(dirs));
        if (nentries == 0) {
            return 0;
        }
        if (nentries < 0) {
            printf("Couldn't list directory\n");
            return -1;
        }
        offset += nentries;
        int i;
        for (i = 0; i < nentries; i++) {
            printf("%s%s\e[00m\n", dirs[i].isdir ? "\e[01;34m" : "", dirs[i].name);
        }
    } while (nentries == ARRAYSIZE(dirs));

    return 0;
}

static int builtin_pwd(char *fullcmd, int argc, char **argv) {
    char cwd[128];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
    return 0;
}

static int builtin_backdoor(char *fullcmd, int argc, char **argv) {
    void *param = argc > 2 ? argv[2] : NULL;
    printf("Sending command '%s 0x%x' to the kernel\n", argv[1], param);
    return backdoor(argv[1], param);
}

static int builtin_status(char *fullcmd, int argc, char **argv) {
    printf("%d\n", cmdstatus);
    return 0;
}

static int builtin_time(char *fullcmd, int argc, char **argv) {
    time_t t;
    time(&t);
    printf("%lu\n", (uint32_t) t.secs);
    return 0;
}

static int builtin_help(char *cmd, int argc, char **argv);

// Null-terminated array of builtins
builtin_t BUILTINS[] = {
    { .cmd = "cat",
        .handler = builtin_cat,
        .help = "Print contents of the given file",
        .minargs = 1,
        .syntax = "cat <file>",
    },
    { .cmd = "xxd",
        .handler = builtin_xxd,
        .help = "Print contents of the given file in hex",
        .minargs = 1,
        .syntax = "xxd <file>",
    },
    { .cmd = "cd",
        .handler = builtin_cd,
        .help = "Change the current directory to the given dir",
        .minargs = 1,
        .syntax = "cd <dir>",
    },
    { .cmd = "ls",
        .handler = builtin_ls,
        .help = "List directory contents",
    },
    { .cmd = "pwd",
        .handler = builtin_pwd,
        .help = "Print name of current/working directory",
    },
    { .cmd = "time",
        .handler = builtin_time,
        .help = "Seconds since 1970-01-01 00:00:00 UTC",
    },
    { .cmd = "status",
        .handler = builtin_status,
        .help = "Print last command exit status",
    },
    { .cmd = "bd",
        .handler = builtin_backdoor,
        .help = "Backdoor to access the kernel for CIA/debugging purposes",
        .minargs = 1,
        .syntax = "bd <command> [<param>]",
    },
    { .cmd = "exit",
        .handler = builtin_exit,
        .help = "Quit the shell",
    },
    { .cmd = "help",
        .handler = builtin_help,
        .help = "Print this help message",
    },
    { 0 },
};

static int builtin_help(char *fullcmd, int argc, char **argv) {
    printf("Builtins:\n");
    builtin_t *b;
    for (b = BUILTINS; b->cmd != NULL; b++) {
        printf("  %-10.10s %s\n", b->cmd, b->help);
    }
    return 0;
}
