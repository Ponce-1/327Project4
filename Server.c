// Server program 
#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <time.h>
#include <string.h>
#define PORT 5000 
#define MAXLINE 1024 
#define CLIENTNUM 3

struct record // data structure to record files
{
	int userid;
	char filename[1024];
	struct tm * timestamp;	
};

int max(int x, int y) 
{ 
	if (x > y) 
		return x; 
	else
		return y; 
} 

int main() 
{ 	
	int id = 0; // incrementing id
	struct record records[CLIENTNUM]; // array to hold client information
	int listenfd, connfd, udpfd, nready, maxfdp1; 
	char buffer[MAXLINE]; 
	pid_t childpid; 
	fd_set rset; 
	ssize_t n; 
	socklen_t len; 
	const int on = 1; 
	struct sockaddr_in cliaddr, servaddr; 
	char* message = "Hello Client"; 
	void sig_chld(int); 

	/* create listening TCP socket */
	listenfd = socket(AF_INET, SOCK_STREAM, 0); 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// binding server addr structure to listenfd 
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
	listen(listenfd, 10); 

	/* create UDP socket */
	udpfd = socket(AF_INET, SOCK_DGRAM, 0); 
	// binding server addr structure to udp sockfd 
	bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 

	// clear the descriptor set 
	FD_ZERO(&rset); 

	// get maxfd 
	maxfdp1 = max(listenfd, udpfd) + 1; 
	for (;;) { 

		// set listenfd and udpfd in readset 
		FD_SET(listenfd, &rset); 
		FD_SET(udpfd, &rset); 

		// select the ready descriptor 
		nready = select(maxfdp1, &rset, NULL, NULL, NULL); 

		// if tcp socket is readable then handle 
		// it by accepting the connection 
		if (FD_ISSET(listenfd, &rset)) { 
			len = sizeof(cliaddr); 
			connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len); 
			if ((childpid = fork()) == 0) { 
				close(listenfd); 
				bzero(buffer, sizeof(buffer)); 
				printf("Message From TCP client: ");
				read(connfd, buffer, sizeof(buffer)); 
				puts(buffer); 

				// Need to save filename, id, and timestamp for each connection
				records[id].userid = id;
				strncpy(records[id].filename, buffer, sizeof(buffer)); // save filename
				time_t rawtime;
				struct tm *timeinfo;
				time( &rawtime );
				timeinfo = localtime( &rawtime );	
				records[id].timestamp = timeinfo;
				////
				char buf[1024];
				//itoa(records[id].userid, buf, 10);
				sprintf(buf, "%d", records[id].userid);
				write(connfd, buf, sizeof(buf));//records[id].userid, sizeof((const char*)records[id].userid)); 
				
				////////
				printf("%d", records[id].userid);
				printf("%s", records[id].filename);
				printf("%s", asctime(records[id].timestamp));
				close(connfd); 
				////////

				exit(0); 
			} 
			///
			id = id + 1;
			///
			close(connfd); 
		} 
		// if udp socket is readable receive the message. 
		if (FD_ISSET(udpfd, &rset)) { 
			len = sizeof(cliaddr); 
			bzero(buffer, sizeof(buffer)); 
			printf("\nMessage from UDP client: "); 
			n = recvfrom(udpfd, buffer, sizeof(buffer), 0, 
						(struct sockaddr*)&cliaddr, &len); 
			puts(buffer); 
			//sendto(udpfd, (const char*)message, sizeof(buffer), 0, 
			//	(struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
		} 
	} 
} 

