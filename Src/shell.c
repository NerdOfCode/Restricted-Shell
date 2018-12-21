/*
	 This file is part of the Restricted-Shell distribution (https://github.com/NerdOfCode/Restricted-Shell).
	 Copyright (c) 2018 NerdOfCode.
 
	 This program is free software: you can redistribute it and/or modify  
	 it under the terms of the GNU General Public License as published by  
	 the Free Software Foundation, version 3.
 
	 This program is distributed in the hope that it will be useful, but 
	 WITHOUT ANY WARRANTY; without even the implied warranty of 
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
	 General Public License for more details.
 
	 You should have received a copy of the GNU General Public License 
 	 along with this program. If not, see <http://www.gnu.org/licenses/>.
 

	#######################################################################################
	Author: NerdOfCode
	License: GPL-2.0
	Updated on: 11/1/18
	#######################################################################################

	#########################################################
	Documentation: Can be found on the github repository at:
	https://github.com/NerdOfCode/Restricted-Shell
	#########################################################

*/
#include "globals.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h> //Access
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#ifndef ENABLED_READLINE 
#include <readline/readline.h>
#include <readline/history.h>
#endif

//make our own version of 'bool'
typedef int bool;

//Function Prototypes
int start_up();
int clean_up();
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
char *logged_in_user;

struct user_access{
	bool pwd_allowed;
	bool whoami_allowed;
} adv_desc_access;

unsigned int mini_kernel_panic_counter;

int main ( int argc, char argv[64] ){

	char input[256] = "";
	char *pinput = "";
	char *hostname = "";

	//Test if user is allowed to use pwd and if allowed show the working directory
	//Also test if user is allowed to use hostname and whomai
	char *pwd_test = "";
	short int return_pwd_test_value = 0;

	char *whoami_test = "";
	short int return_whoami_test_value = 0;

	pwd_test = malloc(64 * sizeof(char));
	strncat(pwd_test,CMD_BIN, sizeof(CMD_BIN) + sizeof(pwd_test));
	strncat(pwd_test,"pwd", sizeof(pwd_test));

	whoami_test = malloc(64 * sizeof(char));
	strncat(whoami_test,CMD_BIN, sizeof(CMD_BIN) + sizeof(whoami_test));
	strncat(whoami_test,"whoami", sizeof(whoami_test));

	//Run our startup function
	start_up();

	//Run function to warn user of any settings enabled by the admin
	warn_user();

	if(access(pwd_test,F_OK) == 0){
		return_pwd_test_value = system(CMD_BIN"pwd -none");
		if(WEXITSTATUS(return_pwd_test_value) == 0){
			adv_desc_access.pwd_allowed = TRUE;
		}else if(return_pwd_test_value == -1){
			puts("Error accessing 'working directory'.");
		}
	}

	if(access(whoami_test,F_OK) == 0){
		return_whoami_test_value = system(CMD_BIN"whoami -none");
		if(WEXITSTATUS(return_whoami_test_value) == 1){
			adv_desc_access.whoami_allowed = TRUE;
		}else if(return_pwd_test_value == -1){
			puts("Error accessing 'working directory'.");
		}
	}

	//Get user and hostname here to eliminate repetitive use!
	logged_in_user = malloc(64 * sizeof(char));
	logged_in_user = getenv("USER");

	change_to_home_dir();

	hostname = malloc(64 * sizeof(char));
	hostname = getenv(HOSTNAME);

	start_up();

	while(1){
		if(adv_desc_access.pwd_allowed == TRUE && adv_desc_access.whoami_allowed == TRUE && ENABLED_READLINE){
			char pwd_buffer[128];
			char *short_pwd;
			//Just in case, run a function that changes the dir to the newly written one!
			update_new_cd(0);
			if(getcwd(pwd_buffer, sizeof(pwd_buffer))==NULL)
				if(DEBUG)
					puts("Error getting 'current working directory'.");
			//Remove all characters up to last one...
			short_pwd = remove_char_until(pwd_buffer, "/");

			printf(YELLOW_TEXT "%s@%s[%s] " RESET, logged_in_user,hostname,short_pwd);
			#ifndef ENABLED_READLINE
			pinput = readline("->");
			#endif
//			add_history(pinput);
			strncpy(input, pinput, 64);

		}else if(adv_desc_access.whoami_allowed == TRUE){
			printf(YELLOW_TEXT "%s@%s: " RESET, logged_in_user,hostname);
			if(fgets(input,sizeof(input),stdin) == NULL)
				if(DEBUG)
					puts("Error retrieving input.");
			strtok(input,"\n");
		}else{
			printf(YELLOW_TEXT "Command: " RESET);
			if(fgets(input,sizeof(input),stdin) == NULL)
				if(DEBUG)
					puts("Error retrieving input.");
			strtok(input,"\n");
		}


		//Reset testing values
		memset(pwd_test,0,sizeof(pwd_test));
		memset(whoami_test,0,sizeof(whoami_test));

		if(LOGGING)
			log_command(input);

		//Not a good idea for case specific things, will need to make its own function for better use

		//Check to see if user wants to exit before re-running loop
		//Have to check for newline too, because of fgets for input
		if(strncmp(input,"exit",sizeof("exit")) == 0){
			clean_up();
			exit(1);
		}else if(strncmp(input,"help",sizeof("help")) == 0){
			help_commands();
		}else if(strncmp(input,"cmds",sizeof("cmds")) == 0){
			commands();
		}else if(strncmp(input," ",sizeof(" ")) == 0){
			printf("He - He\n");
		}else{
			if(check_empty_beginning(input) <= -1){
				main(0,"e");
			}
			//Actually parse the command here
			parseCommand(input);
		}
	}
	//Start the clean up before exit
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

	//For chdir() return code
	short int ret = 0;

        char current_user_home[64] = "/home/";
        strncat(current_user_home,logged_in_user,sizeof(current_user_home));

	//chdir() return -1 on error and 0 on success
	ret = chdir(current_user_home);

	if(ret == -1)
		puts(RED_TEXT"Error: 1005"RESET);

}

