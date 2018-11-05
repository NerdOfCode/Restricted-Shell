#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../../Src/globals.h"

int main(int argc, char *argv[]){

	int ret = 0;
	char buffer[64];
	char dir[128] = "";

	if(argc < 2){
		puts("Please provide an argument: ");
		puts("	-->mkdir Hello-World/");
		return 1;
	}

        if(getcwd(buffer,64)==NULL){
		puts(RED_TEXT"Could not get directory!"RESET);
	}

	strcat(dir,buffer);

	strcat(dir, "/");

	strcat(dir, argv[1]);

	ret = mkdir(dir, 0755);

	if(ret != 0){
		printf(RED_TEXT"%s\n"RESET,strerror(errno));
		return ret;
	}

	return 0;

}
