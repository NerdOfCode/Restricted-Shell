#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../Src/globals.h"

//argv[1] = alias command
//argv[2] = real command

int main( int argc, char *argv[] ){

	puts("The alias command is currently a WIP... Sorry...");

	exit(-1);

	if(argc < 2){

		puts(RED_TEXT"Please follow the formatting example:"RESET);
		puts("alias clear cls");

		return -1;
	}else{

		printf("Creating alias '%s' for command: '%s'\n",argv[1],argv[2]);

	}

	//Define Variables Here

	char new_alias[64];
	char alias_home[64] = "/home/";

	//Create New Alias

	strcat(new_alias, argv[1]);

	strcat(new_alias, "=");

	strcat(new_alias, argv[2]);

	//Create the directory to go to for alias file


	strcat(alias_home, getlogin());

	strcat(alias_home,"/");

	strcat(alias_home, RSHELL_DIR);

	strcat(alias_home, "/");

	strcat(alias_home, ALIAS_FILE);

	if(DEBUG == TRUE){
		printf("Creating aliases in the %s file\n",alias_home);
	}

	FILE *fptr = fopen(alias_home, "ab+");

	if(fptr == NULL){
		puts(RED_TEXT"A mini-kernel panic has occurred..."RESET);
		puts(RED_TEXT"Error Code: 1003");
		exit(-1);
	}


	fprintf(fptr, "%s\n", new_alias);

	fclose(fptr);

	return 0;

}
