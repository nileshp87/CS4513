#include <stdio.h>
#include <stdlib.h>

// Errors
#define SUCCESS 0
#define E_PERMISSION 1
#define E_NONEXIST 2

// Modes
#define DEFAULT 0
#define RECURSIVE 2
#define FORCE 1
#define RECURSIVEFORCE 3

int rm(char* file);
int rmf(char* file);
int rmr(char* file);
int rmrf(char* file);
void usage(char* name);

int main(int argc, char* argv[])
{
	if(argc==1)
		usage(argv[0]);
	// Parse & Validate Command Line Arguments
	int mode = DEFAULT;
	int i;
	for(i = 1; i < argc; i++)
	{
		if(strcmp("-h",argv[i]) == 0)
			usage(argv[0]);
		// Prevent weirdness like: "rm -r -f -r -f -f -r" from messing with modes
		else if(strcmp("-f",argv[i]) == 0)
			mode += mode%2 == 0 ? FORCE : DEFAULT;
		else if(strcmp("-r",argv[i]) == 0)
			mode += mode < 2 ? RECURSIVE : DEFAULT;
		else
			break;
	}
	

}

void usage(char* name){
	printf("Usage: %s [OPTIONS]... FILES...\n\tRemove FILES (send to recycle bin)\n\tOptions:\n\t\t-h\tDisplay this help message\n\t\t-r\tRecursive, remove directories and their contents\n\t\t-f\tforce, removes files permanently\n", name);
	exit(0);
}