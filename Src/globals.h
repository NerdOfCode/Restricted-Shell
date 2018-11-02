/*

        *Licensed to the Apache Software Foundation (ASF) under one
        *or more contributor license agreements.  See the NOTICE file
        *distributed with this work for additional information
        *regarding copyright ownership.  The ASF licenses this file
        *to you under the Apache License, Version 2.0 (the
        *"License"); you may not use this file except in compliance
        *with the License.  You may obtain a copy of the License at
        *
        *       http://www.apache.org/licenses/LICENSE-2.0
        *
        *Unless required by applicable law or agreed to in writing,
        *software distributed under the License is distributed on an
        *"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
        *KIND, either express or implied.  See the License for the
        *specific language governing permissions and limitations
        *under the License.

	NerdOfCode Copyrighted 2018
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
