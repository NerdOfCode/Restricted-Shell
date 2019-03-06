/*
  This file is part of the Restricted-Shell distribution (https://github.com/NerdOfCode/Restricted-Shell).
  Copyright (c) 2019 NerdOfCode.
 
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
  Updated on: 3/6/19
  #######################################################################################

  #########################################################
  Documentation: Can be found on the github repository at:
  https://github.com/NerdOfCode/Restricted-Shell
  #########################################################

*/
#include "globals.h"
#include "lib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <ctype.h>
#include <unistd.h> // Access
#include <libgen.h> // basename
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/types.h>

#if ENABLED_READLINE 
#include <readline/readline.h>
#include <readline/history.h>
#endif

//Function Prototypess
static int start_up();
static int clean_up();

static int change_to_home_dir();

void help_commands();
void commands();
void warn_user();
void sig_handler();

static char *get_home_dir();

int parseCommand();
int check_empty_beginning();
int update_new_cd();
int log_command();

static char *logged_in_user;

struct {
  bool pwd_allowed;
  bool whoami_allowed;
} allowed_access;

unsigned int mini_kernel_panic_counter;

int main ( int argc, char argv[64] ){

  // Prevent release via crtl + n keys
  signal(SIGINT,sig_handler);
  signal(SIGQUIT,sig_handler);
  signal(SIGTSTP,sig_handler);
  
  char input[256] = "";
  char *pinput = "";

  //Test if user is allowed to use pwd and if allowed show the working directory
  //Also test if user is allowed to use hostname and whomai
  short int return_pwd_test_value = 0;
  short int return_whoami_test_value = 0;

  char *pwd_test = (char *) malloc(256 * sizeof(char));
  strncat(pwd_test,CMD_BIN, sizeof(CMD_BIN));
  strncat(pwd_test,"pwd", sizeof("pwd"));

  char *whoami_test = (char *) malloc(256 * sizeof(char));
  strncat(whoami_test,CMD_BIN, sizeof(CMD_BIN));
  strncat(whoami_test,"whoami", sizeof("whoami"));

  //Run our startup function
  start_up();

  //Run function to warn user of any settings enabled by the admin
  warn_user();

  if(access(pwd_test,F_OK) == 0){
    return_pwd_test_value = system(CMD_BIN"pwd -none");
    if(WEXITSTATUS(return_pwd_test_value) == 0){
      allowed_access.pwd_allowed = true;
    }else if(return_pwd_test_value == -1){
      puts("Error accessing 'working directory'.");
    }
  }

  if(access(whoami_test,F_OK) == 0){
    return_whoami_test_value = system(CMD_BIN"whoami -none");
    if(WEXITSTATUS(return_whoami_test_value) == 1){
      allowed_access.whoami_allowed = true;
    }else if(return_pwd_test_value == -1){
      puts("Error accessing 'working directory'.");
    }
  }

  //Get user and hostname here to eliminate repetitive use!
  char *logged_in_user = getenv("USER");
  if(logged_in_user == NULL){
    if(DEBUG){
      puts("Error: global USER not defined");
    }
  }
  // Let's check the status of changing to the user's home dir and output only if DEBUG mode is active
  if(change_to_home_dir()==-1){
    if(DEBUG){
      puts("Failed to change to home directory.");
    }
  }
  
  char *hostname = getenv("HOSTNAME");
  if(hostname == NULL){
    if(DEBUG){
      puts("Error: global HOSTNAME not defined");
    }
  }
      
  start_up();

  while(1){
    if(allowed_access.pwd_allowed && allowed_access.whoami_allowed){
      char pwd_buffer[256];
      char *short_pwd;

      update_new_cd(0);
      if(getcwd(pwd_buffer, sizeof(pwd_buffer)) == NULL){
	if(DEBUG){
	  puts("Error getting 'current working directory'.");
	}
      }else{
	short_pwd = basename(pwd_buffer);
      }
      
#if ENABLED_READLINE
      printf(YELLOW_TEXT "%s@%s[%s]\n" RESET, logged_in_user, hostname, short_pwd);
      pinput = readline("->");
#else
      printf(YELLOW_TEXT "%s@%s[%s]: " RESET, logged_in_user, hostname, short_pwd);
      if(fgets(input,sizeof(input),stdin) == NULL){
	if(DEBUG){
	  puts("Error retrieving input.");
	  puts("\ttry ./run --disable-readline");
      	}
      }
      strtok(input,"\n");
#endif
#if ENABLED_READLINE
      add_history(pinput);
#endif
      if(*pinput && !input){
	strncpy(input, pinput,sizeof(*pinput));
      }
      
      if(DEBUG){
	printf("Received: |%s|\n",input);
      }
    }else if(allowed_access.whoami_allowed){
      printf(YELLOW_TEXT "%s@%s: " RESET, logged_in_user,hostname);
      if(fgets(input,sizeof(input),stdin) == NULL){
	if(DEBUG){
	  puts("Error retrieving input.");
	}
      }
      strtok(input,"\n");
    }else{
      printf(YELLOW_TEXT "Command: " RESET);
      if(fgets(input,sizeof(input),stdin) == NULL){
	if(DEBUG){
	  puts("Error retrieving input.");
	}
      }
      strtok(input,"\n");
    }

      //Reset testing values
    memset(pwd_test,0,sizeof(pwd_test));
    memset(whoami_test,0,sizeof(whoami_test));

    if(LOGGING){
      log_command(input);
    }
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
static int change_to_home_dir( void ){
  //TODO
  //Create a better method for the following
  //Assume users directory is /home/logged_in_user

  //For chdir() return code
  short int ret = 0;
  
  char *current_user_home = getenv("HOME");

  //chdir() return -1 on error and 0 on success
  ret = chdir(current_user_home);

  if(ret == -1){
    puts(RED_TEXT"Error: 1005"RESET);
    return -1;
  }
  return 0;
}

static int start_up( void ){
  //Delete any contents from the previous user in logs...
  FILE *fptr;

  char home[64] = "/home/";

  strncat(home, getenv("USER"), sizeof(*getenv("USER")));

  strncat(home, "/", sizeof("/"));

  strncat(home, RSHELL_DIR, sizeof(RSHELL_DIR));

  strncat(home, "/", sizeof("/"));

  //Just to be sure, lets make sure our config directory exists, if not create it
  mkdir(home,0755);

  strncat(home, USER_CD_LOG, sizeof(USER_CD_LOG));

  //Overwrite any contents that may be in the CWD log file
  fptr = fopen(home, "w");

  if(fptr == NULL)
    return -1;
  else
    fclose(fptr);
}

static int clean_up( void ){
  if(DEBUG)
    printf("Cleaning up...\n");
  //Reset color values
  puts(RESET);
  //TODO
  // - close and sanitize any files used for logging

  //Delete any contents from the previous user in logs...
  FILE *fptr;

  char home[64] = "/home/";

  strncat(home, getenv("USER"), sizeof(*getenv("USER")));

  strncat(home, "/", sizeof("/"));

  strncat(home, RSHELL_DIR, sizeof(RSHELL_DIR));

  strncat(home, "/", sizeof("/"));

  strncat(home, USER_CD_LOG, sizeof(USER_CD_LOG));

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
  bool command_args = false;
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
      command_args = false;
    }else{
      command_args = true;
      pcommand[i+1] = '\0';
      break;
    }
  }
	

  if(DEBUG)
    printf("command_args: %s\n",command_args ? "True" : "False");

  //Obliterate pfilename
  //And check if command exists relative to its filename
  //Convert pcommand into pfilename with absolute path
  memset(pfilename, 0, sizeof(*pfilename));
  strncat(pfilename,CMD_BIN, sizeof(CMD_BIN));
  strncat(pfilename,pcommand,sizeof(pcommand));

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
      memset(pfilename, 0, *pfilename);
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
  command_args = false;
  memset(pfilename, 0, sizeof(*pfilename));
  memset(pcommand, 0, sizeof(*pcommand));
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
  char *home_dir_log;

  home_dir_log = get_home_dir();
  
  strcat(home_dir_log,"/");
  strcat(home_dir_log,RSHELL_DIR);
  strcat(home_dir_log,"/");
  strcat(home_dir_log,LOG_FILE);
  
  FILE *fptr = fopen(home_dir_log,"ab+");
  
  if(fptr == NULL){
    if(mini_kernel_panic_counter < KERNEL_PANIC_MAX_SHOW){
      puts(RED_TEXT"A mini-kernel panic has occurred... Please show the following error to your local admin!"RESET);
      puts(RED_TEXT"Error 1002"RESET);
      mini_kernel_panic_counter++;
    }
    //  return -1;
  }
  //Print the new dir to our CWD file
  fprintf(fptr,"%s\n",command);
  fclose(fptr);
  
  return 0;
}

void sig_handler(int sig_num){
  signal(SIGINT,sig_handler);
  fflush(stdout);
}

static char *get_home_dir(void){
  char *home_dir;
  
  struct passwd *upasswd;
  int current_uid = getuid();
  upasswd = getpwuid(current_uid);

  home_dir = upasswd->pw_dir;

  return home_dir;
}

/*
  Function: 
    void warn_user()
  Purpose:
    Basically give the user a heads up that the admin is using logging tools and whatnot
*/
void warn_user( void ){
  if(LOGGING)
    puts(RED_TEXT"CAUTION: Log Mode On" RESET);
}


