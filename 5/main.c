#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int my_system(const char *cmd_string) {
    pid_t pid;
    int status;

    if (cmd_string == NULL)
        return (1);

    if ((pid = fork()) < 0) {
        status = -1;
    } else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmd_string, (char *)0);
        _exit(127);
    } else {
        while (waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                status = -1;
                break;
            }
        }
    }

    return (status);
}



int main(void) {
    int status;

    if ((status = my_system("date")) < 0) {
        fprintf(stderr, "%s\n", "Error while invoking system()");
    }
    fprintf(stderr, "Exit code: %d\n", status);

    if ((status = my_system("nosuchcommand")) < 0) {
        fprintf(stderr, "%s\n", "Error while invoking system()");
    }
    fprintf(stderr, "Exit code: %d\n", status);

    if ((status = my_system("who; exit 44")) < 0)  {
        fprintf(stderr, "%s\n", "Error while invoking system()");
    }
    fprintf(stderr, "Exit code: %d\n", status);

    return 0;
}