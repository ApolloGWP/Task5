#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define SECONDS 3

int main() {
    pid_t pid;

    printf("Forking started\n");
    pid = fork();

    int count = 0;
    time_t currentTime = time(NULL);

    if (pid == 0) {
        while (time(NULL) != (currentTime + SECONDS)) {
            count++;
        }
        printf("Child PID = %d. Count = %d\n", getpid(), count);
    } else if (pid > 0) {
        while (time(NULL) != (currentTime + SECONDS)) {
            count++;
        }
        printf("Parent PID = %d. Count = %d\n", getpid(), count);
    } else {
        printf("Error\n");
    }

    return 0;
}