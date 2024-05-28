#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#define PAGE_SIZE 4096

void writer(int* buffer) {
	int val = 0;
	while (1) {
		for (int i = 0; i < PAGE_SIZE/sizeof(int); i++) {
			buffer[i] = val++;
			//usleep(1000);
		}
	}
}
void reader(int* buffer) {
	sleep(3);
	int cur_val = 0;
	int prev_val = -1;
	while (1) {
		for (int i = 0; i < PAGE_SIZE/sizeof(int); i++) {
			cur_val = buffer[i];
			if (cur_val - prev_val != 1) {
				printf("Fail in sequence between %d and %d\n", prev_val, cur_val);
				sleep(3);
			}
			printf("%d ", buffer[i]);
			fflush(stdout);
			prev_val = cur_val;
			//usleep(1000);
		}
	}
}

int main() {
	int pid;
	printf("Pid: %d\n\n", getpid());
	char *addr = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED) {
		perror("mmap error");
		return 0;
	}
	printf("Memory mapped - %p\n", addr);
	memset(addr, 0, PAGE_SIZE);
	int* buffer = (int*)addr;
	sleep(1);

	pid = fork();
	if (pid == 0) {					//Child process - reading
		sleep(10);
		printf("READ started...\n");
		reader(buffer);
	}

	else if (pid > 0) {				//Parent process - writing
		printf("Reading process pid: %d\n\n", pid);
		sleep(10);
		printf("WRITE started...\n");
		writer(buffer);
	}

	else {
		printf("fork() failed\n");
	}

	munmap(addr, PAGE_SIZE);
	return 0;
}
