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
*/

//Value Conventions:
//	On --> 1
//	Off --> 0

#define TRUE 1
#define FALSE 0

//Used for displaying debug info for specific functions and commands (Experimental)
//Default:0
#define DEBUG 0

//Used for User-command logging
//Default:0
#define LOGGING 0

//Used for enabling or disabling libreadline-dev
//Default:1
#define ENABLED_READLINE 1

//Used for storing users commands to a specific file in their home directory
//Such as aliases
#define RSHELL_DIR ".r-shell"

#define LOG_FILE "r-shell_history"

#define ALIAS_FILE "r-shell_alias"

//Maximum amount of times to show the mini_kernel panic... 
//Error Code: 1002
#define KERNEL_PANIC_MAX_SHOW 3

//Color Constants
#define RED_TEXT "\033[1;31m"
#define YELLOW_TEXT "\x1B[33m"
#define RESET "\x1B[0m"

#define LATEST_VERSION "0.020"

//Warning: If you're considering changing the below...
//Make sure to adjust the Bin/cmd_src/nano...
//Default Directory of Shell
#define DEFAULT_LOCATION "/etc/Restricted-Shell/"

//Default Directory Log file for user
#define USER_CD_LOG "CWD"

//Default path to command Bin
#define CMD_BIN DEFAULT_LOCATION"Bin/"

#if defined(__linux__)
	#define HOSTNAME "USER"
#endif
