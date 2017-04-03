#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

int main(){
	int socketfd, clen, clientfd;

	int const MAX_CLIENT = 100;

	int clientfds[MAX_CLIENT];

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

	memset(clientfds, 0, sizeof(clientfds));

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
		fd_set set;
		FD_ZERO(&set);
		FD_SET(socketfd, &set);

		int maxfd = socketfd;

		for(int i = 0; i < MAX_CLIENT; i++){
			if(clientfds[i] > 0){
				FD_SET(clientfds[i], &set);
			}

			if(clientfds[i] > maxfd){
				maxfd = clientfds[i];
			}
		}

		select(maxfd + 1, &set, NULL, NULL, NULL);

		// printf(".");

		if(FD_ISSET(socketfd, &set)){
			clientfd = accept(socketfd, (struct sockaddr*)&saddr, &clen);

			int cfl = fcntl(clientfd, F_GETFL, 0);
			cfl |= O_NONBLOCK;
			fcntl(clientfd, F_SETFL, cfl);

			for(int i = 0; i < MAX_CLIENT; i++){
				if(clientfds[i] == 0){
					clientfds[i] = clientfd;
					break;
				}			
			}

			for(int i = 0; i < MAX_CLIENT; i++){
				if(clientfds[i] > 0 && FD_ISSET(clientfds[i], &set)){
					if(read(clientfds[i], buffer, sizeof(buffer)) > 0){
						printf("client %d said: %s\n", clientfds[i], buffer);
					} else{
						printf("client %d has disconnected\n", clientfds[i]);
						clientfds[i] = 0;
					}
				}
			}
		}
	}
	printf("Exiting...\n");

	return 0;
}