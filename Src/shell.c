/*

#######################################################################################
Author: NerdOfCode
Purpose: A work in progress shell built in C... Designed to be lightweight and fast...
Tested on: Antergos(Linux)
License: Apache-2.0
Updated on: 3/10/18
#######################################################################################

#########################################################
Documentation: Can be found on the github repository at:
https://github.com/NerdOfCode/Restricted-Shell
#########################################################

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
void commands();

int parseCommand();

int main ( int argc, char *argv[] ){


	char input[64] = "";


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
		}else if(strcmp(input,"cmds\n") == 0){
			commands();
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
	printf("Welcome to NerdOfCode's Restricted-Shell!!!\n");
	printf("For a complete list of commands, please run 'cmds'\n");
}

void commands(){
   	printf("Current usage:\n");
        printf("exit --> Exits this shell...\n");
        printf("pwd --> Prints the working directory\n");
        printf("clear --> Clears the screen");
        puts("");

}

int parseCommand(char input[64]){

	char filename[66];
	char command[32];

	//Remove all arguments
	for(int i = 0; i <= strlen(input); i++){
		if(input[i] != ' '){
			command[i] = input[i];
		}else{
			command[i+1] = '\0';
			break;
		}

	}


	//Check if command exists relative to its filename

	strcat(filename,CMD_BIN);
	strcat(filename,command);


	//Remove newline character
	filename[strlen(filename)-1] = '\0';

	//If command or rather file is found, proceed
	if(access(filename, F_OK) == 0){
		system(filename);
	}else{
		printf("Command not found: %s\n",input);
	}

	//Reset variables
	memset(filename, 0, sizeof(filename));
	memset(command, 0, sizeof(command));
	memset(input, 0, 64);

	return 0;
}



