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

// Modes
#define DEFAULT 0
#define RECURSIVE 2
#define FORCE 1
#define RECURSIVEFORCE 3

int rm(char* file);
int rmf(char* file);
int rmr(char* file, int end);
int rmrf(char* file, int end);
void usage(char* name);
char *toAbsolute(char* file);
int mov(char* src, char* dst, char* suffix);



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
		fprintf(stderr,"Error: TRASH variable not set, please set $TRASH to a created directory.");
		exit(1);
	}
	// Get the current working directory
	cwd = get_current_dir_name();
	if(mode == 0)
	{
		for(; i < argc; i++){
			if(rm(*(argv + i)))
				exit(1);
		}
	}
	if(mode == 1)
	{
		for(; i < argc; i++){
			if(rmf(*(argv + i)))
				exit(1);
		}	
	}
	if(mode == 2)
	{
		for(; i < argc; i++){
			if(rmr(*(argv + i), 0))
				exit(1);
		}
	}
	if(mode ==3)
	{
		for(; i < argc; i++){
			if(rmrf(*(argv + i), 0))
				exit(1);
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

int mov(char* src, char* dst, char* suffix)
{
	struct stat* sbuff = malloc(sizeof(struct stat));
	int i = stat(src, sbuff);
	if(i)
	{
		fprintf(stderr, "ERROR: Couln't stat file (mov): %s\n\t%s\n", src, strerror(errno));
		return EACCES == errno ? E_PERMISSION : E_NONEXIST;
	}
	int l = link(src, dst);
	if(errno == EEXIST)
		strcpy(suffix, ".0");
	while(l == -1 && errno == EEXIST)
	{
		(*(suffix+1))++;
		l = link(src, dst);
	}
	if(l && errno == EXDEV)
	{
	    int infile, outfile;

        infile = open(src,O_RDONLY);
        if(infile<0){
                fprintf(stderr, "Could not open: %s\n",src);
                return E_PERMISSION;
        }

        outfile = open(dst,O_WRONLY | O_CREAT);
        if(outfile<0){
                fprintf(stderr,"Could not open: %s\n",dst);
                return E_PERMISSION;
        }
        int bytes_read;
        unsigned char *buff = malloc(4096);
        while(bytes_read = read(infile,buff,4096))
                 write(outfile,buff,bytes_read);
        fchmod(outfile, S_IREAD | S_IWRITE | S_IRGRP);
        close(infile);
        close(outfile);
        free(buff);
        l = 0;

	}
	if(l){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return errno == ENOENT ? E_NONEXIST : E_PERMISSION;
	}
	if(!l)
		l = unlink(src);
	if(l){
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		return errno == ENOENT ? E_NONEXIST : E_PERMISSION;
	}
	struct utimbuf* utimebuf = malloc(sizeof(struct utimbuf));
	utimebuf->actime = sbuff->st_atime;
	utimebuf->modtime = sbuff->st_mtime;
	utime(dst,utimebuf);
	//time_t    st_atime;   /* time of last access */
	//time_t    st_mtime;
	free(utimebuf);
	free(sbuff);
	return SUCCESS;
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

	int status = mov(oldPath, newPath, suffix);
	
	free(newPath);
	if(oldPath == orig)
		free(oldPath);
	return status;
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

int rmr(char* file, int end)
{
	char* oldPath = toAbsolute(file);
	char* orig = file;
	char* base = basename(file);
	char* newPath = calloc(strlen(TRASH) + strlen(file) + 253, sizeof(char));

	strcat(newPath, TRASH);
	strcat(newPath, "/");
	if(end){
		strcat(newPath, file+end);
		strcat(newPath, "/");
	}
	else
		strcat(newPath, base);
	struct stat* buff = malloc(sizeof(struct stat));
	int i = stat(oldPath, buff);
	if(i)
	{
		fprintf(stderr, "ERROR: Couln't stat file: %s\n\t%s\n", oldPath, strerror(errno));
		return EACCES == errno ? E_PERMISSION : E_NONEXIST;
	}
	if(S_ISDIR(buff->st_mode))
	{
		i = mkdir(newPath, buff->st_mode);
		if(!i)
			chmod(newPath, buff->st_mode);
		else{
			fprintf(stderr, "ERROR: %s\n", strerror(errno));
			return EACCES == errno ? E_PERMISSION : E_NONEXIST;
		}
		DIR *dp;
		struct dirent *d;

		dp = opendir(oldPath);
		if (dp == NULL) {
			fprintf(stderr,"ERROR: Could not open directory: %s\n", oldPath);
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
				rmr(nPCP, end);
				continue;
			}
			char* sop = malloc(strlen(file) + strlen(d->d_name) + 1);
			strcpy(sop, file);
			strcat(sop,"/");
			strcat(sop, d->d_name);
			char* snp = malloc(strlen(newPath) + strlen(d->d_name) + 4);
			char* sfx = snp + strlen(newPath) + strlen(d->d_name) + 1;
			strcpy(snp, newPath);
			strcat(snp,"/");
			strcat(snp, d->d_name);
			mov(sop, snp, sfx);
			free(sop);
			free(snp);

		}
		closedir(dp);
		rmdir(oldPath);
		return SUCCESS;
	}
	rm(oldPath);
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


void usage(char* name){
	printf("Usage: %s [OPTIONS]... FILES...\n\tRemove FILES (send to recycle bin)\n\tOptions:\n\t\t-h\tDisplay this help message\n\t\t-r\tRecursive, remove directories and their contents\n\t\t-f\tforce, removes files permanently\n", name);
	exit(0);
}