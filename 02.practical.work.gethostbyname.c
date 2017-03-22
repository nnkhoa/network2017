#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv){
	char hostname[256]; 
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

	printf("%s\n", hostname);

	struct hostent *host_ent = gethostbyname(hostname);

	struct in_addr **addr_list = (struct in_addr **)host_ent -> h_addr_list;
	
	for(int i = 0; addr_list[i] != NULL; i++){
		struct in_addr ip = *addr_list[i];
		printf("%s\n", inet_ntoa(ip));
	}
	return 0;
}