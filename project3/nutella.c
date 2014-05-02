#include "playerlib.h"
#include "msock.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <dirent.h>
#include <sys/types.h>

#define QUERYPORT 7175
#define QUERYGROUP "239.70.65.71"
#define RESPPORT 7176
#define RESPGROUP "239.70.65.72"

void printUsage(char* name);
void startServer();
void startClient();
char* readInput(char* buffer, int* length);
void breakdownURL(char* URL);
void bindServer(char* name);

int main(int argc, char* argv[]){
	int mode = 0;
	if(argc != 2)
		printUsage(argv[0]);
	if(!strncmp(argv[1], "-c", 2))
		mode = 'c';
	if(!strncmp(argv[1], "-s", 2))
		mode = 's';
	if(!mode)
		printUsage(argv[0]);
	if(mode == 's')
		startServer();
	if(mode == 'c')
		startClient();
	return 1;
}

void printUsage(char* name)
{
	printf("Usage: %s [OPTION]\n", name);
	puts("\tOptions:\n\t\t-s\tServer Mode\n\t\t-c\tClient Mode\n");
	exit(0);
}

void startServer(){
	puts("Server Mode Starting...");
	char* movieDir = getenv("NUTELLA");
	if(!movieDir){
		fprintf(stderr,"Error: NUTELLA variable not set, please set $NUTELLA to a movie directory.\n");
		exit(1);
	}

	char hostname[1028];
	hostname[1023] = '\0';
	gethostname(hostname, 1023);
	strcat(hostname, ":7777");
	int querySock = msockcreate(RECV, QUERYGROUP, QUERYPORT);
	int responseSock = msockcreate(SEND, RESPGROUP, RESPPORT);
	if(querySock == -1){
		fprintf(stderr, "Error: Could not create query socket.");
		exit(-1);
	}
	if(responseSock == -1){
		fprintf(stderr, "Error: Could not create response socket.");
		exit(-1);
	}
	char* query = calloc(256, sizeof(char));
	int receivedLength;
	int port = 7777;
	DIR *dp;
	struct dirent *d;

	while(1)
	{
		puts("Now waiting for queries...");
		receivedLength = mrecv(querySock, query, 256);
		printf("Received Query for: %s\n", query);

		dp = opendir(movieDir);
		if (dp == NULL) {
			fprintf(stderr, "ERROR: Couldn't open movie directory.\n");
			exit(-1);
		}

		d = readdir(dp);
		while (d) {
			if(!strcmp(d->d_name, query))
				break;
			d = readdir(dp);
		}
		closedir(dp);
		if(d)
		{
			msend(responseSock, hostname, strlen(hostname));
			printf("Movie Found! Sending  IP and port: %s\n", hostname);
			char* file = malloc(strlen(query) + strlen(movieDir) + 1);
			strcpy(file, movieDir);
			strcat(file, "/");
			strcat(file, query);
			bindServer(file);
		}else{
			puts("Movie not found.");
		}
		d =NULL;
		memset(query, 0, 256);
	}
}

