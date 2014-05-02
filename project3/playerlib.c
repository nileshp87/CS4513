#include "playerlib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int playFile(int handle){
    printf("\033[2J\033[0;0f");
	int frameSize = 2;
	char* cFrame = calloc(frameSize * 8192, sizeof(char));
	char* orig = cFrame;
	char* cFramep = cFrame;
	char frame = 'a';
	int c = 0;
	while(read(handle, &frame, 1))
	{
		if(c == 0 && frame == '\n'){
			//printf("nl");
			c++;
			continue;
		}
		else if(c == 1 && frame== 'e'){
			c++;
			continue;
		}
		else if(c == 2 && frame == 'n'){
			c++;
			continue;
		}
		else if(c == 3 && frame == 'd'){
			c++;
			continue;
		}
		else if((c == 4 && frame == '\n'))
		{
			printf("\033[2J\033[0;0f");
			printf("%s", cFrame);
			fflush(stdout);
			memset(cFrame, 0, cFramep - cFrame);
			cFramep = cFrame;
			usleep(80000);
  			c = 0;
  			continue;
		}
		else if(c){
			//printf("%d print", c);
			cFramep[0] = '\n';
			cFramep++;
			if(c > 1){
				cFramep[0] = 'e';
				cFramep++;
			}
			if(c > 2){
				cFramep[0] = 'n';
				cFramep++;
			}
			if(c > 3){
				cFramep[0] = 'd';
				cFramep++;
			}
			c = 0;
		}
		cFramep[0] = frame;
		cFramep++;
	}
	printf("\nDone\n");
	free(cFrame);
	return 1;
}