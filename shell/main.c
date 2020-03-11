#include <stdio.h>
#include <stdbool.h>
#include <printf.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <utils/utils.h>
#include <utils/math.h>
#include "shell.h"

void show_banner(void) {
    time_t t;
    time(&t);

    // 32 max banners supported... lazy
    listdir_t dirs[32];
    int nentries = listdir("/sys/res/shell/banners", 0, dirs, ARRAYSIZE(dirs));
    if (nentries <= 0) {
        printf("No banners found\n");
        return;
    }
    // Ignore '..' dir entry
    uint8_t idx = t.secs % (nentries - 1) + 1;

    char buf[128];
    snprintf(buf, sizeof(buf), "/sys/res/shell/banners/%s", dirs[idx].name);
    file_t *f = open(buf, ACCESS_MODE_READ);
    if (f == NULL) {
        printf("Couldn't open banner '%s'\n", buf);
        return;
    }
    int nbytes;
    while ((nbytes = read(f, buf, sizeof(buf) - 1)) > 0) {
        buf[nbytes] = '\0';
        printf("%s", buf);
    }
    printf("\n\n");

    close(f);
}

static int parse_args(char *cmd, int *argc, char **argv) {
    argv[0] = cmd;
    *argc = 1;

    bool quote_mode = false;
    bool new_arg = false;
    int i;
    for (i = 0; cmd[i] != '\0' && *argc < MAX_ARGS; i++) {
        if (cmd[i] == ' ' && !quote_mode) {
            cmd[i] = '\0';
            new_arg = true;
        } else if (cmd[i] == '\"') {
            cmd[i] = '\0';
            quote_mode = !quote_mode;
        } else if (new_arg) {
            argv[(*argc)++] = &cmd[i];
            new_arg = false;
        }
    }
    return 0;
}

static int dispatch_command(char *cmd, int argc, char **argv) {
    if (cmd == NULL || cmd[0] == '\0') {
        return 0;
    }

    builtin_t *b;
    for (b = BUILTINS; b->cmd != NULL; b++) {
        if (strncmp(argv[0], b->cmd, MAX_CMD_LEN) == 0) {
            if (argc < b->minargs + 1) {
                printf("Syntax: %s\n", b->syntax != NULL ? b->syntax : "error");
                return -1;
            }
            return b->handler(cmd, argc, argv);
        }
    }
    printf("Command '%s' not found. Type 'help' for more info\n", cmd);
    return -1;
}

static inline void print_prompt(void) {
    char cwd[128];
    getcwd(cwd, sizeof(cwd));
    printf("\e[38;5;70m%s\e[00m $ ", cwd);
}

int cmdstatus = 0;

int main(void) {
    set_process_name("shell");

    show_banner();

    while (cmdstatus != STATUS_TERMINATE) {
        print_prompt();

        char cmd[MAX_CMD_LEN];
        int ret = readline_and_prompt(cmd, sizeof(cmd));
        if (ret == 0) {
            cmdstatus = 0;
            continue;
        } else if (ret == CHAR_EOF) {
            break;
        }

        int argc;
        char *argv[MAX_ARGS] = { 0 };
        if (parse_args(cmd, &argc, argv) < 0) {
            printf("Error parsing arguments\n");
            continue;
        }

        cmdstatus = dispatch_command(cmd, argc, argv);
    }

    printf("\nBye\n");
    return 0;
}
