// Display the local users command history

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../../Src/globals.h"

int main(int argc, char *argv[]){

	int show_max = 0;

	if(argc < 2){
		//Set default amount lines to show to 9999
		show_max = 9999;
	}else{
		if(strncmp(argv[1],"--help",sizeof("--help")) == 0){
			puts("History Help:");
			puts("	-->Pass a number as an argument to only display n number of lines...");
			puts("	-->Ex: history 5 --> Displays 5 lines of history");
			return 0;
		}
		show_max = strtol(argv[1], NULL, 0);
	}

	char *username;
	char file[64] = "/home/";
	char output;
	char line[256];

	int display_count = 0;

	username = (char *) malloc(15);

	username = getenv("USER");

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

	FILE* fptr; 
	fptr = fopen(file, "r");

	if(fptr == NULL){
		puts("Unable to access file.");
		exit(-1);
	}



	while(fgets(line, sizeof(line), fptr) != NULL && show_max > display_count){
			display_count += 1;
			printf("%s", line);
	}


	fclose(fptr);

	//Free allocated memory
	//free(username);

	return 0;
}