void startClient(){
	puts("Client Mode Selected...");
	int length = 128;
	char* movieName = (char*) calloc(length, sizeof(char));
	char* respBuff;
	short int port;
	int receivedLength = 0;
	int i = 0;
	struct timeval timeout;      
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	while(1)
	{
		respBuff = calloc(1024, sizeof(char));
		memset(movieName,0, 128 * sizeof(char));
		printf("Enter movie name: ");
		movieName = readInput(movieName,&length);
		int querySock = msockcreate(SEND, QUERYGROUP, QUERYPORT);
		int responseSock = msockcreate(RECV, RESPGROUP, RESPPORT);
		msend(querySock, movieName, strlen(movieName));
	    if (setsockopt (responseSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
            sizeof(timeout)) < 0)
        error("setsockopt failed\n");
		receivedLength = mrecv(responseSock, respBuff, 1024);
		msockdestroy(querySock);
		msockdestroy(responseSock);
		if(receivedLength == -1)
		{
			puts("Movie not found on any listening servers...");
			free(respBuff);
			continue;
		}
		i = 0;
		while( respBuff[i] != ':')
			i++;
		char* host = calloc(i+1, sizeof(char));
		strncpy(host, respBuff, i);
		i++;
		sscanf(respBuff+i, "%hu", &port);
		puts("Server and socket received!");
		struct hostent *hostname = gethostbyname(host); // Convert server name into 32bit ip
		breakdownURL(host);
		int sd;
		if(0 > (sd = socket(AF_INET, SOCK_STREAM, 0))) { // Create the socket to the server
			puts("Error creating socket.");
			printf("\n Error : Connect Failed \nReason: %s\n", strerror(errno));
		}
		struct sockaddr_in serv_addr;
		memset(&serv_addr, '\0', sizeof(serv_addr)); 
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(port);
		if(inet_pton(AF_INET, host, &serv_addr.sin_addr)<=0)
	    {
	        fprintf(stderr, "ERROR: %s\n", strerror(errno));
	        exit(1);
	    }
		if( connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	    {
	       fprintf(stderr, "ERROR: Connect Failed \nReason: %s\n", strerror(errno));
	       exit(1);
	    }
	    char* recvBuffer = calloc(1024, sizeof(char));
	    int c = 0;
		while(read(sd, recvBuffer, 1) > 0){
			if(c == 0 && recvBuffer[0] == '\n'){
				c++;
			}
			else if(c == 1 && recvBuffer[0] == 'd'){
				c++;
			}
			else if(c == 2 && recvBuffer[0] == 'o'){
				c++;
			}
			else if(c == 3 && recvBuffer[0] == 'n'){
				c++;
			}
			else if(c == 4 && recvBuffer[0] == 'e'){
				c++;
			}
			else if(c == 5 && recvBuffer[0] == '\n')
			{
				break;
			}
			printf("%s", recvBuffer);
			fflush(stdout);
		}
		puts("");
		free(recvBuffer);
		free(respBuff);

	}

}



char* readInput(char* buffer, int* length){
	char c;
	int read = 0;
	while(1){
		if(read == *length){
			(*length) *= 1.3;
			buffer = realloc(buffer, (*length) * sizeof(char));
			if(!buffer)
			{
				fprintf(stderr, "Could not allocate enough memory to hold command\n");
				exit(-1);
			}
		}
		c = (char)getchar();
		if(c=='\n'){
			buffer[read] = '\0';
			break;
		}
		buffer[read++] = c;
	}
	return buffer;
}

void breakdownURL(char* URL){
	int a,b,c,d;
	int length = 1;
	if(sscanf(URL,"%d.%d.%d.%d",&a,&b,&c,&d)==4) // if the URL is an ip
	{
		// If statements calculate length of IP, so characters after can be file
		if(a>10) 
			length++;
		if(a>100)
			length++;
		if(b>10)
			length++;
		if(b>100)
			length++;
		if(c>10)
			length++;
		if(c>100)
			length++;
		if(d>10)
			length++;
		if(d>100)
			length++;
		URL[length+6] = '\0'; // termination character
	}
	else{ 
		length = strlen(URL)+1;
		struct hostent *he;
		struct in_addr **addr_list;
		URL[length] = '\0';
		if ((he = gethostbyname(URL)) == NULL) {  // get the host info
			herror("gethostbyname");
			return;
		}
		addr_list = (struct in_addr **)he->h_addr_list;
		strcpy(URL, inet_ntoa(*addr_list[0]));
	}
	
}


void bindServer(char* name)
{
	int sock, serv_host_port, clilen, newsock;
   struct sockaddr_in cli_addr, serv_addr;

   serv_host_port = atoi("7777");
	struct timeval timeout;      
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
   printf("Listen activating.\n");

   printf("Trying to create socket at port %d...\n", serv_host_port);   
   /* create socket from which to read */
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("creating socket");
      exit(1);
   }
	int opt = 1;
   setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
   /* bind our local address so client can connect to us */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(serv_host_port);
   if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("can't bind to local address");
      exit(1);
   }
   setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
   /* mark socket as passive, with backlog num */
   if (listen(sock, 1) == -1) {
     perror("listen");
     exit(1);
   }
   setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
  if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
        sizeof(timeout)) < 0)
   printf("Socket ready to go! Accepting connections....\n\n");

   /* wait here (block) for connection */
   clilen = sizeof(cli_addr);
   newsock = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
   if (newsock < 0) {
     puts("Client did not connect, resuming query handling...");
     return;
   }
   	int file = open(name, O_RDONLY);
   	fflush(stdout);
   	int pdup = dup(1);
	dup2(newsock, 1);
	playFile(file);
	close(file);
   /* close connected socket and original socket */
	shutdown(newsock,  SHUT_RDWR);
   close(newsock);
   shutdown(sock,  SHUT_RDWR);
   close(sock);
   dup2(pdup, 1);
   puts("Movie sent, resuming query handling...");
}