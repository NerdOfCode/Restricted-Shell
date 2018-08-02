// Display the local users command history

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../../Src/globals.h"

int main(int argc, char *argv[]){

	int show_max = 0;
	int ch;

	if(argc < 2){
		show_max = 9999;
	}else{
		show_max = strtol(argv[1], NULL, 0);
	}

	FILE *fptr;

	char *username;
	char file[64] = "/home/";
	char output;

	int display_count = 0;

	username = (char *) malloc(15);

	username = getlogin();

	strcat(file,username);

	//Append the log file name to the end

	strcat(file, "/");

	strcat(file, RSHELL_DIR);

	strcat(file,"/");

	strcat(file, LOG_FILE);

	if(DEBUG == TRUE){
		printf("Pulling history from: %s\n",file);
	}

	//Open file and get some work done

	fptr = fopen(file, "r");

	if(fptr == NULL){
		puts("Unable to access file.");
		exit(-1);
	}

	while((ch = fgetc(fptr)) != EOF  && display_count <= show_max){

		printf("%c",ch);

		display_count++;
	}

	fclose(fptr);

	printf("\n");

	//Free allocated memory
	//free(username);

	return 0;
}
