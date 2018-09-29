#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../../Src/globals.h"

int main(int argc, char *argv[]){

	char buffer[64] = "";
	char rmdir[128] = "";

	if(argc < 2 || argc > 2){
		puts("Please provide at  one argument:");
		puts("	-->rmdir dir_name");
		return 1;
	}

	strcat(rmdir, "rm -rf ");

        getcwd(buffer,64);

	strcat(rmdir,buffer);

	strcat(rmdir, "/");

	strcat(rmdir, argv[1]);

	if(DEBUG)
		printf("%s\n",rmdir);

	system(rmdir);

	return 0;
}
