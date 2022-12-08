#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main() {
    setbuf(stdout, NULL);
    printf("Main PID %d\n", getpid());
    int local = 0;
    pid_t pid[2];
    pid[0] = fork();

    if (pid[0] == 0) {
        // child process
        local++;
        printf("\nLocal variable = %d\nAddress = %p\nParent PID = %d\nOwn PID = %d\n", local, &local, getppid(), getpid());

        pid_t pidd[2];
        pidd[0] = fork();

        if (pidd[0] == 0) {
            // child of child
            local++;
            printf("\nLocal variable = %d\nAddress = %p\nParent PID = %d\nOwn PID = %d\n", local, &local, getppid(), getpid());
        } else if (pidd[0] > 0) {
            pidd[1] = fork();
            if (pidd[1] == 0) {
                local++;
                printf("\nLocal variable = %d\nAddress = %p\nParent PID = %d\nOwn PID = %d\n", local, &local, getppid(), getpid());
            } else if (pidd[1] > 0) {
                printf("Second Main IDs1 = %d IDs2 = %d\n", pidd[0], pidd[1]);
                wait(NULL);
            }
        }
    } else if (pid[0] > 0) {
        // parent
        pid[1] = fork();
        if (pid[1] == 0) {
            // child of parent
            local++;
            printf("\nLocal variable = %d\nAddress = %p\nParent PID = %d\nOwn PID = %d\n", local, &local, getppid(), getpid());
            pid_t pidd[2];
            pidd[0] = fork();
            if (pidd[0] == 0) {
                // parent of parent child
                local++;
                printf("\nLocal variable = %d\nAddress = %p\nParent PID = %d\nOwn PID = %d\n", local, &local, getppid(), getpid());
            } else if (pidd[0] > 0) {
                // child of parent child
                pidd[1] = fork();
                if (pidd[1] == 0) {
                    local++;
                    printf("\nLocal variable = %d\nAddress = %p\nParent PID = %d\nOwn PID = %d\n", local, &local, getppid(), getpid());
                } else if (pidd[1] > 0) {
                    printf("Second Main IDs1 = %d IDs2 = %d\n", pidd[0], pidd[1]);
                }
            }
        } else if (pid[1] > 0) {
            printf("Main ID1 = %d ID2 = %d\n", pid[0], pid[1]);
            wait(NULL);
        }
    }

    return 0;
}