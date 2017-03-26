#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){
	struct sockaddr_in saddr;
	struct hostent *h;

	int socketfd;

	unsigned short port = 8554;

	char hostname[256]; 

	char buffer[256];

	char temp[256];
	
	if(argc > 2){
		printf("Too many arguments.\n");
		printf("Exiting with return code: -1\n");
		return -1;
	}else if(argc == 2){
		strcpy(hostname, argv[1]);
	}else if (argc == 1){
		printf("Enter hostname: ");
		scanf("%s", hostname);
	}

	if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Error creating socket\n");
		close(socketfd);
		return -1;
	}

	if((h=gethostbyname(hostname)) == NULL){
		printf("Unknown host\n");
		close(socketfd);
		return -1;
	}

	memset(&saddr, 0, sizeof(saddr));

	saddr.sin_family = AF_INET;

	memcpy((char *)&saddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);

	struct in_addr **addr_list = (struct in_addr **)h -> h_addr_list;

	struct in_addr ip = *addr_list[0];

	printf("%s\n", inet_ntoa(ip));

	saddr.sin_port = htons(port);

	if(connect(socketfd, (struct sockaddr *) &saddr, sizeof(saddr)) < 0 ){
		printf("Cannot connect\n");
		close(socketfd);
		return -1;
	}

	printf("Connected!\n");

	while(1){
		memset(buffer, 0, sizeof(buffer));
		
		while(1){
			recv(socketfd, temp, sizeof(temp), 0);
			strcat(buffer, temp);
			memset(temp, 0, sizeof(temp));
			if(buffer[strlen(buffer)] == '\0') break;
		}

		printf(">Server: %s\n", buffer);
		
		memset(buffer, 0, sizeof(buffer));
		
		printf(">Client: ");
		
		if(fgets(buffer, sizeof(buffer), stdin) == NULL){
			return -1;
		}

		if(strcmp(buffer, "/quit\n") == 0){
			shutdown(socketfd, SHUT_RDWR);
			close(socketfd);
			printf("Exitting....\n");
			break;
		}

		send(socketfd, buffer, strlen(buffer), 0);
	}

	return 0;
}