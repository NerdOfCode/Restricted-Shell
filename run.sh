#!/bin/bash 

: '

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

'
#Author: NerdOfCode
#Purpose: Easily setup this Restricted-Shell

#Source the global_bash_var file
source Src/global_bash_var

#Change to 0 to turn auto updates off
updates=0

config=".config"

#Change to 1 if you don't want to recompile if built
dont_compile=0

##Where users are stored
##UNUSED AS OF 7/10/18
user_db="Logs/users.db"

##FOR COLOR COATING
RED='\033[0;41m'
YELLOW='\e[0;33m'

RESET='\033[0;37m'


if [[ "$updates" == "1" ]]
then
	echo "Updating repository..."
	git pull origin master
fi

#Shortcut to make sure all shell scripts are in fact executable
chmod +x Bin/*

check_commands(){

	exit_status=0

	if [[ ! $(command -v sed) ]]
	then
        	echo "Please install 'sed'..."
		exit_status=1
	fi
	
	if [[ ! $(command -v make) ]]
	then
		echo "Please install 'make'..."
		exit_status=1
	fi

	if [[ ! $(command -v gcc) ]]
	then
		echo "Please install 'gcc'..."
		exit_status=1
	fi

	if [[ $exit_status -eq 1 ]]
	then
		exit -1
	fi

}

check_exec(){
	if [[ -f Src/shell ]]
	then
		echo "This shell appears to already be compiled..."
		read -p "Would you like to run without re-compiling, (y)es or (n)o: " option
	fi
	if [[ "$option" == "y" ]]
	then
		dont_compile=1
	fi
}

disallow_shell_command(){
	echo "$(sed 's/^/#/' $1)" > $1
	echo "echo \"Command is disallowed by admin...\"" >> $1
}

disallow_c_command(){
    	contents="$(cat $1)"
        echo "/*" > $1
        echo "$contents" >> $1
        echo "*/" >> $1
        echo "#include <stdio.h>" >> $1
        echo "int main(void){" >> $1
        echo "printf(\"Command disallowed by admin...\n\");return -1;}" >> $1
}
sign_in(){
	read -p "Enter Username: " USERNAME
	read -p "Enter Password: " PASSWORD
	PASSWORD="$(echo \"$PASSWORD\" | sha512sum | awk -F ' ' '{print $1}' )"

	#verify
	verify=$(sqlite3 $user_db "SELECT id FROM users WHERE password=\"$PASSWORD\";")

	if [[ ! -z "$verify" ]]
	then
		printf "${YELLOW}Welcome, user: $verify${RESET}\n"
		add_admin=0
	else
		printf "${RED}Authentication Failed!${RESET}\n"
		exit -1
	fi

}
create_admin_acc(){

	sqlite3 $user_db "CREATE TABLE IF NOT EXISTS users ( id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT NOT NULL, password TEXT NOT NULL);"

	status=$(sqlite3 $user_db "SELECT id FROM users WHERE id=1;")
	admin_count=$(sqlite3 $user_db "SELECT id FROM users;")

	#echo "Local Admins: ${admin_count: -1}"

	if [ "$status" == "1" ]
	then
		echo "You will need to be signed in by an exisiting administrator! "
		sign_in
	fi

	if [[ $add_admin != 0 ]]
	then
		printf "${RED}One More Thing! It is Time to setup the Admin account!\n"
		printf "Enter the following information!${RESET}\n"
	
		read -p "Enter Username: " username
		read -p "Enter Password: " password

		password="$(echo \"$password\" | sha512sum | awk -F ' ' '{print $1}' )"	
		echo $password
		sqlite3 $user_db "INSERT INTO users(username,password) VALUES(\"$username\", \"$password\");"
	fi

}

clear

check_commands

if [[ ! -f $config ]]
then

	clear

	read -p "Enter Default Directory for Shell(Default: '${DEFAULT_LOCATION}): " location

	if [[ -z $location ]]
	then
        	location="${DEFAULT_LOCATION}"
	else
		sed -i "s|${DEFAULT_LOCATION}|${location}|g" run.sh
		sed -i "s|${DEFAULT_LOCATION}|${location}|g" Src/globals.h
		sed -i "s|${DEFAULT_LOCATION}|${location}|g" Src/global_bash_var
		sed -i "s|${DEFAULT_LOCATION}|${location}|g" Bin/nano
	fi

	#Prompt user to allow what commands
	echo -e "Please choose what commands to allow your users to use below: \n"

	read -p  "'ls'(y/n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallow_shell_command "Bin/ls"
	fi

	read -p "'nano'(y/n): " option1
	
	if [[ "$option1" != "y" ]]
	then
		disallow_shell_command "Bin/nano"
	fi

	read -p "'pwd'(y/n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallow_c_command "Bin/cmd_src/pwd.c"

	fi

	read -p "'flags'(y/n): "

	if [[ "$option1" != "y" ]]
	then
		disallow_c_command "Bin/cmd_src/flags.c"
	fi

	read -p "'cd'(y/n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallow_c_command "Bin/cmd_src/cd.c"
	fi

	read -p "'whoami'(y\n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallow_c_command "Bin/cmd_src/whoami.c"
	fi

	read -p "'hostname'(y/n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallow_c_command "Bin/cmd_src/hostname.c"
	fi

	read -p "'history'(y/n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallow_c_command "Bin/cmd_src/history.c"
	fi

	read -p "'rm'(y/n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallow_c_command "Bin/cmd_src/rm.c"
	fi

	read -p "'mkdir'(y/n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallow_c_command "Bin/cmd_src/mkdir.c"
	fi

	read -p "'rmdir'(y/n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallow_c_command "Bin/cmd_src/rmdir.c"
	fi

	touch $config
fi
 
clear

#create_admin_acc

check_exec

if [[ $dont_compile -eq 0 ]]
then
	make
fi

cd Src/

#clear

#if [ ! -f Src/shell ]
#then
#	echo "An error occurred in compiling shell.c ... Please scroll up for errors"
#	exit -1
#fi

./shell

