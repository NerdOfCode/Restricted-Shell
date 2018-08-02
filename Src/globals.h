/*****************************/
//EDIT

//Used for displaying debug info for specific functions and commands (Experimental)
#define DEBUG 1

//Used for User-command logging
#define LOGGING 1

//Used for storing users commands to a specific file in their home directory
//Such as aliases
#define RSHELL_DIR ".r-shell"

#define LOG_FILE "r-shell_history"

//Maximum amount of times to show the mini_kernel panic... 
//Error Code: 1002
#define KERNEL_PANIC_MAX_SHOW 3

//Only Edit Below If You Can Afford To Break Things
/*****************************/

//Color Constants
#define RED_TEXT "\033[1;31m"
#define YELLOW_TEXT "\x1B[33m"

//Use 43m for background shading
#define RESET "\x1B[0m"

//Default Directory of Shell
#define DEFAULT_LOCATION "/etc/Restricted-Shell/"

//Default Directory Log file for user
#define USER_CD_LOG "/etc/Restricted-Shell/Logs/cwd.db"

//Where commands are logged to...
//#define USER_LOG "/etc/Restricted-Shell/Logs/user.log"

//Default path to command Bin
#define CMD_BIN "/etc/Restricted-Shell/Bin/"

#define TRUE 1
#define FALSE 0


//Platform detection
#if defined(__linux__)
	#define HOSTNAME "USER"
#endif
