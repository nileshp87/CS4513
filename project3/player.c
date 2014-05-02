#include "playerlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void printUsage(char* name);

int main(int argc, char* argv[])
{
	if(argc != 2 || !strncmp(argv[1], "-h"))
		printUsage(argv[0]);
	int file = open(argv[1], O_RDONLY);
	puts(argv[1]);
	playFile(file);
	close(file);
}

void printUsage(char* name)
{
	printf("Usage: %s [filename]\n", name);
	exit(0);
}