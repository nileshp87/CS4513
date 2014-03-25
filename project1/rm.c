#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h> 

#define _GNU_SOURCE

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
char *toAbsolute(char* file);

char* TRASH;
char* cwd;

int main(int argc, char* argv[])
{
	if(argc==1)
		usage(argv[0]);
	// Parse & Validate Command Line Arguments
	int mode = DEFAULT;
	int i;
	for(i = 1; i < argc; i++)
	{
		if(!strcmp("-h",argv[i]))
			usage(argv[0]);
		// Prevent weirdness like: "rm -r -f -r -f -f -r" from messing with modes
		else if(!strcmp("-f",argv[i]))
			mode += mode%2 == 0 ? FORCE : DEFAULT;
		else if(!strcmp("-r",argv[i]))
			mode += mode < 2 ? RECURSIVE : DEFAULT;
		else
			break;
	}
	// Get the trash variable
	TRASH = getenv("TRASH");
	if(!TRASH){
		puts("Error: TRASH variable not set, please set $TRASH to a created directory.");
		exit(1);
	}
	cwd = get_current_dir_name();
	printf("CWD: %s\n", cwd);
	printf("TRASH: %s\n", TRASH);
	printf("FD: %s\n", *(argv + i));
	toAbsolute(*(argv + i));
	rm(*(argv + i));
}

char *toAbsolute(char* file)
{
	if(!strncmp(file,"/",1))
		puts(file);
	else
		printf("%s/%s\n",cwd,file);
	return file;
}

int rm(char* file)
{
	char* newpath = calloc(strlen(file) + strlen(TRASH) + 1, sizeof(char));
	strcat(newpath, TRASH);
	strcat(newpath, file);
	int x = link(file, newpath);
	printf("Status: %d\n", x);
	int errsv = errno;
	printf("Status: %s\n", strerror(errsv));
	printf("NP: %s\n", newpath);
	return x;
}

void usage(char* name){
	printf("Usage: %s [OPTIONS]... FILES...\n\tRemove FILES (send to recycle bin)\n\tOptions:\n\t\t-h\tDisplay this help message\n\t\t-r\tRecursive, remove directories and their contents\n\t\t-f\tforce, removes files permanently\n", name);
	exit(0);
}