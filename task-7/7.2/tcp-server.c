#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <signal.h>
#include <errno.h>
#define BUFF_SIZE 64
#define PORT 25565
#define CLIENTS_COUNT 5

void client_func(int cltfd) {
	char buffer[BUFF_SIZE];
	int read_n;
	int write_n;
	while (1) {
		memset(buffer, 0, BUFF_SIZE);
		printf("Waiting for message from client...\n");

		read_n = read(cltfd, buffer, BUFF_SIZE);
		if (read_n == -1) {
			printf("read() failed: %s\n", strerror(errno));
			close(cltfd);
			exit(1);
		}
		if (read_n == 0) {
			printf("Empty read, pid: %d\n", getpid());
			return;
		}

		printf("Message received: %s\n", buffer);
		sleep(1);

		write_n = write(cltfd, buffer, BUFF_SIZE);
		if (write_n == -1) {
			printf("write() failed: %s\n", strerror(errno));
			close(cltfd);
			exit(1);
		}
		printf("Message sent back.\n\n");
		sleep(1);
	}
}

int main() {
	int sfd, cltfd;
	struct sockaddr_in srv_sockaddr, clt_sockaddr;

	//Init server socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd == -1) {
		printf("socket() failed: %s\n", strerror(errno));
		exit(1);
	}

	memset(&srv_sockaddr, 0, sizeof(srv_sockaddr));
	memset(&clt_sockaddr, 0, sizeof(clt_sockaddr));

	//Filling server information
	srv_sockaddr.sin_family = AF_INET;
	srv_sockaddr.sin_port = htons(PORT);
	srv_sockaddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sfd, (struct sockaddr*)&srv_sockaddr, sizeof(srv_sockaddr)) == -1) {
		printf("bind() failed: %s\n", strerror(errno));
		close(sfd);
		exit(1);
	}

	if (listen(sfd, CLIENTS_COUNT) == -1){
		printf("listen() failed: %s\n", strerror(errno));
		close(sfd);
		exit(1);
	}
	int port = PORT;
	printf("TCP echo server is in listening mode...\n\tPORT - %d\n", port);
	socklen_t len = sizeof(clt_sockaddr);

	while (1) {
		cltfd = accept(sfd, (struct sockaddr*)&clt_sockaddr, &len);
		if (cltfd == -1) {
			printf("accept() failed: %s\n", strerror(errno));
			close(sfd);
			exit(1);
		}

		int pid = fork();
		if (pid == 0) {
			close(sfd);
			client_func(cltfd);
			close(cltfd);
			break;
		}
		else if (pid < 0) {
			printf("fork() failed: %s\n", strerror(errno));
			close(sfd);
			close(cltfd);
			exit(1);
		}
		else {
			close(cltfd);
		}
	}
	close(sfd);
	return 0;
}
