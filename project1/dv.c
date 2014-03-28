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

void usage(char* name);
int dv(char* file, int end);
int mov(char* src, char* dst);
char* toAbsolute(char* file);

char* TRASH;
char* cwd;

int main(int argc, char* argv[])
{
	if(argc < 2)
		usage(argv[0]);
	if(!strcmp(argv[1],"-h"))
		usage(argv[0]);
	// Get the trash variable
	TRASH = getenv("TRASH");
	if(!TRASH){
		fprintf(stderr,"Error: TRASH variable not set, please set $TRASH to a created directory.");
		exit(1);
	}
	cwd = get_current_dir_name();
	int i = 1;
	for(; i < argc; i++){
		if(dv(*(argv + i), 0))
			exit(1);
	}

}
char* toAbsolute(char* file)
{

	if(!strncmp(file,"/",1))
		return file;
	char* abs = calloc(strlen(file)+strlen(TRASH)+2,sizeof(char));
	strcat(abs, TRASH);
	strcat(abs, "/");
	strcat(abs, file);
	return abs;
}

int dv(char* file, int end)
{
	char* oldPath = toAbsolute(file);
	char* orig = strdup(file);
	char* tofree = orig;
	char* base = basename(orig);
	char* newPath = calloc(strlen(cwd) + strlen(file) + 253, sizeof(char));

	strcat(newPath, cwd);
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
				dv(nPCP, end);
				continue;
			}
			char* sop = malloc(strlen(oldPath) + strlen(d->d_name) + 1);
			strcpy(sop, oldPath);
			strcat(sop,"/");
			strcat(sop, d->d_name);
			char* snp = malloc(strlen(newPath) + strlen(d->d_name) + 4);
			strcpy(snp, newPath);
			strcat(snp,"/");
			strcat(snp, d->d_name);
			mov(sop, snp);
			free(sop);
			free(snp);

		}
		closedir(dp);
		rmdir(oldPath);
		free(tofree);
		return SUCCESS;
	}
	free(tofree);
	dvs(oldPath);
}

int dvs(char* file)
{
	char* oldPath = toAbsolute(file);
	char* orig = file;
	char* base = basename(file);
	char* newPath = calloc(strlen(cwd) + strlen(base) + 5, sizeof(char));
	char* suffix = newPath+(strlen(cwd)+strlen(base) + 1);

	strcat(newPath, cwd);
	strcat(newPath, "/");
	strcat(newPath, base);

	int status = mov(oldPath, newPath);
	
	free(newPath);
	if(oldPath == orig)
		free(oldPath);
	return status;
}

int mov(char* src, char* dst)
{
	struct stat* sbuff = malloc(sizeof(struct stat));
	int i = stat(src, sbuff);
	if(i)
	{
		fprintf(stderr, "ERROR: Couln't stat file: %s\n\t%s\n", src, strerror(errno));
		return EACCES == errno ? E_PERMISSION : E_NONEXIST;
	}
	int l = link(src, dst);
	while(l == -1 && errno == EEXIST)
	{
		fprintf(stderr, "File already exists in current directory\n");
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

void usage(char* name)
{
	printf("Usage: %s [-h] FILES...\n\tRestores FILES from recycle bin to current directory\n\tOptions:\n\t\t-h\tDisplay this help\n\t\tfiles\tthe space separated list of files to restore.\n", name);
	exit(0);
}