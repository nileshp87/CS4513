#include <stdio.h>
#include <stdlib.h>

#define USER nileshp
#define PASS apassword

void usage(char *name);

int main(int argc, char* argv[])
{
	int i;
	int port = 6013;
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
			sscanf(argv[i+1],"%d",&port);
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

	// Authentication:

	// Sending Command:

	// Receive and Display Response:

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
