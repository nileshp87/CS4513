#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h> 
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>	
#include <dirent.h>
#include <fcntl.h>
#include <utime.h>


#define _GNU_SOURCE

// Errors
#define SUCCESS 0
#define E_PERMISSION 1
#define E_NONEXIST 2


int rmrf(char* file, int end);
void usage(char* name);
char *toAbsolute(char* file);
int mov(char* src, char* dst, char* suffix);
int rmf(char* file);

char* TRASH;

int main(int argc, char* argv[])
{
	if(argc==2)
		usage(argv[0]);

	// Get the trash variable
	TRASH = getenv("TRASH");
	if(!TRASH){
		fprintf(stderr,"Error: TRASH variable not set, please set $TRASH to a created directory.");
		exit(1);
	}
	// Get the current working directory
	struct stat* buff = malloc(sizeof(struct stat));
	int s = stat(TRASH, buff);
	if(s)
	{
		fprintf(stderr, "ERROR: Couln't stat file: %s\n\t%s\n", TRASH, strerror(errno));
		return EACCES == errno ? E_PERMISSION : E_NONEXIST;
	}
	rmrf(TRASH, 0);
	if(S_ISDIR(buff->st_mode))
	{
		s = mkdir(TRASH, buff->st_mode);
		if(!s)
			chmod(TRASH, buff->st_mode);
		else{
			fprintf(stderr, "ERROR: %s\n", strerror(errno));
			return EACCES == errno ? E_PERMISSION : E_NONEXIST;
		}
	}
	free(buff);

}


/** function: toAbsolute
  * params: file to get absolute path of
  * returns: absolute path to given file
**/
char *toAbsolute(char* file)
{

	if(!strncmp(file,"/",1))
		return file;
	char* abs = calloc(strlen(file)+strlen(TRASH)+2,sizeof(char));
	strcat(abs, TRASH);
	strcat(abs, "/");
		strcat(abs, file);
	return abs;
}



int rmrf(char* file, int end)
{
	char* oldPath = toAbsolute(file);
	char* orig = file;
	char* base = basename(file);
	struct stat* buff = malloc(sizeof(struct stat));
	int i = stat(oldPath, buff);
	if(i)
	{
		fprintf(stderr, "ERROR: Couln't stat file: %s\n\t%s\n", oldPath, strerror(errno));
		return EACCES == errno ? E_PERMISSION : E_NONEXIST;
	}
	if(S_ISDIR(buff->st_mode))
	{
		DIR *dp;
		struct dirent *d;

		dp = opendir(oldPath);
		if (dp == NULL) {
			perror("open");
			exit(1);
		}
		char* nPCP = malloc(sizeof(oldPath) + 250);
		strcpy(nPCP, oldPath);
		strcat(nPCP, "/");
		char* suffix = nPCP+strlen(oldPath)+1;
		end = end == 0 ? strlen(oldPath) - strlen(base) : end;
		while (d = readdir(dp)) {
			if(!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
				continue;
			strcpy(suffix,d->d_name);
			if(d->d_type == 4){
				rmrf(nPCP, end);
				continue;
			}
			char* sop = malloc(strlen(file) + strlen(d->d_name) + 1);
			strcpy(sop, file);
			strcat(sop,"/");
			strcat(sop, d->d_name);
			int l = unlink(sop);
			if(l){
				fprintf(stderr, "ERROR: %s\n", strerror(errno));
				return errno == ENOENT ? E_NONEXIST : E_PERMISSION;
			}
			free(sop);

		}
		closedir(dp);
		rmdir(oldPath);
		return SUCCESS;
	}
	rmf(oldPath);
}


int rmf(char* file)
{
	char* oldPath = toAbsolute(file);
	int l = unlink(oldPath);
	if(l){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return errno == ENOENT ? E_NONEXIST : E_PERMISSION;
	}
	if(oldPath == file)
		free(oldPath);
	return SUCCESS;
}

void usage(char* name){
	printf("Usage: %s [OPTIONS]\n\tEmpty the trash\n\tOptions:\n\t\t-h\tDisplay this help message\n", name);
	exit(0);
}