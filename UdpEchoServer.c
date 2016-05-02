// This is a UDP echo server which responds to UDP client and prints the same msg back to UDP client as it receives.

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<errno.h>
#include<arpa/inet.h>
#include<string.h>

#define my_port 15000
#define MAX_BUF 1024

int main()
{
//initialization
	int sock; struct sockaddr_in server; 
	char buffer[MAX_BUF];	
	int cli;
	int n;
	struct sockaddr_in client;
	int len = sizeof(client);

//create socket
if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
{
	perror("socket error");
	exit(-1);
}

//initializing address & port
	memset(&server, 0, sizeof(server));	
	server.sin_family = AF_INET;
	server.sin_port = htons(my_port);
	server.sin_addr.s_addr = INADDR_ANY;

// bind to a port number & Ip
if (bind(sock, (struct sockaddr*)&server, sizeof(server)) == -1)
{
	perror("bind error");
	exit(-1);
}

	printf("connected to UDP server with IP: %s and port : %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

	printf("UDP Server ready.\n");
	memset(buffer, 0, MAX_BUF);

// receive the msg from the client
if ((recvfrom (sock,buffer, MAX_BUF ,0, (struct sockaddr*)&client, &len)) == -1)
{
	perror("UDP Receive failed:");
	exit(-1);
}
	printf("Received UDP data: %s\n", buffer);

	printf("Echoing UDP data to the client.\n");

//echo back msg to udp client

if ((sendto(sock, buffer, strlen(buffer) ,0, (struct sockaddr*)&client, sizeof(server))) == -1)
{	perror("UDP Send failed:");
	exit(-1);
}
	close(sock);
}
