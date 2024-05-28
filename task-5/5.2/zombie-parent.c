#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

double global_var = 3.14159;

int main() {
	int pid;
    int local_var = 5;
    printf("local var: %p (%d)\n", &local_var, local_var);
    printf("global var: %p (%f)\n", &global_var, global_var);
    printf("Parent pid: %d\n\n", getpid());

    pid = fork();
    if (pid == 0) {		//Child process
		int new_pid;

        new_pid = fork();
        if (new_pid == 0) {		//Child process of child
            printf("Grandchild(%d): Parent pid of child process: %d\n\n", getpid(), getppid());
            sleep(5);
            printf("\nGrandchild(%d): Parent pid of orphan process: %d\n", getpid(), getppid());
            sleep(30);
        }
		else if (new_pid > 0) {	//Just child process
            sleep(3);
            printf("Child(%d): Zombie-parent pid: %d\n", getpid(), getpid());
            printf("Child(%d): Child of zombie pid: %d\n", getpid(), new_pid);
            exit(5);
        }
		else {
			printf("fork() failed '%s'\n", strerror(errno));
		}
    }

	else if (pid > 0) {	//Parent process
        sleep(60);
    }

	else {
		printf("fork() failed '%s'\n", strerror(errno));
	}
	return 0;
}

