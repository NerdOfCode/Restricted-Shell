/*

Author: NerdOfCode
Purpose: A work in progress shell built in C... Designed to be lightweight and fast...
Updated: 3/10/18

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> //Access
#include <string.h>

//Color Constants
#define RED_TEXT "\033[1;31m"
#define YELLOW_TEXT "\x1B[33m"
//Use 43m for background shading
#define RESET "\x1B[0m"

//Default path to command Bin
#define CMD_BIN "./../Bin/"

//Function Prototypes
void clean_up();
void help_commands();

int parseCommand();

int main ( void ){


	char input[64];

	while(1){
		printf(YELLOW_TEXT "Command: " RESET, " " );
		fgets(input,64,stdin);

		//TODO 
		// - logging

		//Convert input to lowercase for unitext
		input[0] = tolower(input[0]);

		//Check to see if user wants to exit before re-running loop
		//Have to check for newline too, because of fgets for input
		if(strcmp(input,"exit\n") == 0){
			clean_up();
			exit(1);
		}else if(strcmp(input,"help\n") == 0){
			help_commands();
		}else{
			//Actually parse the command here
			parseCommand(input);
		}
	}


	//Start the clean up b4 exit
	clean_up();

	return 0;
}

void clean_up(){

	printf("Cleaning up...\n");
	//Reset color values
	printf("%s\n",RESET);
	//TODO 
	// - close any files used for logging


}

void help_commands(){
	printf("Current usage:\n");
	printf("exit --> Exits this shell...\n");
}


int parseCommand(char input[64]){

	char filename[66];

	//Check if command exists relative to its filename

	strcat(filename,CMD_BIN);
	strcat(filename,input);

	//Remove newline character
	filename[strlen(filename)-1] = 0;

	//If command or rather file is found, proceed
	if(access(filename, F_OK) == 0){
		system(filename);
	}else{
		printf("Command not found: %s\n",input);
	}

	//Reset variables
	memset(filename, 0, sizeof(filename));

	return 0;
}
