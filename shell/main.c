#include <stdio.h>
#include <stdbool.h>
#include <printf.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 5
#define MAX_CMD_LEN 128

#define STATUS_TERMINATE 0xAABBCCDD

typedef struct {
    char *cmd;
    int (*handler)(char *cmd, int argc, char **argv);
    uint8_t minargs;
    char *syntax;
} builtin_t;


static void welcome_message(void) {
    printf(" _______________");
    printf("< laritOS shell >");
    printf(" ---------------");
    printf("        \\   ^__^");
    printf("         \\  (oo)\\_______");
    printf( "            (__)\\       )\\/\\");
    printf( "                ||----w |");
    printf( "                ||     ||");
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

static int builtin_exit(char *cmd, int argc, char **argv) {
    return STATUS_TERMINATE;
}

static int builtin_cd(char *cmd, int argc, char **argv) {
    printf("Entering dir %s", argv[1]);
    return 0;
}

static builtin_t BUILTINS[] = {
    { .cmd = "exit", .handler = builtin_exit },
    { .cmd = "cd", .handler = builtin_cd, .minargs = 1, .syntax = "cd <dir>" },
};

static int dispatch_command(char *cmd, int argc, char **argv) {
    if (cmd == NULL || cmd[0] == '\0') {
        return 0;
    }

    int i;
    for (i = 0; i < sizeof(BUILTINS) / sizeof(BUILTINS[0]); i++) {
        builtin_t *b = &BUILTINS[i];
        if (strncmp(argv[0], b->cmd, MAX_CMD_LEN) == 0) {
            if (argc < b->minargs + 1) {
                printf("Syntax: %s", b->syntax != NULL ? b->syntax : "error");
                return -1;
            }
            return b->handler(cmd, argc, argv);
        }
    }
    printf("Command '%s' not found", cmd);
    return -1;
}

int main(void) {
    set_process_name("shell");

    welcome_message();

    int status = 0;

    while (status != STATUS_TERMINATE) {
        printf("[%d] $", status);

        char cmd[MAX_CMD_LEN];

        int bytes_read = readline(cmd, sizeof(cmd));
        if (bytes_read < 0) {
            printf("Error reading from input device");
            goto exit;
        }
        printf("[%d] $ %s", status, cmd);

        if (strlen(cmd) <= 0) {
            continue;
        }

        int argc;
        char *argv[MAX_ARGS] = { 0 };
        if (parse_args(cmd, &argc, argv) < 0) {
            printf("Error parsing arguments");
            continue;
        }

        status = dispatch_command(cmd, argc, argv);
    }

exit:
    printf("Exiting shell");
    return 0;
}
