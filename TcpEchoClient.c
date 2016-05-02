
//This is a TCP echo client code which connects to TCP echo server and calulates the Round trip time 
// by noting the sending time while sending the msg and receiving time while receiving the echo msg from TCP server.

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<arpa/inet.h>

#define server_port 7777
#define local_port 12000
#define BUFFER_SIZE_MAX 1024

int main()
{
	// Initilization
	int sock;
	struct sockaddr_in server;
	int serv_len = sizeof(server);
	char buffer[BUFFER_SIZE_MAX];
	struct timeval time1, time2;
	int read_bytes = 0, sent_bytes = 0;
	long int RTT_in_seconds = 0;
	long int RTT_in_microSeconds = 0;
	long int t1_timeInSeconds;
	long int t1_timeInMicroSeconds;
	long int t2_timeInSeconds;
	long int t2_timeInMicroSeconds;

	// Create socket
	if((sock = socket (AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error: ");
		exit(-1);
	}

	memset(&server, 0, sizeof(server));  // clear the memory before connect
	server.sin_family = AF_INET;
	server.sin_port = htons(server_port);
	server.sin_addr.s_addr = INADDR_ANY;

	// connect to server
	printf("Connecting to the echo server on port %d ... \n", server_port);
	if ((connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in))) == -1)
	{
		perror("connect error:");
		exit(-1);
	} 
	printf("Enter the message to echo server:");
	//scanf("%s", buffer);
	gets(buffer);
	
	// Generate a timestamp before sending an echo message.
	gettimeofday(&time1, NULL);
	t1_timeInSeconds = time1.tv_sec;
	t1_timeInMicroSeconds = time1.tv_usec;
	printf("TimeStamp before sending: %ld seconds, %ld microSeconds\n", t1_timeInSeconds, t1_timeInMicroSeconds);
	
	// Send msg to the sever
	sent_bytes = send(sock, buffer, BUFFER_SIZE_MAX, 0);
	if (sent_bytes < 0)
	{
		perror("send error:");
		exit(-1);
	}

	// Receive echo msg from server
	read_bytes = recv(sock, buffer, BUFFER_SIZE_MAX, 0);
	if (read_bytes < 0)
	{
		perror("receive error:");
		exit(-1);
	}
	printf("Echo server reply: %s\n", buffer);
	
	// Generate a timestamp before sending an echo message.
	gettimeofday(&time2, NULL);
	t2_timeInSeconds = time2.tv_sec;
	t2_timeInMicroSeconds = time2.tv_usec;
	printf("TimeStamp after receiving: %ld seconds, %ld microSeconds\n", t2_timeInSeconds, t2_timeInMicroSeconds);

	// Calculate the round trip time and print it
	RTT_in_seconds = t2_timeInSeconds - t1_timeInSeconds;
	RTT_in_microSeconds = t2_timeInMicroSeconds - t1_timeInMicroSeconds;
	printf("The total RTT in seconds is: %ld\n", RTT_in_seconds);
	printf("The total RTT in microseconds is: %ld\n", RTT_in_microSeconds);
	return(0);
}



