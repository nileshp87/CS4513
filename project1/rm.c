#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h> 
#include <libgen.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

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
	// Get the current working directory
	cwd = get_current_dir_name();
	if(mode == 0)
	{
		for(; i < argc; i++){
			rm(*(argv + i));
		}
	}
	if(mode == 1)
	{
		for(; i < argc; i++){
			rmf(*(argv + i));
		}	
	}
}


/** function: toAbsolute
  * params: file to get absolute path of
  * returns: absolute path to given file
**/
char *toAbsolute(char* file)
{

	if(!strncmp(file,"/",1))
		return file;
	char* abs = calloc(strlen(file)+strlen(cwd)+2,sizeof(char));
	strcat(abs, cwd);
	strcat(abs, "/");
	strcat(abs, file);
	return abs;
}

int rm(char* file)
{
	char* oldPath = toAbsolute(file);
	char* orig = file;
	char* base = basename(file);
	char* newPath = calloc(strlen(TRASH) + strlen(base) + 5, sizeof(char));
	char* suffix = newPath+(strlen(TRASH)+strlen(base) + 1);

	strcat(newPath, TRASH);
	strcat(newPath, "/");
	strcat(newPath, base);

	printf("rm %s -> %s\n", oldPath, newPath);
	int l = link(oldPath, newPath);
	int i = 0;
	strcpy(suffix, ".0");
	while(l == -1 && errno == EEXIST)
	{
		if(*(suffix)+1 == '9')
		{
			*(suffix+1) = '0';
			suffix++;
		}
		(*(suffix+1))++;
		l = link(oldPath, newPath);
	}
	if(l){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return errno == ENOENT ? E_NONEXIST : E_PERMISSION;
	}
	if(!l)
		l = unlink(oldPath);
	if(l){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return errno == ENOENT ? E_NONEXIST : E_PERMISSION;
	}
	free(newPath);
	if(oldPath == orig)
		free(oldPath);
	return SUCCESS;
}

int rmf(char* file)
{
	char* oldPath = toAbsolute(file);
	printf("rm -f %s \n", oldPath);
	int l = unlink(oldPath);
	if(l){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return errno == ENOENT ? E_NONEXIST : E_PERMISSION;
	}
	if(oldPath == file)
		free(oldPath);
	return SUCCESS;
}

int rmr(char *file)
{
	char* oldPath = toAbsolute(file);
	char* orig = file;
	char* base = basename(file);
	char* newPath = calloc(strlen(TRASH) + strlen(base) + 5, sizeof(char));
	char* suffix = newPath+(strlen(TRASH)+strlen(base) + 1);

	strcat(newPath, TRASH);
	strcat(newPath, "/");
	strcat(newPath, base);

}

void usage(char* name){
	printf("Usage: %s [OPTIONS]... FILES...\n\tRemove FILES (send to recycle bin)\n\tOptions:\n\t\t-h\tDisplay this help message\n\t\t-r\tRecursive, remove directories and their contents\n\t\t-f\tforce, removes files permanently\n", name);
	exit(0);
}