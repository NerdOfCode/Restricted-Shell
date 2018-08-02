/*

#######################################################################################
Author: NerdOfCode
Purpose: A work in progress shell built in C... Designed to be lightweight and fast...
Tested on: Ubuntu Server 16.04
Status: Working --> ^^^
License: Apache-2.0
Updated on: 7/31/18
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
#include "globals.h"

//make our own version of 'bool'
typedef int bool;

//Function Prototypes
void start_up();
void clean_up();
void help_commands();
void commands();
void warn_user();
void change_to_home_dir();

char *remove_char_until();

int parseCommand();
int check_empty_beginning();
int update_new_cd();
int log_command();

//Globals
char remove_char_result[128];
char *home_dir;
char *logged_in_user;

bool pwd_allowed = FALSE;
bool whoami_allowed = FALSE;

int mini_kernel_panic_counter = 0;

int main ( int argc, char argv[64] ){

	char input[64] = "";
	char *string_compare = "";
	char *hostname = "";

	//Test if user is allowed to use pwd and if allowed show the working directory
	//Also test if user is allowed to use hostname and whomai
	char *pwd_test;
	int return_pwd_test_value;

	char *whoami_test;
	int return_whoami_test_value;

	pwd_test = malloc(64 * sizeof(char));
	strcat(pwd_test,CMD_BIN);
	strcat(pwd_test,"pwd");

	whoami_test = malloc(64 * sizeof(char));
	strcat(whoami_test,CMD_BIN);
	strcat(whoami_test,"whoami");

	//Run our startup function
	start_up();

	//Run function to warn user of any settings enabled by the admin
	warn_user();

	if(access(pwd_test,F_OK) == 0){
		//EXITSTATUS(system(CMD_BIN"pwd"));
		return_pwd_test_value = system(CMD_BIN"pwd -none");
		if(WEXITSTATUS(return_pwd_test_value) == 0){
			pwd_allowed = TRUE;
		}
	}

	if(access(whoami_test,F_OK) == 0){
		return_whoami_test_value = system(CMD_BIN"whoami -none");
		if(WEXITSTATUS(return_whoami_test_value) == 1){
			whoami_allowed = TRUE;
		}
	}

	//Get user and hostname here to eliminate repetitive use!
	logged_in_user = malloc(64 * sizeof(char));
	logged_in_user = getlogin();

	change_to_home_dir();

	hostname = malloc(64 * sizeof(char));
	hostname = getenv(HOSTNAME);

	start_up();

	while(1){
		if(pwd_allowed == TRUE && whoami_allowed == TRUE){
			char pwd_buffer[128];
			char *short_pwd;
			//Just in case, run a function that changes the dir to the newly written one!
			update_new_cd(0);
			getcwd(pwd_buffer, sizeof(pwd_buffer));
			//Remove all characters up to last one...
			short_pwd = remove_char_until(pwd_buffer, "/");
			printf(YELLOW_TEXT "%s@%s[%s]: " RESET, logged_in_user,hostname,short_pwd);
			//printf("CD BUFFER: %s\n",cd_buffer);

			//TODO --> Real Time History Check
			//Send all key calls directly to stdin
			//system("/bin/stty raw");
			//Send STTY back to normal behavior
                	//system("/bin/stty cooked");

			fgets(input,64,stdin);

		}else if(whoami_allowed == TRUE){
			printf(YELLOW_TEXT "%s@%s: " RESET, logged_in_user,hostname);
			fgets(input,64,stdin);
		}else{
			printf(YELLOW_TEXT "Command: " RESET);
			fgets(input,64,stdin);
		}



		//Reset testing values
		memset(pwd_test,0,sizeof(pwd_test));
		memset(whoami_test,0,sizeof(whoami_test));

		//TODO
		log_command(input);

		//Not a good idea for case specific things, will need to make its own function for better use

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

	//Start to free dynamically allocated memory
	free(pwd_test);
	free(whoami_test);
	free(logged_in_user);
	free(hostname);

	return 0;
}

//Basically change to the users home directory
void change_to_home_dir( void ){
	//TODO
        //Create a better method for the following
        //Assume users directory is /home/logged_in_user
        char current_user_home[64] = "/home/";
        strcat(current_user_home,logged_in_user);
        chdir(current_user_home);
}

void start_up( void ){
	//Delete any contents from the previous user in logs...
	FILE *fptr;
        char buffer[255];

        fptr = fopen(USER_CD_LOG, "w");

	fclose(fptr);

}

void clean_up( void ){

	printf("Cleaning up...\n");
	//Reset color values
	puts(RESET);
	//TODO
	// - close and sanitize any files used for logging

 	//Delete any contents from the previous user in logs...
        FILE *fptr;
        char buffer[255];

	//Get rid of users cwd
        fptr = fopen(USER_CD_LOG, "w");
        fclose(fptr);
}

void help_commands(){
	printf("Welcome to NerdOfCode's Restricted-Shell!!!\n");
	printf("For a complete list of commands, please run 'cmds'\n");
}

void commands(){
   	puts("Current usage:");
        puts("exit");
        puts("pwd");
        puts("clear");
	puts("cd");
	puts("ls");
	puts("version");
	puts("hostname");
	puts("whoami");
	puts("flags");
	puts("history");
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
	//Remove characters up until the highest 
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
	//or 'ls;bash'

	for(int i = 0; i <= strlen(input); i++){
		if(input[i] == '&' || input[i] == '|' || input[i] == ';'){
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

	//Obliterate filename_ptr
	//And check if command exists relative to its filename
	memset(filename_ptr, 0, sizeof(filename_ptr));
	strcat(filename_ptr,CMD_BIN);
	strcat(filename_ptr,command_ptr);

	//Only remove newline if command has arguments
	if(command_args){
		filename_ptr[strlen(filename_ptr)-1] = '\0';
	}


        command_ptr[strlen(command_ptr)-1] = '\0';
        input[strlen(input)-1] = '\0';

	//TEMPORARY FIX!!! 4/20/18
        //SPECIAL CASE: If command is cd, give a heads up to update the cwd
        if(strncmp(command_ptr,"c", sizeof(command_ptr)) == 0){
                update_new_cd(1);
        }



	//If command or rather file is found, proceed
	if(access(filename_ptr, F_OK) == 0){
		//Since the command exists we can try running the arguments the user has provided
		//Check if args and no args are different
		if(input != command_ptr){

			//Reset to default users args
			memset(filename_ptr, 0, 128);
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

//Basically reads the cwd.db log file and takes the cwd from it

int update_new_cd( int update ){
	//Get the new directory from the log file
	FILE *fptr;
	char cd_buffer[255] = "";
	char cwd[1024] = "";

	fptr = fopen(USER_CD_LOG, "r");
	fscanf(fptr, "%s", cd_buffer);
	fclose(fptr);

	if(strcmp(cd_buffer,"../") || strcmp(cd_buffer,"..")){
		//Write over file
		//LINUX SYSTEM DEPENDENT
		if(HOSTNAME != "USER"){
			puts("This system is not supported...");
			exit(1);
		}
		truncate(USER_CD_LOG, 0);

		chdir(cd_buffer);
		memset(cd_buffer, 0, sizeof(cd_buffer));
	}else{
		//We should probably add a security mechanism here at a later date... 4/17/18
		chdir(cd_buffer);
	}


	/*
   	if(getcwd(cwd, sizeof(cwd)) != NULL){
       		printf("Current working dir: %s\n", cwd);
   	}else{
		fprintf(stderr, "cwd() error");
	}
	*/

	return 0;
}

int log_command(char *command){
	//Convert Log File to include home directory
	char home_dir_log[64] = "/home/";

	strcat(home_dir_log,logged_in_user);

	strcat(home_dir_log,"/");

	strcat(home_dir_log,RSHELL_DIR);

	strcat(home_dir_log, "/");

	strcat(home_dir_log, LOG_FILE);

	FILE *fptr = fopen(home_dir_log,"ab+");

        //Protect against errors
        if(fptr == NULL){
                if(mini_kernel_panic_counter < KERNEL_PANIC_MAX_SHOW){
			puts(RED_TEXT"A mini-kernel panic has occurred... Please show the following error to your local admin!"RESET);
			puts(RED_TEXT"Error 1002"RESET);
			mini_kernel_panic_counter++;
        	}
			return -1;
	}

        //Print the new dir to our file
        fprintf(fptr,"%s",command);
        fclose(fptr);
}

//Basically give the user a heads up that the admin is using logging tools and whatnot
//Also, since this function is most likely going to be run when the shell is activated, 
//We will need to clear the screen

void warn_user( void ){

	//-->
	//system(CMD_BIN"clear");

	if(LOGGING == TRUE){
		puts(RED_TEXT"CAUTION: Log Mode On" RESET);
	}

}