int start_up( void ){
	//Delete any contents from the previous user in logs...
	FILE *fptr;

	char home[64] = "/home/";

	strncat(home, getenv("USER"), sizeof(home));

	strncat(home, "/", sizeof(home));

	strncat(home, RSHELL_DIR, sizeof(home));

	strncat(home, "/", sizeof(home));

	//Just to be sure
	mkdir(home,0755);

	strncat(home, USER_CD_LOG, sizeof(home));

	//Overwrite
        fptr = fopen(home, "w");

	if (fptr == NULL) {
		return -1;
	}else{
		fclose(fptr);
	}
}

int clean_up( void ){

	printf("Cleaning up...\n");
	//Reset color values
	puts(RESET);
	//TODO
	// - close and sanitize any files used for logging

 	//Delete any contents from the previous user in logs...
        FILE *fptr;

	char home[64] = "/home/";

	strncat(home, getenv("USER"), sizeof(home));

	strncat(home, "/", sizeof(home));

	strncat(home, RSHELL_DIR, sizeof(home));

	strncat(home, "/", sizeof(home));

	strncat(home, USER_CD_LOG, sizeof(home));

	//Get rid of users cwd
        fptr = fopen(home, "w");

	if(fptr != NULL){
		fclose(fptr);
	}else{
		return -1;
	}

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
	puts("rm");
	puts("date");
        puts("mkdir");

}

