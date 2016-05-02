// This is a TCP echo server which echo back same msg as it recieve from the TCP client

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<errno.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>

#define serv_port 7777
#define BUFFER_SIZE_MAX 1024
#define BACKLOG_CONNECTIONS 10
int main()
{
	// Socket descriptors & struct definition for server code
	int sock = -1, clientSock = -1; 
	struct sockaddr_in server; 
	socklen_t serv_len = sizeof(server);
	struct sockaddr_in client;
	socklen_t cli_len = sizeof(client);
	char buffer[BUFFER_SIZE_MAX];
	int read_bytes = 0;
	int sent_bytes = 0;

	// Create socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Socket error: ");
		exit(-1);
	}

	// Initializing server address & port
	memset(&server, 0 , sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(serv_port);
	server.sin_addr.s_addr = INADDR_ANY;

	// Bind Server to IP and Port number
	if ((bind(sock, (struct sockaddr*)&server, serv_len)) == -1)
	{
		perror("Bind error: ");
		exit(-1);
	}

	// Listen to incoming client
	if (listen(sock, BACKLOG_CONNECTIONS) == -1)
	{
		perror("Listen error: ");
		exit(-1);
	}
	printf("Echo server is listening on the server port %d ... \n", serv_port);
	// Waiting for client connections
	while(1)
	{
		// Accept a new connection
		clientSock = accept (sock, (struct sockaddr*)&client, &cli_len);
		printf("A new client is connected to the server with IP '%s' and port '%d'\n", 
							inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		// Receive msg from the client
		read_bytes = recv(clientSock, buffer, BUFFER_SIZE_MAX, 0);
		if (read_bytes < 0)
		{
			perror("recv error: ");
			exit(-1);
		}
		printf("Message received from client:\n%s\n", buffer);
		
		// Echo back the message to the client
		sent_bytes = send(clientSock, buffer, BUFFER_SIZE_MAX, 0);
		if (sent_bytes < 0)
		{
			perror("send error: ");
			exit(-1);
		}
		printf("Sent '%d' bytes back to the client.\n", sent_bytes);
		close(clientSock);
		printf("Closed the client connection with IP '%s' and port '%d'.\n", 
				inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	}
	close(sock);
	return 0;
}
