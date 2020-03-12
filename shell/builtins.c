#include <stdio.h>
#include <stdbool.h>
#include <printf.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <utils/property.h>
#include <utils/utils.h>
#include "shell.h"

#define COLOR_BLUE    "\e[01;34m"
#define COLOR_RESTORE "\e[00m"

static int builtin_exit(char *fullcmd, int argc, char **argv) {
    return STATUS_TERMINATE;
}

static int builtin_cat(char *fullcmd, int argc, char **argv) {
    file_t *f = open(argv[1], ACCESS_MODE_READ);
    if (f == NULL) {
        printf("No such file or directory\n");
        return -1;
    }

    char buf[128];
    int nbytes;
    while ((nbytes = read(f, buf, sizeof(buf) - 1)) > 0) {
        buf[nbytes] = '\0';
        printf("%s", buf);
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
        printf("  |  " COLOR_BLUE);
        for (i = 0; i < sizeof(buf); i++) {
            if (i < nbytes) {
                printf("%c", is_printable(buf[i]) ? buf[i] : '.');
            }
        }
        printf(COLOR_RESTORE "\n");
    }
    printf("\n");

    if (nbytes < 0) {
        printf("Couldn't read '%s'\n", argv[1]);
    }

    close(f);

    return 0;
}

static int builtin_run(char *fullcmd, int argc, char **argv) {
    int pid = spawn_process(argv[1]);
    if (pid < 0) {
        printf("Couldn't execute program '%s'\n", argv[1]);
        return pid;
    }
    int status;
    waitpid(pid, &status);
    return status;
}

static int builtin_write(char *fullcmd, int argc, char **argv) {
    file_t *f = open(argv[1], ACCESS_MODE_WRITE);
    if (f == NULL) {
        printf("No such file or directory\n");
        return -1;
    }

    int n = write(f, argv[2], strlen(argv[2]));
    if (n < 0) {
        printf("Couldn't write into file\n");
    }
    printf("%d bytes were written into '%s'\n", n, argv[1]);

    close(f);
    return n;
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
            printf("%s%s" COLOR_RESTORE "\n", dirs[i].isdir ? COLOR_BLUE : "", dirs[i].name);
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

static int builtin_banner(char *fullcmd, int argc, char **argv) {
    show_banner();
    return 0;
}

static int builtin_getprop(char *fullcmd, int argc, char **argv) {
    int offset = 0;
    int nentries;
    listdir_t dirs[8];

    do {
        nentries = listdir("/kernel/property/", offset, dirs, ARRAYSIZE(dirs));
        if (nentries == 0) {
            return 0;
        }
        if (nentries < 0) {
            printf("Couldn't list '/kernel/property/' directory\n");
            return -1;
        }
        offset += nentries;

        int i;
        for (i = 0; i < nentries; i++) {
            if (strncmp(dirs[i].name, "..", 3) == 0) {
                continue;
            }

            char buf[PROPERTY_VALUE_MAX_LEN] = { 0 };
            if (get_property(dirs[i].name, buf) < 0) {
                buf[0] = '?';
            }
            printf(COLOR_BLUE "%s" COLOR_RESTORE "=%s\n", dirs[i].name, buf);
        }

    } while (nentries == ARRAYSIZE(dirs));
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
    { .cmd = "run",
        .handler = builtin_run,
        .help = "Run executable",
        .minargs = 1,
        .syntax = "run <executable>",
    },
    { .cmd = "write",
        .handler = builtin_write,
        .help = "Write <data> into <file>",
        .minargs = 2,
        .syntax = "write <file> <data>",
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
    { .cmd = "getprop",
        .handler = builtin_getprop,
        .help = "List properties and their values",
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
    { .cmd = "banner",
        .handler = builtin_banner,
        .help = "Display a welcome message",
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