//Basically provide string and char and this will remove everything up to the last occurence of remove_char
char *remove_char_until(char specified_buffer[128],char remove_char[2]){
	//Begin removal process below

	static char remove_char_result[128] = "";

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

int check_empty_beginning(char input[256]){
	//0 --> No space at beggining
	//-1 --> A single space at index 0
	//-2 --> Two spaces at beggining

	//We'll hold off if it is only one space...
	if(input[0] == ' '){
		//If user puts two spaces before a command... GO ahead and let them know it's not found... 
		if(input[1] == ' '){
				puts("Command not found...\a");
				return -2;
		}
	}else{
		return 0;
	}
}

int parseCommand(char input[256]){

	char *pfilename = "";
	char *pcommand = "" ;
	bool command_args = FALSE;
	int command_status = 0;

	//The pcommand is for the user input but without any arguments attached

	//Dynamic memory  allocation
	pfilename = malloc(256 * sizeof(char));
	pcommand = malloc(256 * sizeof(char));

	//Check for any allocation errors before saving input
	if(pfilename == NULL || pcommand == NULL){
		fprintf(stderr, RED_TEXT"Failed to fork... Not enough memory...\n"RESET);
		return -1;
	}

	//Just in case, check for an empty command
	if(input[0] == '\0'){
		return -1;
	}

	//Prevent user from escaping by running something like: 'ls && exec /bin/bash'
	//or 'ls -fdsafajfsdfl || exec /bin/bash'
	//or 'ls;bash'

	for(int i = 0; i <= strlen(input); i++){
		if(input[i] == '&' || input[i] == '|' || input[i] == ';'){
				fprintf(stderr,"Illegal character detected...\n");
				return -255;
		}
	}

	//Remove all arguments
	for(int i = 0; i <= strlen(input); ++i){
		if(input[i] != ' '){
			//Delete args
			pcommand[i] = input[i];
			pcommand[i+1] = '\0';
			command_args = FALSE;
		}else{
			command_args = TRUE;
			pcommand[i+1] = '\0';
			break;
		}
	}
	

	if(DEBUG)
		printf("command_args: %s\n",command_args ? "False" : "True");

	//Obliterate pfilename
	//And check if command exists relative to its filename
	//Convert pcommand into pfilename with absolute path
	memset(pfilename, 0, sizeof(pfilename));
	strncat(pfilename,CMD_BIN, sizeof(CMD_BIN) + sizeof(pfilename));
	strncat(pfilename,pcommand, sizeof(pfilename) + sizeof(pcommand));

	//TEMPORARY FIX!!! 4/20/18
        //SPECIAL CASE: If command is cd, give a heads up to update the cwd
        if(strncmp(pcommand,"cd", sizeof(pcommand)) == 0){
                update_new_cd(1);
        }

	//If command or rather file is found, proceed
	if(access(pfilename, F_OK) == 0){
		//Since the command exists we can try running the arguments the user has provided
		//Check if args and no args are different
		if(strncmp(input, pcommand, sizeof(pcommand)) != 0){
			//ARGS
			//Reset to default users args
			memset(pfilename, 0, 256);
			strncat(pfilename, CMD_BIN, sizeof(CMD_BIN) + sizeof(pfilename));
			strncat(pfilename, input, sizeof(CMD_BIN) + sizeof(pfilename));

			if(system(pfilename) == -1)
				if(DEBUG)
					printf("Error executing: %s\n",pfilename);
		}else{
			//NO ARGS
			if(system(pfilename) == -1 )
				if(DEBUG)
					printf("Error executing: %s\n",pfilename);
		}
	}else{
			//Could not find file

			puts(RED_TEXT"Command Not Found!\a"RESET);
			if(DEBUG){
				//Will show the pathway to file
				//We can safely ignore return value
				int ignore_me = system(pfilename);
			}
		//}
	}


	//Reset variables
	command_args = FALSE;
	memset(pfilename, 0, sizeof(pfilename));
	memset(pcommand, 0, sizeof(pcommand));
	memset(input, 0, 64);

	return 0;
}

//Basically reads the cwd.db log file and takes the cwd from it

int update_new_cd( int update ){
	//Get the new directory from the log file
	FILE *fptr;
	char cd_buffer[255] = "";
	char cwd[1024] = "";
	char cwd_file[64] = "/home/";

	strncat(cwd_file, getenv("USER"), sizeof(cwd_file));

	strncat(cwd_file, "/", sizeof(cwd_file));

	strncat(cwd_file,RSHELL_DIR, sizeof(cwd_file));

	strncat(cwd_file, "/", sizeof(cwd_file));

	strncat(cwd_file, USER_CD_LOG, sizeof(cwd_file));

	fptr = fopen(cwd_file, "r");
	
	if(fptr != NULL){
		if(fscanf(fptr, "%s", cd_buffer) == 0)
			printf("Error: reading change directory");
	}else{
		return -1;
	}


	fclose(fptr);


	if(strncmp(cd_buffer,"../",sizeof("../")) || strncmp(cd_buffer,"..",sizeof("../"))){
		//Write over file

		//Delete old cwd file
		if(truncate(cwd_file, 0) == -1)
			printf("Error: unable to overwrite old 'cwd' file.");
		//Pretty much useless for now....
		short int ret = chdir(cd_buffer);
		memset(cd_buffer, 0, sizeof(cd_buffer));
	}else{
		//We should probably add a security mechanism here at a later date... 4/17/18
		short int ret = chdir(cd_buffer);
	}

	return 0;
}

int log_command(char *command){
	//Convert Log File to include home directory
	char home_dir_log[64] = "/home/";

	strncat(home_dir_log,logged_in_user, sizeof(home_dir_log));

	strncat(home_dir_log,"/", sizeof(home_dir_log));

	strncat(home_dir_log,RSHELL_DIR, sizeof(home_dir_log));

	strncat(home_dir_log, "/", sizeof(home_dir_log));

	strncat(home_dir_log, LOG_FILE, sizeof(home_dir_log));

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


