#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main() {
    pid_t pid;
    pid = fork();

    if (pid == 0) {
        return 6;
    } else if (pid > 0) {
        system("ps -ax");
        wait(NULL);
        printf("After wait method call\n");
        system("ps -ax");
        return 0;
    }

    return 0;
}