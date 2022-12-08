#include <stdio.h>
#include <unistd.h>

int main() {
	printf("PID %d\n", getpid());
	printf("Group PID %d\n", getpgrp());
	printf("Effective group ID %d\n", getegid());
	printf("User ID %d\n", getuid());
	printf("Parent PID %d\n", getppid());
	printf("Effective user ID %d\n", geteuid());
	printf("Group ID %d\n", getgid());
	return 0;
}
