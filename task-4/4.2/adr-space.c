#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/socket.h>

int i = 1;
int size = 4096;

void stack_array(void) {
	char arr[4096];
	printf("Address of array: %p\n", arr);
	printf("%d - Size: %d Bytes. Pid - %d\n", i++, size+=4096, getpid());
	usleep(10000);
	if (i > 2000) {
		return;
	}
	stack_array();
}

void heap_array(void) {
	int length = 50;
	char* p[length];
	printf("Start\n");
	for (int i = 0; i < length; i++) {
		printf("Malloc number %d\n", i+1);
		p[i] = malloc(1024*100);
		usleep(100000);
	}
	for (int i = 0; i < length; i++) {
		printf("Free number %d\n", i+1);
		free(p[i]);
		usleep(100000);
	}
	printf("End\n");
}

void handler_sigsegv(int signum) {
	printf("POIMALI SIGSEGV!\n");
	signal(signum, SIG_DFL);
	exit(signum);
}

int main() {
	printf("pid:	%d\n", getpid());
	sleep(5);
	printf("Stack array\n");
	stack_array();

	printf("Heap array\n");
	heap_array();

	char *addr = mmap(NULL, 4096*10, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED) {
		perror("mmap error");
		return 0;
	}
	printf("PROT_RWX,\tp = %p\n", addr);
	sleep(3);
	mprotect(addr, 4096*10, PROT_NONE);
	printf("PROT_NONE\n");
	sleep(3);
/*
	char data[4096];

	printf("Try to read...\n");
	//signal(SIGSEGV, handler_sigsegv);
	//memcpy(data, addr, 4096);

	for (int i = 0; i < 4096; i++) {
		data[i] = i%256;
	}
	printf("Try to write...\n");
	//signal(SIGSEGV, handler_sigsegv);
	//memcpy(addr, data, 4096);;
*/
	printf("munmap...\n");
	sleep(3);

	munmap(addr + 4096*4, 4096*2);

	printf("Разбили регион на 2\n");
	sleep(5);

	munmap(addr, 4096*4);

	printf("Минус один...\n");
	sleep(3);

	munmap(addr + 4096*6, 4096*4);

	printf("Минус второй...\n");
	sleep(25);
}
