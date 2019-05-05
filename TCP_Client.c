// TCP Client program 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define PORT 5000 
#define MAXLINE 1024 
#define MAXRECVSTRING 255  /* Longest string to receive */
char* fileID[MAXLINE];

struct arg_struct{
	char* fileName[MAXLINE];
	int round;
};
struct broadcast{
	char* port[MAXLINE];
	char* ip[MAXLINE];
	char* stringB[MAXLINE];
};
void TCP_CALL(void *arguments){
	struct arg_struct *args = arguments;
	int sockfd; 
	char buffer[MAXLINE]; 
	rc = pthread_create(&threads[0],NULL, TCP_CALL, (void*)&args2);
	//rc = pthread_create(&threads[2],NULL, Broadcast_reviever_sender, (void*)&broad);
	//rc = pthread_create(&threads[2],NULL, Broadcast_reviever_call, (void*)NULL);
	//}
	char* message = "Hello Server"; 
	struct sockaddr_in servaddr; 

	int n, len; 
	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("socket creation failed"); 
		exit(0); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 

	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

	if (connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr)) < 0) { 
		printf("\n Error : Connect Failed \n"); 
	} 
	memset(buffer, 0, sizeof(buffer)); 
	strcpy(buffer, args  -> fileName); 
	write(sockfd, buffer, sizeof(buffer)); 
	read(sockfd, buffer, sizeof(buffer)); 
	if((int*)args -> round == 2){
		printf("Client ID: "); 
		strcpy(fileID, buffer); 
		puts(buffer); 
	}
	if((int*)args -> round == 1){
		printf("File ID: "); 
		puts(buffer); 
	}
	close(sockfd);
}
void UDP_CALL(){
	//while(1){
		printf("ggg");
		int sockfd; 
		char buffer[MAXLINE]; 
		char* message = "Hello From client 1"; 
		struct sockaddr_in servaddr; 

		int n, len; 
		//hile(1){
		// Creating socket file descriptor 
		if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
			printf("socket creation failed"); 
			exit(0); 
		}
		printf("ffffff");
		while(1){
		memset(&servaddr, 0, sizeof(servaddr)); 
		//while(1){
		// Filling server information 
		servaddr.sin_family = AF_INET; 
		servaddr.sin_port = htons(PORT); 
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
		// send hello message to server 
		//while(1){	
		printf("qqqq");
		sendto(sockfd, (const char*)message, strlen(message), 
			0, (const struct sockaddr*)&servaddr, 
			sizeof(servaddr)); 
		close(sockfd); 
		sleep(1);
	}
	//return 0; 
}
//copy to here from code he posted reviever
void Broadcast_reviever_reciever(int argc, char *argv[]){
	int sock;                         /* Socket */
	struct sockaddr_in broadcastAddr; /* Broadcast Address */
	unsigned short broadcastPort;     /* Port */
	char recvString[MAXRECVSTRING+1]; /* Buffer for received string */
	int recvStringLen;                /* Length of received string */

	if (argc != 2)    /* Test for correct number of arguments */
	{
	fprintf(stderr,"Usage: %s <Broadcast Port>\n", argv[0]);
	exit(1);
	}

	broadcastPort = atoi(argv[1]);   /* First arg: broadcast port */

	/* Create a best-effort datagram socket using UDP */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
	printf("Socket() failed\n");
	//DieWithError("socket() failed");
	}
	/* Construct bind structure */
	memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
	broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
	broadcastAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* Any incoming interface */
	broadcastAddr.sin_port = htons(broadcastPort);      /* Broadcast port */

	/* Bind to the broadcast port */
	if (bind(sock, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) < 0)
	{
	//DieWithError("bind() failed");
	printf("bind() failed");
	}
	/* Receive a single datagram from the server */
	if ((recvStringLen = recvfrom(sock, recvString, MAXRECVSTRING, 0, NULL, 0)) < 0)
	{
	printf("recvfrom() failed");
	}
	//DieWithError("recvfrom() failed");

	recvString[recvStringLen] = '\0';
	printf("Received: %s\n", recvString);    /* Print the received string */

	close(sock);
}
void Broadcast_reviever_sender(void *arguments)
{
	struct broadcast *args = arguments;
	int sock;                         /* Socket */
	struct sockaddr_in broadcastAddr; /* Broadcast address */
	char *broadcastIP;                /* IP broadcast address */
	unsigned short broadcastPort;     /* Server port */
	char *sendString;                 /* String to broadcast */
	int broadcastPermission;          /* Socket opt to set permission to broadcast */
	unsigned int sendStringLen;       /* Length of string to broadcast */

	//if (argc < 4)                     /* Test for correct number of parameters */
	//{
	//fprintf(stderr,"Usage:  %s <IP Address> <Port> <Send String>\n", argv[0]);
	//exit(1);
	//}

	broadcastIP = args->ip;            /* First arg:  broadcast IP address */ 
	broadcastPort = atoi(args->port);    /* Second arg:  broadcast port */
	sendString = fileID;             /* Third arg:  string to broadcast */

	/* Create socket for sending/receiving datagrams */
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	//if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
	printf("socket() failed");
	}
	//DieWithError("socket() failed");

	/* Set socket to allow broadcast */
	broadcastPermission = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
	//if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (void *) &broadcastPermission, 
	  sizeof(broadcastPermission)) < 0)
	{
	printf("setsockeopt() failed");
	}
	  //DieWithError("setsockopt() failed");

	/* Construct local address structure */
	memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
	broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
	broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP);/* Broadcast IP address */
	broadcastAddr.sin_port = htons(broadcastPort);         /* Broadcast port */

	sendStringLen = strlen(sendString);  /* Find length of sendString */

	for (int i = 0; i < 5; i++) /* Run forever */
	{
	 /* Broadcast sendString in datagram to clients every 3 seconds*/
	 if (sendto(sock, sendString, sendStringLen, 0, (struct sockaddr *) 
	       &broadcastAddr, sizeof(broadcastAddr)) != sendStringLen)
	 {
	     printf("sendto() sent a different number of bytes than expected");
	 }
	     //DieWithError("sendto() sent a different number of bytes than expected");

	sleep(3);   /* Avoids flooding the network */
	}
	/* NOT REACHED */
}
int main(int argc, char *argv[]) { 
	struct arg_struct args, args2;
	struct broadcast broad;
	//args.fileName = file;
	char fileName[MAXLINE];
	if(argv[1] == NULL){
		printf("Please enter a file name \n");
		return 1;	
	}	
	strcpy(args.fileName, argv[1]);
	args.round = 1;
	strcpy(broad.port, "8080");
	strcpy(broad.ip, "127.0.0.1");
	pthread_t threads[3];
	struct sockaddr_in servaddr, cli;
	char* rc;

	rc = pthread_create(&threads[0],NULL, TCP_CALL, (void*)&args);
	rc = pthread_create(&threads[1],NULL, UDP_CALL, (void*)NULL);
	sleep(1); 
	printf("Please enter file name to search for: ");
	fgets(fileName,MAXLINE, stdin);
	strcpy(args2.fileName, fileName);
	args2.round = 2;
	strcpy(broad.stringB, "null");
	rc = pthread_create(&threads[0],NULL, TCP_CALL, (void*)&args2);
	//rc = pthread_create(&threads[2],NULL, Broadcast_reviever_sender, (void*)&broad);
	//rc = pthread_create(&threads[2],NULL, Broadcast_reviever_call, (void*)NULL);
	//}
	//main thread waits until all threads are done
	//for(int i =0; i < 2; i++){
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
	//}


}
