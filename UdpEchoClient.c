// This is a UDP echo client code which connects to UDP echo server and calulates the Round trip time 
// by noting the sending time while sending the msg and receiving time while receiving the echo msg from UDP server.

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<errno.h>
#include<arpa/inet.h>
#include<string.h>

#define server_port 15000
#define MAX_BUF 1024

main()
{
//initialization
	int sock, cli;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char buffer[MAX_BUF];
	int len= sizeof(server);

	char input[MAX_BUF];
	char output[MAX_BUF];
	long int t1,t2,t3,t4, t_out, RTTtime;
	struct timeval start, end;

//create socket
if ((sock=socket(AF_INET,SOCK_DGRAM,0))== -1)
{
	perror("socket error:");
	exit(-1);
}
	puts("socket created");
	memset(&server, 0, sizeof(server)); 
	server.sin_family = AF_INET;
	server.sin_port = htons(server_port);
	memset(buffer,0, MAX_BUF);
	printf("Enter the message to server:");
	scanf("%s", buffer);

// send the msg to the server
if ((sendto(sock, buffer, MAX_BUF, 0, (struct sockaddr*)&server, sizeof(server)))== -1)
{
	perror("UDP Send failed:");
	exit(-1);
}

// Note the time while sending the msg to server
gettimeofday(&start, NULL);
	t1=start.tv_sec;
	t2=start.tv_usec;
	sprintf(input, "%ld %ld", t1,t2);

printf("The time stamp at client has second value of : %ld and a microsecond of :%ld\n",t1,t2);


// receive the echo msg from the server
if ((recvfrom(sock, buffer, strlen(buffer) , 0, (struct sockaddr*)&server, &len)) == -1)
{
	perror("UDP receive failed:");
	exit(-1);
}

// note the time after receiving the echo msg
 	t_out= atoi(output);
	gettimeofday(&end, NULL);
	t3 = end.tv_sec;
	t4= end.tv_usec;
	sprintf(output, "%ld %ld", t3,t4);
	printf("The time stamp at server has second value of : %ld and a microsecond of :%ld\n",t3,t4);

// calculate the RTT and print it

	RTTtime= (t4 - t2) ;

	printf(" The total RTT is: %ld\n", RTTtime);


}


