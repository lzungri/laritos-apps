#pragma once

#define MAX_ARGS 5
#define MAX_CMD_LEN 128

#define STATUS_TERMINATE 0xAABBCCDD

typedef struct {
    char *cmd;
    int (*handler)(char *fullcmd, int argc, char **argv);
    uint8_t minargs;
    char *help;
    char *syntax;
} builtin_t;

extern int cmdstatus;
extern builtin_t BUILTINS[];
void show_banner(void);
