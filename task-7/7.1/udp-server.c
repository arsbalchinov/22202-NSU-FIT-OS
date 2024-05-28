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
#define BUFF_SIZE 1024
#define PORT 25565

int main() {
	int sfd;
	char buffer[BUFF_SIZE];
	struct sockaddr_in srv_sockaddr, clt_sockaddr;

	//Init server socket
	sfd = socket(AF_INET, SOCK_DGRAM, 0);
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
	socklen_t len = sizeof(clt_sockaddr);
	int n;

	while (1) {
		printf("Waiting for message from client...\n");

		n = recvfrom(sfd, buffer, BUFF_SIZE, 0, (struct sockaddr *)&clt_sockaddr, &len);
		if (n == -1) {
			printf("recvfrom() failed: %s\n", strerror(errno));
			close(sfd);
			exit(1);
		}
    	buffer[n] = '\0';

    	printf("Message received: %s\n", buffer);
		sleep(1);

    	if (sendto(sfd, buffer, BUFF_SIZE, 0, (struct sockaddr*)&clt_sockaddr, len) == -1) {
			printf("sendto() failed: %s\n", strerror(errno));
			close(sfd);
			exit(1);
		}

		printf("Message sent back.\n\n");
		sleep(1);
	}
	close(sfd);
	return 0;
}
