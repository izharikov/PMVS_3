#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#define PORT_NO 20001
#define MAX_CONNECTIONS 7
#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 256

void* thread_function(void* param){
	char command[BUFFER_SIZE];
	char buffer[BUFFER_SIZE];
	int bytes_send = 0, bytes_read = 0, client_id = (intptr_t)param;
	memset(buffer, 0, sizeof(buffer));
	
	bytes_read = read(client_id, command, sizeof(command) - 1);
	if ( bytes_read == -1){
		printf("ERROR on reading command.\n");
		close(client_id);
		return NULL;
	}
	command[bytes_read] = 0;
	printf("Readed info: %s\n", command);
	FILE *fp;
	fp = popen(command, "r");
	if (fp == NULL) {
		printf("Failed to run command\n" );
		exit(1);
	}
	while (TRUE) {
		bytes_read = fread(buffer, 1, BUFFER_SIZE, fp);
	
		if (bytes_read > 0) {
			bytes_send = write(client_id, buffer, bytes_read);
			if (bytes_send < bytes_read)
				printf("Error sending command output.\n");
		}

		if (bytes_read < BUFFER_SIZE) {
			if (feof(fp))
				printf("Command output was sent.\n");
			if (ferror(fp))
				printf("Error reading command output.\n");
			break;
		}
	}
	pclose(fp);
	close(client_id);
}

int main(){
	int socket_id = 0, client_id = 0, opt = 1;
	struct sockaddr_in server_socket;
#ifdef DEF_PROCESS
	pid_t p_id = 0;
#else 
	pthread_t client_thr[MAX_CONNECTIONS];
	memset(client_thr, 0, sizeof(client_thr));
	int i = 0;
#endif
	if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("ERROR opening socket.\n");
		return 1;
	}
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = INADDR_ANY;
	server_socket.sin_port = htons(PORT_NO);

	if ((bind(socket_id, (struct sockaddr *)&server_socket, sizeof(server_socket))) == -1) {
		printf("ERROR on binding.\n");
		return 1;
	}

	if (listen(socket_id, MAX_CONNECTIONS) == -1) {
		printf("ERROR on listening.\n");
		return -1;
	}
	while(TRUE){
		if ( (client_id = accept(socket_id, NULL, (socklen_t *)SOCK_CLOEXEC)) < 0){
			printf("ERROR on accept client.\n");
		}
#ifdef DEF_PROCESS
		switch (p_id = fork()) {
		case -1:
			printf("Process creation failed.\n");
			break;
		case 0:
			thread_function((void*)(intptr_t)client_id);
			return 0;
		default:
		  	close(client_id);
			break;
		}
#else
		for (i = 0; i < MAX_CONNECTIONS; ++i){
			if ((!client_thr[i]) || (pthread_kill(client_thr[i], 0) != ESRCH))
				break;
		}
		if (i >= MAX_CONNECTIONS) {
			printf("No free threads.\n");
			continue;
		}

		if (pthread_create(&client_thr[i], NULL, thread_function, (void*)(intptr_t)client_id)) {
			printf("Thread wasn't created.\n");
			continue;
		}
		printf("Thread was created.\n");
#endif
	}

}
