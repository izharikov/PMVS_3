#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define PORT_NO 20001
#define BUF_SIZE 256

int main(int argc, char *argv[])
{
	int bytes_received = 0, socket_id = 0;
	FILE *file;
	char buffer[BUF_SIZE];
	char command[BUF_SIZE];
	struct sockaddr_in server_socket;
	if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("ERROR opening socket.\n");
		return -1;
	}
	server_socket.sin_family = AF_INET;
	server_socket.sin_port = htons(PORT_NO); 
	server_socket.sin_addr.s_addr = INADDR_ANY;
	if (connect(socket_id, (struct sockaddr *)&server_socket, sizeof(server_socket)) == -1) {
		printf("ERROR connecting.\n");
		return -1;
	}
	printf("Input command: ");
	scanf ("%[^\n]%*c", command);
	printf("command: %s\n", command);
	if (send(socket_id, command, strlen(command), 0) == -1) {
		printf("Send failed.\n");
		return -1;
	}
	file = fopen("output.txt", "wb");
	while ((bytes_received = read(socket_id, buffer, BUF_SIZE)) > 0) {
		fwrite(buffer, 1, bytes_received, file);
	}
	if (bytes_received < 0)
		printf("Reading failed.\n");
	printf("Command output reading finished.\n");
	close(socket_id);
    	return 0;
}
