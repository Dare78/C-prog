#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SYS_ERROR -1
#define HTTP_URL_LEN_MAX 512
#define HOSTNAME_LEN_MAX 128
#define RESOURCE_LEN_MAX 128
#define BUFFER_SIZE_MAX 1024
#define REQUEST_BUF_SIZE_MAX 512
#define RESPONSE_BUF_SIZE_MAX 1024
#define HTTP_PORT 80

main(int argc, char** argv)
{
	int sock = -1, i = 0;
	struct sockaddr_in remote_server;
	char *http_response_data = NULL;
	int http_url_len = 0, resourceLocationLen = 0;
	struct hostent *server;
	int total_data_recvd = 0;
	int rcvd_bytes = 0;
	char http_url[HTTP_URL_LEN_MAX];
	char hostName[HOSTNAME_LEN_MAX];
	char resource_location[RESOURCE_LEN_MAX];
	char request_msg[REQUEST_BUF_SIZE_MAX];
	char response_msg[RESPONSE_BUF_SIZE_MAX];
	int hostIndex = 0, resourceLocIndex = 0;
	char *ptr = NULL;

	if (argc != 2) {
		printf("Error - Invalid no. of arguments specified.\n");
		printf("Usage: ./httpClient <serverUrl>\n");
		exit(SYS_ERROR);
	}

	http_response_data = (char*)malloc(BUFFER_SIZE_MAX);
	if(NULL == http_response_data)
	{
		perror("Memory allocation failed: ");
		exit (SYS_ERROR);
	}

	strncpy(http_url, argv[1], HTTP_URL_LEN_MAX);
	printf("HTTP URL: %s\n", http_url);
	http_url_len = strlen(http_url);

	int beginHostAddress = 0;
	int j = 0, k = 0;
	int index = 0;

	while ((index < http_url_len) && (http_url[index] != '/')) 
	{
		index++;
	}
	if ((http_url[index] == '/') && (http_url[index+1] == '/'))
	{
		index += 2;	
		hostIndex = index;
	}
	
	if (hostIndex)
	{
		while ((index < http_url_len) && (http_url[index] != '/')) 
		{
			index++;
		}
	}

	/* Extract the host name */
	memset(hostName, 0, HOSTNAME_LEN_MAX);
	ptr = &http_url[hostIndex];
	strncpy(hostName, ptr, index-hostIndex);
	printf("HostName: %s\n", hostName);
	
	/* Extract the resource path */
	resourceLocIndex = index;
	memset(resource_location, 0, RESOURCE_LEN_MAX);
	ptr = &http_url[resourceLocIndex];
	strncpy(resource_location, ptr, RESOURCE_LEN_MAX);
	printf("Resource Location: %s\n", resource_location);

	if((sock = socket(AF_INET,SOCK_STREAM,0))== SYS_ERROR)
	{
		perror("Can't create TCP socket: ");
		exit(SYS_ERROR);
	}
	
	memset(request_msg, 0, REQUEST_BUF_SIZE_MAX);
	sprintf(request_msg, "GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n\r\n", resource_location, hostName, HTTP_PORT);
	server = gethostbyname(hostName);
	if (server == NULL)
	{ 
		perror("SYS_ERROR, No such host: ");
		exit(SYS_ERROR);
	}
	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons(HTTP_PORT);
	memcpy(&remote_server.sin_addr.s_addr,server->h_addr,server->h_length);
	bzero(&remote_server.sin_zero, sizeof(struct sockaddr_in));

	if((connect(sock,(struct sockaddr*)&remote_server, sizeof(struct sockaddr_in))) == SYS_ERROR )
	{
		perror("connection failed :");
		exit(SYS_ERROR);
	}

	int sent_bytes = 0;
	printf("Sending HTTP URL Request:\n%s\n", request_msg);
	
	sent_bytes = send(sock, request_msg, strlen(request_msg), 0);
	if (sent_bytes < 0)
	{
		perror("Send error: ");
		exit(SYS_ERROR);
	}
	printf("Request bytes: %d\n", sent_bytes);
	printf("Receiving HTTP server response ...\n");
	while (1) {
		memset(response_msg, 0, RESPONSE_BUF_SIZE_MAX); 	
		rcvd_bytes = recv(sock, response_msg, RESPONSE_BUF_SIZE_MAX-1, 0);
		total_data_recvd += rcvd_bytes;
		printf("Received %d bytes.\n", rcvd_bytes);
		printf("%s", response_msg);
		if (strstr(response_msg, "</html>")) {
			break;
		}
	}
	printf("HTTP server fetch completed and read %d bytes succesfully.\n", total_data_recvd);	
	close(sock);
	return 0;	
}



