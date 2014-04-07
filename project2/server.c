#define _XOPEN_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <sys/time.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>

//Function prototypes
void printUsage();
void handleClient(int sd);
void processRequest(int sd, char* request);
void cleanup();

int sd;
const char* username = "nileshp";
const char* password = "apassword";

int main(int argc, char* argv[]){
	srand(time(NULL));
	int port = 6013;
	if(argc == 2)
	{
		if(!sscanf(argv[1],"%d",&port)){
			fprintf(stderr,"Error in server: Port was not a number.\n");
			printUsage();
			return;
		}
	}
	puts("Starting server...");
	if(0 > (sd = socket(AF_INET, SOCK_STREAM, 0))) { // get socket int
		puts("Error creating socket.");
		fprintf(stderr,"\n Error : Connect Failed \nReason: %s\n", strerror(errno));
		return;
	}
	atexit(cleanup); // now that socket has been created, at exit is set.
	struct sockaddr_in saddr;
	memset(&saddr,'\0',sizeof(saddr)); // zero structure out
	saddr.sin_family = AF_INET; // match the socket() call
	saddr.sin_addr.s_addr = htonl(INADDR_ANY); // bind to any local addr
	saddr.sin_port = htons(port); // specify port to listen on
	if((bind(sd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0)){ // bind!
		fprintf(stderr,"Error in server: Error binding\nReason: %s\n", strerror(errno));
		return;
	}
	if(listen(sd,5)!=0){ // listen for connections
		fprintf(stderr,"Error in server: Error listening\nReason: %s\n", strerror(errno));
		return;
	}
	struct sockaddr clientaddr; // places for accept to put data (not actually used)
	socklen_t addrlen;
	int clientsd;
	struct pollfd fds; // Polling stuff
	int ret;
	fds.fd = sd;
	fds.events = POLLIN;
	printf("Server successfully started on port: %d\n",port);
	// Infinite loop for server
	while(1){
		poll(&fds,1,-1); // Poll the socket
		if(~(clientsd = accept(sd, &clientaddr, &addrlen))){ // Handle the request
			puts("Connection request received...");
			int childPID = fork();
			if(childPID == 0){
				puts("\tforked child and sent to handler...");
				handleClient(clientsd);
			}
		}
	}
}
/** Prints the usage to stdout.
*/
void printUsage(){
	puts("Usage: ./server [PORT]");
}

void handleClient(int csd){
	printf("\tClient connected via socket %d.\n", csd);
	int n;
	char* buffer = malloc(1024); // buffer for reading
	char* fullRequest = malloc(1024); // final buffer
	char* current = fullRequest; // tracker on where the buffer should copy to
	memset(buffer, '0',sizeof(buffer)); // zero out buffer
	memset(fullRequest, '0', sizeof(fullRequest)); // zero out final buffer
	while((n = read(csd,buffer,sizeof(buffer)-1))>0) // read the request in
	{
		buffer[n] = 0; // set last character to 0
		if(fullRequest + sizeof(fullRequest) < current + n){ // if the request will overflow the buffer make it bigger
			int size = current - fullRequest;
			fullRequest = realloc(fullRequest, current + n - fullRequest);
			current = fullRequest + size;
			
		}
		memcpy(current, buffer, n+1); // dump the buffer into the final buffer
		current+=n; // increment the dump position
		if(!strcmp(buffer, username))
			break;
	}
	printf("\tUser: %s\n", fullRequest);

	int authNum = rand();
	char* num = malloc(33);
	sprintf(num, "%d", authNum);
	write(csd, num, strlen(num) );
	//memset(fullRequest, '0',strlen(fullRequest)); 
	
	char* hash = crypt(password, num);
	read(csd, fullRequest, strlen(hash));
	//printf("My Hash: %s\n", hash);

	//printf("Received Hash: %s\n", fullRequest);

	write(csd, strncmp(hash, fullRequest, strlen(hash)) ? "0" : "1", 2);
	if(strncmp(hash, fullRequest, strlen(hash)))
		return;
	else
		puts("\tPasswords matched, authenticated.");
	current = fullRequest;

	n = read(csd,buffer,1024);
	buffer[n] = 0;
	printf("\tReceived Command: %s\n", buffer);
		int childPID = fork();
	if(childPID == 0){

		char* frd = strdup((const char *)buffer);
		char* argument;
		char* program = strtok(frd, " ");
		int length = 40;
		int numArgs = 1;
		int i;
		char* command = (char*) calloc(length, sizeof(char));
		char** arguments = (char**) calloc(numArgs, sizeof(char*));
		arguments[0] = program;
		for(i = 1; argument = strtok(NULL, " "); i++){
			if(i == numArgs){
				numArgs *= 1.3;
				arguments = realloc(arguments, numArgs * sizeof(char *));
				if(!arguments)
					puts("Error: Could not allocate enough space for arguments");
			}
			arguments[i] = argument;
		}
		//write(csd, "stuff", 5);
		//close(0);
		puts("\tExecuting command...");
		fflush(stdout);
		dup2(csd, 1);
		dup2(csd, 2);
		execvp(program, arguments);
		exit(1);
	}
	int status;
    wait(&status);
    shutdown(csd, SHUT_RDWR);
    if(status == 256)
    	puts("Couldn't find program");

	free(buffer); // free buffer
	free(fullRequest); // free final buffer
}

/** Cleanup function called on exit to ensure the port is not held up.
*/
void cleanup(){
	shutdown(sd, SHUT_RDWR); // Shutdown reading a writing on socket, to ensure port is immediately available.
	if(close(sd)<0){ // Close socket and check for errors, print out message if there are
		puts("Error closing socket.");
		printf("Reason: %s\n", strerror(errno));
	}
}