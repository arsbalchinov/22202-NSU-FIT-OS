#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

double global_var = 3.14159;

int main() {
	int pid;
	int local_var = 5;
	printf("Address of local var - %p (%d)\n", &local_var, local_var);
	printf("Address of global var - %p (%f)\n", &global_var, global_var);
	printf("Pid - %d\n", getpid());
	sleep(10);
	printf("\nFork\n\n");

	pid = fork();
	if (pid == 0) {
		printf("I'm a child process (pid %d)\n", getpid());
		printf("My parent process' pid - %d\n", getppid());
		printf("Address of local var - %p (%d)\n", &local_var, local_var);
		printf("Address of global var - %p (%f)\n", &global_var, global_var);
		local_var = 100;
		global_var = 2.71828;
		printf("Changed local var - %p (%d)\n", &local_var, local_var);
		printf("Changed global var - %p (%f)\n", &global_var, global_var);
		sleep(1);
		exit(5);
	}
	else if (pid > 0) {
		sleep(5);
		printf("I'm a parent process (pid %d)\n", getpid());
		printf("Address of local_var - %p (%d)\n", &local_var, local_var);
		printf("Address of global_var - %p (%f)\n", &global_var, global_var);
		sleep(30);
		int status;
		wait(&status);
		if (WIFEXITED(status)) {
			int exit_status = WEXITSTATUS(status);
			printf("Child process with pid %d exit status - %d\n", pid, exit_status);
		}
	}
	else {
		printf("fork() failed: '%s'\n", strerror(errno));
	}
	return 0;
}
