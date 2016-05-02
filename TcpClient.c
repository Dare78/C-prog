/*############################################################################### 
  This is the Client Code that send a message to the server. It also prints the 
  received message from the server, which is a echo of what message it sent. In
  addition to this the client also sends a timestamp with the sent message and
  also gets the time of receiving the echo message from server. Round trip time
  is calculated.
################################################################################*/
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/time.h>

#define ERROR -1
#define MAX_DATA 1024
#define PORT 7777
int main()
{

	int client_socket;
	struct sockaddr_in server_info;
	int lengthof_sockaddr;
	int rcv_datasize;
	struct timeval data_time;
	long int send_time, recv_time;
	char data[MAX_DATA], buffer[MAX_DATA];
	if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
	{
		printf("Client socket Error, Socket creation Failed!! \n");
		exit(-1);
	}
	memset(&server_info, 0, sizeof(server_info));
	server_info.sin_family = AF_INET;
	server_info.sin_port = htons(PORT);
	/* Binds to all available interfaces loacally i.e (eth0/lo) machines*/
	server_info.sin_addr.s_addr = INADDR_ANY;
	/* Zero padding of all unused structures */
	bzero(&server_info.sin_zero, 8);
	lengthof_sockaddr = sizeof(struct sockaddr_in);
	if (connect(client_socket, (struct sockaddr *)&server_info, sizeof(struct sockaddr)) < 0) 
	{
		printf("Connect to Server failed \n");
		exit(-1);
	}	
	while(1) 
	{
		printf("Client(Enter Data for Server)\t :\n");
		gets(data);
		gettimeofday(&data_time, NULL);
		printf("Client(Sending time)\t : %ld \n", data_time.tv_usec);
		send_time = data_time.tv_usec;
		if ((send(client_socket, data, strlen(data),0)) == ERROR) 
		{
			printf("Failure Sending Message\n");
			close(client_socket);
			exit(-1);
		}
		else 
		{
			rcv_datasize = recv(client_socket, data, sizeof(data),0);
			recv_time = gettimeofday(&data_time, NULL);
			if ( rcv_datasize <= 0 )
			{
				printf("Either Connection Closed or Error\n");
				//Break from the While
				break;
			}

			buffer[rcv_datasize] = '\0';
			printf("Client(Message Received From Server)\t:%s\n", data);
			printf("Client(Time received from Server)\t:%ld\n", data_time.tv_usec);
			/*Calculating the round trip time*/
			printf("Client(Round Trip Time(in microseconds)\t:%ld\n", data_time.tv_usec-send_time);   
		}	
	}
	close(client_socket);

}
