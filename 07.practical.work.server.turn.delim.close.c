#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

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
		if ((clientfd = accept(socketfd, (struct sockaddr *) &caddr, &clen)) < 0){
			printf("Error accpeting connection\n");
			close(clientfd);
			continue;
		}
		
		printf("There's a connected client\n");
		
		while(1){
			memset(buffer, 0, sizeof(buffer));
			
			printf(">Server: ");
			
			fgets(buffer, sizeof(buffer), stdin);
			
			if(strcmp(buffer, "/dc\n") == 0){
				shutdown(clientfd, SHUT_RDWR);
				close(clientfd);
				break;
			}
			
			send(clientfd, buffer, strlen(buffer), 0);
			
			memset(buffer, 0, sizeof(buffer));
			
			while(1){
				recv(clientfd, temp, sizeof(temp), 0);
				strcat(buffer, temp);
				memset(temp, 0, sizeof(temp));
				if(buffer[strlen(buffer)] == '\0') break;
			}
			printf(">Client: %s\n", buffer);
		}
	}
	printf("Exiting...\n");

	return 0;
}