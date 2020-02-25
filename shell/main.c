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

char *banners[] = {
    " ____                      \n"
    "< Hi >                     \n"
    " ----                      \n"
    "   \\   ^__^               \n"
    "    \\  (oo)\\_______      \n"
    "       (__)\\       )\\/\\\n"
    "           ||----w |      \n"
    "           ||     ||      \n",

    "                                   \n"
    "       `'. ..         ,,           \n"
    "      ,````,;`````*,.',,         HI\n"
    "      ,,``` ,,......``,;`.         \n"
    "   ''`   ,.`     '''```*. ``,,'    \n"
    "   ,*   ;`               *   *,    \n"
    "  .`   ,`                ';   `'   \n"
    "  ;   ,^                 `^,,  `.  \n"
    " .`  `;`             ,,,,,,*;   '  \n"
    ",;   ]`.-'**^^*·,,,;`      ` ;  ;  \n"
    "';,  ;*         ,  *       ' ,.;   \n"
    "  *, ^`',    ,,*    `*···*^`  ;    \n"
    "   `*    `````                     \n",

    " ______                                                             \n"
    "<  Hi  >                                                            \n"
    " ------                        / `.   .' '                          \n"
    "      \\              .---.  <    > <    >  .---.                   \n"
    "       \\             |    \\  \\ - ~ ~ - /  /    |                 \n"
    "         _____          ..-~             ~-..-~                     \n"
    "        |     |   \\~~~\\.'                    `./~~~/              \n"
    "       ---------   \\__/                        \\__/               \n"
    "      .'  O    \\     /               /       \\  '                 \n"
    "     (_____,    `._.'               |         }  \\/~~~/            \n"
    "      `----.          /       }     |        /    \\__/             \n"
    "            `-.      |       /      |       /      `. ,~~|          \n"
    "                ~-.__|      /_ - ~ ^|      /- _      `..-'          \n"
    "                     |     /        |     /     ~-.     `-. _  _  _ \n"
    "                     |_____|        |_____|         ~ - . _ _ _ _ _>\n"
    "                                                                    \n",

    " ____            \n"
    "< Hi >           \n"
    " ----            \n"
    "   \\            \n"
    "    \\           \n"
    "        .--.     \n"
    "       |o_o |    \n"
    "       |:_/ |    \n"
    "      //   \\ \\ \n"
    "     (|     | )  \n"
    "    /'\\_   _/`\\\n"
    "    \\___)=(___/ \n",

};


static void show_banner(void) {
    time_t t;
    time(&t);
    char *line = banners[t.secs % ARRAYSIZE(banners)];
    while (line != NULL && *line != '\0') {
        char *lineend = strchr(line, '\n');
        if (lineend == NULL) {
            printf("%s\n", line);
            break;
        }
        *lineend = '\0';
        printf("%s\n", line);
        line = lineend + 1;
    }
    printf("\n");
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

static inline void print_prompt(int status) {
    char cwd[128];
    getcwd(cwd, sizeof(cwd));
    printf("\e[01;34m%s\e[00m $ ", cwd);
}

int cmdstatus = 0;

int main(void) {
    set_process_name("shell");

    show_banner();

    while (cmdstatus != STATUS_TERMINATE) {
        print_prompt(cmdstatus);

        char cmd[MAX_CMD_LEN];
        if (readline_and_prompt(cmd, sizeof(cmd)) <= 0) {
            cmdstatus = 0;
            continue;
        }

        int argc;
        char *argv[MAX_ARGS] = { 0 };
        if (parse_args(cmd, &argc, argv) < 0) {
            printf("Error parsing arguments\n");
            continue;
        }

        cmdstatus = dispatch_command(cmd, argc, argv);
    }

    printf("Bye\n");
    return 0;
}
