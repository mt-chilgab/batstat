#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void main(){

	int sock, msgsock, rval, length;
	struct sockaddr_in server;

	// Opening stream socket
	sock = socket(AF_INET, SOCK_STREAM, 0);	
	if(sock < 0){
		perror("Opening Stream Socket");
		exit(1);
	}

	// Binding stream socket (to port 40000)
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;	
	server.sin_port = htons(40000);
	length = sizeof(server);

	if(bind(sock, (struct sockaddr *)&server, sizeof(server))){
		perror("Binding Stream Socket");
		exit(1);
	}
	
	// Get socket name
	if(getsockname(sock, (struct sockaddr *)&server, &length)){
		perror("Getting Socket Name");
		exit(1);
	}
	printf("Socket has port #%d\n", ntohs(server.sin_port));

	// Start listening on the port and accepting
	#define SIZE 1024

	char *buf = malloc(sizeof(char)*SIZE);
	listen(sock, 5);
	do{
		msgsock = accept(sock, 0, 0);
		if(msgsock == -1){
			perror("Accept");
			exit(1);
		}
		else{
			memset(buf, '\0', sizeof(char)*SIZE);
			do{
				if((rval = read(msgsock, buf, sizeof(char)*SIZE)) < 0) perror("Reading Message");
				else if(rval == 0){
					printf("Ending connection\n");
					exit(0);
				}
				else printf("-->%s\n", buf);
			}while(rval > 0);
		}
	}while(1);
	
	close(sock);
	close(msgsock);
}
