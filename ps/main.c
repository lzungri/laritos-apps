#include <stdio.h>
#include <printf.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <utils/utils.h>

#define COLOR_BLUE    "\e[01;34m"
#define COLOR_RESTORE "\e[00m"

static int read_proc_sysfs(char *pid, char *sysfsnode, char *buf, int blen) {
    char fullpath[128] = { 0 };
    snprintf(fullpath, sizeof(fullpath), "/proc/%s/%s", pid, sysfsnode);

    file_t *f = open(fullpath, ACCESS_MODE_READ);
    if (f == NULL) {
        memset(buf, 0, blen);
        return -1;
    }
    int nbytes = read(f, buf, blen - 1);
    buf[nbytes] = '\0';
    close(f);

    return nbytes;
}

static void print_proc_info(char *pid) {
    char buf[128] = { 0 };
    read_proc_sysfs(pid, "name", buf, sizeof(buf));
    printf("%-7.7s ", buf);
    printf("%4.4s ", pid);
    read_proc_sysfs(pid, "ppid", buf, sizeof(buf));
    printf("%4.4s ", buf);
    read_proc_sysfs(pid, "prio", buf, sizeof(buf));
    printf("%3.3s ", buf);
    read_proc_sysfs(pid, "kernel", buf, sizeof(buf));
    printf("%-1.1s ", buf[0] == 1 ? "K" : "U");
    read_proc_sysfs(pid, "status", buf, sizeof(buf));
    printf("%-8.8s ", buf);
    read_proc_sysfs(pid, "start_time", buf, sizeof(buf));
    printf("%9.9s ", buf);
    read_proc_sysfs(pid, "pc", buf, sizeof(buf));
    printf("%-35.35s ", buf);
    read_proc_sysfs(pid, "mode", buf, sizeof(buf));
    printf("%13.13s ", buf);
    read_proc_sysfs(pid, "cmd", buf, sizeof(buf));
    printf("%-20.20s", buf);
}

int main(void) {
    printf(COLOR_BLUE "%-7.7s %4.4s %4.4s %3.3s %-1.1s %-8.8s %9.9s %35.35s %13.13s %-20.20s" COLOR_RESTORE "\n",
            "name", "pid", "ppid", "prio", "K", "status", "starttime", "pc", "mode", "cmd");

    int offset = 0;
    int nentries;
    listdir_t dirs[8];

    do {
        nentries = listdir("/proc/", offset, dirs, ARRAYSIZE(dirs));
        if (nentries == 0) {
            return 0;
        }
        if (nentries < 0) {
            printf("Couldn't list '/proc' directory\n");
            return -1;
        }
        offset += nentries;

        int i;
        for (i = 0; i < nentries; i++) {
            if (dirs[i].name[0] >= '0' && dirs[i].name[0] <= '9') {
                print_proc_info(dirs[i].name);
                printf("\n");
            }
        }
    } while (nentries == ARRAYSIZE(dirs));

    return 0;
}
