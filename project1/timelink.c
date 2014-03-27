#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <libgen.h>

struct timeval timeDiff(struct timeval *end, struct timeval *start);

int main(int argc, char* argv[]){
	struct timeval startTime; // start time struct
	struct timeval endTime; // end time struct
	struct timeval diff; // end time struct

    gettimeofday(&startTime,NULL);
    link(argv[1], argv[2]);
    unlink(argv[2]);
    gettimeofday(&endTime,NULL);
    diff = timeDiff(&endTime,&startTime);
        printf("Timings Report:\n              Before:        After:        Difference:\n     Seconds: %10lld  %10lld  %10lld\nMicroSeconds: %10lld  %10lld  %10lld\n",startTime.tv_sec,endTime.tv_sec,diff.tv_sec,startTime.tv_usec,endTime.tv_usec,diff.tv_usec);

}

struct timeval timeDiff(struct timeval *end, struct timeval *start){
        struct timeval diff;
        diff.tv_sec = (*end).tv_sec - (*start).tv_sec;
        if(diff.tv_sec == 0)
                diff.tv_usec = (*end).tv_usec - (*start).tv_usec;
        else{
                diff.tv_usec = (*end).tv_usec-(*start).tv_usec > 0 ? (*end).tv_usec - (*start).tv_usec : (*start).tv_usec - (*end).tv_usec;
        }if((*end).tv_usec < (*start).tv_usec)
                diff.tv_sec = diff.tv_sec - 1;
        return diff;
}


