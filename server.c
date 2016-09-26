#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

int sockfd;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int create_exit_thread();
void *exit_func(void *ptr);

int create_client_thread(pthread_t *threat, int* socket);
void *client_function(void *ptr);

int main(int argc, char *argv[])
{
     int newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
	create_exit_thread();
     	while(1){
	     listen(sockfd,5);
	     clilen = sizeof(cli_addr);
	     newsockfd = accept(sockfd, 
		         (struct sockaddr *) &cli_addr, 
		         &clilen);
		pthread_t th;
		#ifndef DEF_PROCESS
		printf("threads used\n");
		create_client_thread(&th, &newsockfd);
		#endif
		#ifdef DEF_PROCESS
		printf("processes used\n");
		pid_t  pid;
		pid = fork();
		if (pid != 0){
			client_function(&newsockfd);
		}
		#endif
	}
     	//return 0; 
}


int create_exit_thread(){
	pthread_t threat;
	return pthread_create( &threat, NULL, exit_func, 0);
}

void *exit_func(void *ptr){
	printf("Enter 'q' to exit...");
	char c;
	while(1){
		c = getchar();
		if ( c == 'q'){
			int option = 1;
			//setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption, sizeof(iSetOption));
			//setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
			close(sockfd);
			printf("Connection closed.");
			exit(0);
		}
	}
}

int create_client_thread(pthread_t *threat, int* socket){
	return pthread_create( threat, NULL, client_function, (void*) socket);
}

void *client_function(void *ptr){
	int* newsockfd_ptr = (int*)ptr;
	int newsockfd = *newsockfd_ptr;
	char buffer[256];
	if (newsockfd < 0) 
		  error("ERROR on accept");
	     bzero(buffer,256);
	int n = read(newsockfd,buffer,255);
	if (n < 0) error("ERROR reading from socket");
	printf("Here is the message: %s\n",buffer);
	n = write(newsockfd,"I got your message",18);
	if (n < 0) error("ERROR writing to socket");
	close(newsockfd);
	//printf("File name: %s\n", buffer);
}
