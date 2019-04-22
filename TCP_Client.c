// TCP Client program 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define PORT 5000 
#define MAXLINE 1024 
void TCP_CALL(){
	int sockfd; 
	char buffer[MAXLINE]; 
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
	strcpy(buffer, "Hello Server"); 
	write(sockfd, buffer, sizeof(buffer)); 
	printf("Message from server: "); 
	read(sockfd, buffer, sizeof(buffer)); 
	puts(buffer); 
	close(sockfd);
}
void UDP_CALL(){
while(1){
	int sockfd; 
	char buffer[MAXLINE]; 
	char* message = "Hello From client"; 
	struct sockaddr_in servaddr; 

	int n, len; 
	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
		printf("socket creation failed"); 
		exit(0); 
	} 
//	while(1){
	memset(&servaddr, 0, sizeof(servaddr)); 
	//while(1){
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	// send hello message to server 
	//while(1){	
	sendto(sockfd, (const char*)message, strlen(message), 
		0, (const struct sockaddr*)&servaddr, 
		sizeof(servaddr)); 
	close(sockfd); 
	sleep(10);
	}
	//return 0; 
}
//copy to here from code he posted reviever
//void Broadcast__(
//	int sock;
//	struct
//	braodcast
//}
int main() { 
	pthread_t threads[2];
	struct sockaddr_in servaddr, cli;
	int rc;
  	//Creating 2 threads
	//for(int i = 0; i < 2; i++){
	//pthread_attr_init(&attr);
	rc = pthread_create(&threads[0],NULL, TCP_CALL, (void*)NULL);
	rc = pthread_create(&threads[1],NULL, UDP_CALL, (void*)NULL);
	//rc = pthread_create(&threads[2],NULL, Broadcast_reviever_call, (void*)NULL);
	//}
	//main thread waits until all threads are done
	//for(int i =0; i < 2; i++){
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	//}
}
