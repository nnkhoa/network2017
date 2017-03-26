#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
	int socketfd, clen, clientfd;

	struct sockaddr_in saddr, caddr;

	unsigned short port = 8554;

	char buffer[256];

	char temp[256];

	if((socketfd = socket(AF_INET, SOCK_STREAM, 0 )) < 0){
		printf("Error creating socket\n");
		close(socketfd);
		return -1;
	}

	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

	int fl = fcntl(socketfd, F_GETFL, 0);
	fl |= O_NONBLOCK;
	fcntl(socketfd, F_SETFL, fl);

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	if((bind(socketfd, (struct sockaddr *) &saddr, sizeof(saddr))) < 0){
		printf("Error in binding\n");
		close(socketfd);
		return -1;
	}

	if(listen(socketfd, 5) < 0){
		printf("Error Listening\n");
		close(socketfd);
		return -1;
	}

	clen = sizeof(caddr);

	while(1){
		if ((clientfd = accept(socketfd, (struct sockaddr *) &caddr, &clen)) > 0){
			
			int cfl = fcntl(clientfd, F_GETFL, 0);
			cfl |= O_NONBLOCK;
			fcntl(clientfd, F_SETFL, cfl);
			
			printf("There's a connected client\n");
			
			while(1){
				memset(buffer, 0, sizeof(buffer));
				if(recv(clientfd, buffer, sizeof(buffer), 0) > 0){
					printf("%s\n", buffer);
				}
			}
		}
	}
	printf("Exiting...\n");

	return 0;
}