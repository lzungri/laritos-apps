#include <stdio.h>
#include <stdbool.h>
#include <printf.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"


static void welcome_message(void) {
    printf(" _______________\n");
    printf("< laritOS shell >\n");
    printf(" ---------------\n");
    printf("        \\   ^__^\n");
    printf("         \\  (oo)\\_______\n");
    printf( "            (__)\\       )\\/\\\n");
    printf( "                ||----w |\n");
    printf( "                ||     ||\n");
}

static int parse_args(char *cmd, int *argc, char **argv) {
    argv[0] = cmd;
    *argc = 1;

    for ( ; *argc < MAX_ARGS; (*argc)++) {
        char *token = strchr(argv[*argc - 1], ' ');
        if (token == NULL || token[1] == '\0') {
            break;
        }

        *token = '\0';
        argv[*argc] = token + 1;
    }

    return 0;
}

static int builtin_help(char *cmd, int argc, char **argv);

static builtin_t BUILTINS[] = {
    { .cmd = "exit",
        .handler = builtin_exit,
        .help = "Quit the shell",
    },
    { .cmd = "pwd",
        .handler = builtin_pwd,
        .help = "Print name of current/working directory",
    },
    { .cmd = "cd",
        .handler = builtin_cd,
        .help = "Change  the  current  directory to dir",
        .minargs = 1,
        .syntax = "cd <dir>",
    },
    { .cmd = "help",
        .handler = builtin_help,
        .help = "Print this help message",
    },
};

static int builtin_help(char *cmd, int argc, char **argv) {
    printf("Builtins:\n");
    int i;
    for (i = 0; i < sizeof(BUILTINS) / sizeof(BUILTINS[0]); i++) {
        builtin_t *b = &BUILTINS[i];
        printf("  %-10.10s %s\n", b->cmd, b->help);
    }
    return 0;
}

static int dispatch_command(char *cmd, int argc, char **argv) {
    if (cmd == NULL || cmd[0] == '\0') {
        return 0;
    }

    int i;
    for (i = 0; i < sizeof(BUILTINS) / sizeof(BUILTINS[0]); i++) {
        builtin_t *b = &BUILTINS[i];
        if (strncmp(argv[0], b->cmd, MAX_CMD_LEN) == 0) {
            if (argc < b->minargs + 1) {
                printf("Syntax: %s\n", b->syntax != NULL ? b->syntax : "error");
                return -1;
            }
            return b->handler(cmd, argc, argv);
        }
    }
    printf("Command '%s' not found\n", cmd);
    return -1;
}

int main(void) {
    set_process_name("shell");

    welcome_message();

    int status = 0;

    while (status != STATUS_TERMINATE) {
        printf("[%d] $ ", status);

        char cmd[MAX_CMD_LEN];

        int bytes_read = readline(cmd, sizeof(cmd));
        if (bytes_read < 0) {
            printf("Error reading from input device\n");
            goto exit;
        }
        printf("\n[%d] $ %s\n", status, cmd);

        if (strlen(cmd) <= 0) {
            continue;
        }

        int argc;
        char *argv[MAX_ARGS] = { 0 };
        if (parse_args(cmd, &argc, argv) < 0) {
            printf("Error parsing arguments\n");
            continue;
        }

        status = dispatch_command(cmd, argc, argv);
    }

exit:
    printf("Exiting shell\n");
    return 0;
}
