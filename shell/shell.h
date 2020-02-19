#pragma once

#define MAX_ARGS 5
#define MAX_CMD_LEN 128

#define STATUS_TERMINATE 0xAABBCCDD

typedef struct {
    char *cmd;
    int (*handler)(char *cmd, int argc, char **argv);
    uint8_t minargs;
    char *help;
    char *syntax;
} builtin_t;

int builtin_exit(char *cmd, int argc, char **argv);
int builtin_cd(char *cmd, int argc, char **argv);
int builtin_pwd(char *cmd, int argc, char **argv);