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

int main() {
	printf("Pid: %d\n", getpid());
	int sfd;
	char buffer[BUFF_SIZE];
	struct sockaddr_in srv_sockaddr;

	//Init server socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd == -1) {
		printf("socket() failed: %s\n", strerror(errno));
		exit(1);
	}

	memset(&srv_sockaddr, 0, sizeof(srv_sockaddr));

	//Filling server information
	srv_sockaddr.sin_family = AF_INET;
	srv_sockaddr.sin_port = htons(PORT);
	srv_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sfd, (struct sockaddr*)&srv_sockaddr, sizeof(srv_sockaddr)) == -1) {
		printf("connect() failed: %s\n", strerror(errno));
		close(sfd);
		exit(1);
	}
	socklen_t len = sizeof(srv_sockaddr);
	int n;
	const char *hello = "Hello from client!";

	while (1) {
        printf("Sending message from client...\n");
        sleep(1);
        n = sendto(sfd, hello, strlen(hello), 0, (struct sockaddr *)&srv_sockaddr, len);
        if (n == -1) {
            printf("sendto() failed: %s\n", strerror(errno));
            close(sfd);
            exit(1);
        }

        printf("Message sent: %s\n", hello);
        if (recvfrom(sfd, buffer, BUFF_SIZE, 0, (struct sockaddr*)&srv_sockaddr, &len) == -1) {
            printf("recvfrom() failed: %s\n", strerror(errno));
            close(sfd);
            exit(1);
        }
        buffer[n] = '\0';

        printf("Message received: '%s'.\n\n", buffer);
        sleep(5);
    }
	close(sfd);
	return 0;
}
