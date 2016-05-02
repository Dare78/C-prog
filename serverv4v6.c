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
	struct sockaddr_in6 server, client; 
	socklen_t serv_len = sizeof(server);
	socklen_t cli_len = sizeof(client);
	char buffer[BUFFER_SIZE_MAX];
	int read_bytes = 0; 
	int on = 1;
	int sent_bytes = 0;
	char str[INET6_ADDRSTRLEN];

	if ((sock = socket(AF_INET6, SOCK_STREAM, 0)) == -1)
	{
		perror("IPV6 Socket error: ");
		exit(-1);
	}
	if ((setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
                   (int *)&on,sizeof(on))) < 0)
      	{
         perror("setsockopt(SO_REUSEADDR) failed");
		exit(0);         
      	}
	//Initialize server address & port for ipv6
	memset(&server, 0, sizeof(server));
      	server.sin6_family = AF_INET6;
      	server.sin6_port   = htons(serv_port);
	server.sin6_addr   = in6addr_any;

	// Bind Server to IP and Port number
	if ((bind(sock, (struct sockaddr*)&server, sizeof(server))) == -1)
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
       if ((clientSock = accept(sock, NULL, NULL)) < 0)
       {
         perror("accept() failed");
         //break;
       }
       else
       {
         /*****************************************************************/
         /* Display the client address.  Note that if the client is       */
         /* an IPv4 client, the address will be shown as an IPv4 Mapped   */
         /* IPv6 address.                                                 */
         /*****************************************************************/
         getpeername(clientSock, (struct sockaddr *)&client, &cli_len);
         if(inet_ntop(AF_INET6, &client.sin6_addr, str, sizeof(str))) {
            printf("Client address is %s\n", str);
            printf("Client port is %d\n", ntohs(client.sin6_port));
         }
		

	}
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
		buffer[BUFFER_SIZE_MAX] = '\0';
		printf("Sent '%d' bytes back to the client.\n", sent_bytes);
		close(clientSock);
	}
	close(sock);
	return 0;
}
