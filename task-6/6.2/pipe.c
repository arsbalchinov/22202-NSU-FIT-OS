#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define PAGE_SIZE 4096

void writer(int fd) {
	int val = 0;
	while (1) {
		for (int i = 0; i < PAGE_SIZE/sizeof(int); i++) {
			write(fd, &val, sizeof(int));
			val++;
			//usleep(1000);
		}
	}
}
void reader(int fd) {
	sleep(3);
	int prev_val = -1;
	int cur_val = 0;

	while (1) {
		for (int i = 0; i < PAGE_SIZE/sizeof(int); i++) {
			read(fd, &cur_val, sizeof(int));
			if (cur_val - prev_val != 1) {
				printf("\nFail in sequence between %d and %d\n", prev_val, cur_val);
				sleep(3);
			}
			//printf("%d ", cur_val);
			fflush(stdout);
			prev_val = cur_val;
			//usleep(1000);
		}
	}
}

int main() {
	int pid;
	printf("Pid - %d\n", getpid());
	int pipes[2];
	pipe(pipes);

	pid = fork();
	if (pid == 0) {					//Child process - reading
		sleep(10);
		printf("READ started...\n");
		reader(pipes[0]);
	}
	else if (pid > 0) {				//Parent process - writing
		printf("Reading process pid: %d\n\n", pid);
		sleep(10);
		printf("WRITE started...\n");
		writer(pipes[1]);
	}
	else {
		printf("fork() failed\n");
	}
	close(pipes[0]);
	close(pipes[1]);
	return 0;
}
