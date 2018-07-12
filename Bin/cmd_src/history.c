// Display the local users command history

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../../Src/globals.h"

int main(void){

	FILE *fptr;

	char *username;
	char file[64] = "/home/";
	char output;

	username = (char *) malloc(15);

	username = getlogin();

	strcat(file,username);

	//Append the log file name to the end

	strcat(file,"/"LOG_FILE);

	//Open file and get some work done

	fptr = fopen(file, "r");

	if(fptr == NULL){
		puts("Unable to access file.");
		exit(-1);
	}

	output = fgetc(fptr);

	while(output != EOF){
		printf("%c",output);
		output = fgetc(fptr);
	}


	fclose(fptr);

	//Free allocated memory
	//free(username);

	return 0;
}
