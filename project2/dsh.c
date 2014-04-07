#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h> 
#include <string.h> 
#include <errno.h>
#include <unistd.h>


const char* username = "nileshp";
const char* password = "apassword";

void usage(char *name);
void breakdownURL(char* URL);
int receiveint(int *num, int fd);

int main(int argc, char* argv[])
{
	int i;
	unsigned short int port = 6013;
	char* host = 0;
	char* command = 0;
	// Command Line Processing and Verification:
		if(argc == 1)
			usage(argv[0]);	
		for(i = 1; i < argc-1; i++)
		{
			if(!strcmp(argv[i],"-h"))
				usage(argv[0]);
			else if(!strcmp(argv[i],"-c"))
			{
				if(!strncmp(argv[i+1],"-",1))
					usage(argv[0]);
				command = argv[i+1];
				i++;
			}
			else if(!strcmp(argv[i],"-s"))
			{
				if(!strncmp(argv[i+1],"-",1))
					usage(argv[0]);
				host = argv[i+1];
				i++;
			}
			else if(!strcmp(argv[i],"-p"))
			{
				if(!strncmp(argv[i+1],"-",1))
					usage(argv[0]);
				sscanf(argv[i+1],"%hu",&port);
				i++;
			}
			else
			{
				fprintf(stderr, "Bad argument specified: %s\n", argv[i]);
				usage(argv[0]);
			
			}	
		}
		if(!host || !command)
		{
			fprintf(stderr, "Missing required option: %s\n", host ? "-c command" : "-h host" );
			usage(argv[0]);
		}
		
	// Initializing Network:
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

	// Authentication:
		char* recvBuffer = malloc(1024);
		// Get Number: 
			int authNum;
		    write(sd, username,strlen(username));
		   	read(sd, recvBuffer, 1024);
		   	sscanf(recvBuffer, "%d", &authNum);

	    // Encryption:
		    char* salt = malloc(33);
		    sprintf(salt, "%d%d",authNum,authNum);
		    char* hash = crypt(password, salt);
			if(!hash)
				fprintf(stderr,"ERROR: %s\n", strerror(errno));
		    write(sd, hash,strlen(hash));
		    read(sd, recvBuffer, 4);
		   	sscanf(recvBuffer, "%d", &authNum);
		    if(!authNum)
		    {
		    	fprintf(stderr,"ERROR: Bad Username or Password\n");
		    	exit(1);
			}

	// Sending Command:
		write(sd, command, strlen(command));
	// Receive and Display Response:
		while(read(sd, recvBuffer, 1024) > 0)
		printf("%s", recvBuffer);
		puts("");
	// Cleanup: 
    	close(sd);
    	free(recvBuffer);

}

void usage(char *name)
{
	printf("Usage: %s [flags] <command>, where flags are:\n", name);
	puts("\t{-c command}\tcommand to execute remotely");
	puts("\t{-s host}\thost server to execute command on");
	puts("\t[-p #]\tport server is on (default is 6013)");
	puts("\t[-h]\tthis help message\n");
	exit(0);
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

int receiveint(int *num, int fd)
{
    int32_t ret;
    char *data = (char*)&ret;
    int left = sizeof(ret);
    int rc;
    while (left) {
        ret = read(fd, data + sizeof(ret) - left, left);
        if (ret < 0) return -1;
        left -= ret;
    }
    *num = ret;
    return 0;
}