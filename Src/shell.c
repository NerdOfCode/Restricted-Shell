/*

#######################################################################################
Author: NerdOfCode
Purpose: A work in progress shell built in C... Designed to be lightweight and fast...
Tested on: Ubuntu 16.04
Status: Working
License: Apache-2.0
Updated on: 4/17/18
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
#include "globals.h"

//Default path to command Bin
#define CMD_BIN "./../Bin/"

//make our own version of 'bool'
typedef int bool;

#define TRUE 1
#define FALSE 0

//Function Prototypes
void clean_up();
void help_commands();
void commands();

char *remove_char_until();
int parseCommand();
int check_empty_beginning();


char remove_char_result[128];

int main ( int argc, char argv[64] ){

	bool pwd_allowed = FALSE;
	char input[64] = "";
	char *string_compare = "";

	//Test if user is allowed to use pwd and if allowed show the working directory
	char *pwd_test;
	int return_test_value;

	pwd_test = malloc(64 * sizeof(char));
	strcat(pwd_test,CMD_BIN);
	strcat(pwd_test,"pwd");

	if(access(pwd_test,F_OK) == 0){
			//EXITSTATUS(system(CMD_BIN"pwd"));
			return_test_value = system(CMD_BIN"pwd -none");
			if(WEXITSTATUS(return_test_value) == 0){
					pwd_allowed = TRUE;
			}
	}

	while(1){
		if(pwd_allowed == TRUE){
				char pwd_buffer[128];
				char *short_pwd;
				getcwd(pwd_buffer, sizeof(pwd_buffer));
				//Remove all characters up to last one...
				short_pwd = remove_char_until(pwd_buffer, "/");
				printf(YELLOW_TEXT "Command[%s]: " RESET, short_pwd);
				//printf("CD BUFFER: %s\n",cd_buffer);
				fgets(input,64,stdin);
		}else{
				printf(YELLOW_TEXT "Command: " RESET);
				fgets(input,64,stdin);
		}

		memset(pwd_test,0,sizeof(pwd_test));

		//TODO
		// - logging

		//Not a good idea for case specific things, will need to make its own function for better use
		//Convert input to lowercase for unitext

		//Check to see if user wants to exit before re-running loop
		//Have to check for newline too, because of fgets for input
		if(strncmp(input,"exit\n",sizeof(input)) == 0){
			clean_up();
			exit(1);
		}else if(strncmp(input,"help\n",sizeof(input)) == 0){
			help_commands();
		}else if(strncmp(input,"cmds\n",sizeof(input)) == 0){
			commands();
		}else if(strncmp(input," \n",sizeof(input)) == 0){
			printf("He - He\n");
		}else{
			if(check_empty_beginning(input) <= -1){
				main(0,"e");
			}
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

//Basically provide string and char and this will remove everything up to the last occurence of remove_char
char *remove_char_until(char specified_buffer[128],char remove_char[2]){
	//Begin removal process below

	int highest = 0,i = 0;

	for(i = 0; i <= strlen(specified_buffer);++i){
			if(specified_buffer[i] == '/'){
					//Will re-write until the last number which will hopefully be the highest :))))
					highest = i;
			}
	}
	i=0;
	while(highest != strlen(specified_buffer)){
		highest++;
		remove_char_result[i] = specified_buffer[highest];
		i++;
	}

	return remove_char_result;
}

int check_empty_beginning(char input[64]){
	//0 --> No space at beggining
	//-1 --> A single space at index 0
	//-2 --> Two spaces at beggining

	//We'll hold off if it is only one space...
	if(input[0] == ' '){
		if(input[1] == ' '){
				puts("Command not found...");
				return -2;
		}
	}else{
		return 0;
	}
}

int parseCommand(char input[64]){

	char *filename_ptr;
	char *command_ptr;
	bool command_args = FALSE;
	int command_status = 0;
	//The command_ptr is for the user input but without any arguments attached

	//Dynamic memory  allocation
	filename_ptr = malloc(64 * sizeof(char));
	command_ptr = malloc(64 * sizeof(char));

	//Check for any allocation errors before saving input
	if(filename_ptr == NULL || command_ptr == NULL){
		fprintf(stderr, RED_TEXT"Failed to fork... Not enough memory...\n"RESET);
		return 0;
	}

	//Just in case, check for an empty command
	if(input[0] == '\n'){
		return 0;
	}

	//Prevent user from escaping by running something like: 'ls && exec /bin/bash'
	//or 'ls -fdsafajfsdfl || exec /bin/bash'

	for(int i = 0; i <= strlen(input); i++){
		if(input[i] == '&' || input[i] == '|'){
				fprintf(stderr,"Illegal character detected...\n");
				return 0;
		}
	}


	//Remove all arguments
	for(int i = 0; i <= strlen(input); ++i){
		if(input[i] != ' '){
			command_ptr[i] = input[i];
			command_args = TRUE;
		}else{
			command_args = FALSE;
			break;
		}

	}


	//Check if command exists relative to its filename

	strcat(filename_ptr,CMD_BIN);
	strcat(filename_ptr,command_ptr);


	//Only remove newline if command has arguments
	if(command_args){
			filename_ptr[strlen(filename_ptr)-1] = '\0';
	}

	command_ptr[strlen(command_ptr)-1] = '\0';
	input[strlen(input)-1] = '\0';



	//If command or rather file is found, proceed
	if(access(filename_ptr, F_OK) == 0){
		//Since the command exists we can try running the arguments the user has provided
		//Check if args and no args are different
		if(input != command_ptr){

			//Reset to default users args
			memset(filename_ptr, 0, 64);
			strcat(filename_ptr, CMD_BIN);
			strcat(filename_ptr, input);
			system(filename_ptr);
		}else{
				system(filename_ptr);
		}
	}else{
		puts("Command not found...");
	}

	//Reset variables
	command_args = FALSE;
	memset(filename_ptr, 0, sizeof(filename_ptr));
	memset(command_ptr, 0, sizeof(command_ptr));
	memset(input, 0, 64);

	return 0;
}
