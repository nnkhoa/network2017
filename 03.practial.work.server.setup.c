#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(){
	int socketfd, clen, clientfd;

	struct sockaddr_in saddr, caddr;

	unsigned short port = 8554;

	if((socketfd = socket(AF_INET, SOCK_STREAM, 0 )) < 0){
		printf("Error creating socket\n");
		return -1;
	}

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);

	if((bind(socketfd, (struct sockaddr *) &saddr, sizeof(saddr))) < 0){
		printf("Error in binding\n");
		return -1;
	}

	if(listen(socketfd, 5) < 0){
		printf("Error Listening\n");
		return -1;
	}

	clen = sizeof(caddr);

	if ((clientfd = accept(socketfd, (struct sockaddr *) &caddr, &clen)) < 0){
		printf("Error accpeting connection\n");
		return -1;
	}
	
	printf("There's a connected client\n");
	printf("Exiting...\n");

	return 0;
}