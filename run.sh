#!/bin/bash 
: '
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
'
#Author: NerdOfCode
#Purpose: Easily setup this Restricted-Shell

#Source the global_bash_var file
source Src/global_bash_var

#If file exists, don't prompt user for start-configuration
config=".config"

#Change to 1 if you don't want to recompile if built
dont_compile=0

#Assume user doesen't want to disable readline functionality if they dont pass --disable-readline
disable_readline=0

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

help(){
	#If --help flag is thrown
	echo "Below are the available options:"
	echo "    --help -> Display this help menu"
	echo ""
	echo "    --disable-readline -> disable readline functionality... Useful if dependency not installed."
	echo "    --enable-readline -> used to enable readline functionality after being disabled."
	echo ""
	echo "    --reinstall -> Display reinstallation instructions"
	echo "    --update -> Update the R-Shell via Git"

	exit 0
}

reinstall(){
	#If --reinstall flag is thrown
	echo "Please run: "
	echo "		bash reinstall.sh"
	exit 0
}

check_dependency(){
	eval $1
	if [[ $? -ne 0 ]]
	then
		#Tell the whole script that we have a dependency problem
		dependency_exit=1
		echo "$2"
		exit -1
	fi

}

check_updates(){
	#If --update flag is thrown
	echo "Attempting to update..."
	git pull origin master
	exit 0
}

check_commands(){

	exit_status=0

	if [[ ! $(command -v readlink) ]]
	then
		echo "Please install 'readlink'..."
		exit_status=1
	fi

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

#Run through args
for arg in "$@"
do
	if [[ "$arg" == "--help" ]]
	then
		help
	fi

	if [[ "$arg" == "--disable-readline" ]]
	then
	    sed -i 's/ENABLED_READLINE ./ENABLED_READLINE 0/g' Src/globals.h
	    sed -i 's/libs_for_gcc=.*/libs_for_gcc=/g' Makefile
	    disable_readline=1
	fi

	if [[ "$arg" == "--enable-readline" ]]
	then
	    sed -i 's/ENABLED_READLINE ./ENABLED_READLINE 1/g' Src/globals.h
            sed -i 's/libs_for_gcc=.*/libs_for_gcc= -lreadline/g' Makefile
	    check_dependency "dpkg -l libreadline-dev >/dev/null 2>&1" "Please install 'libreadline-dev'." 
	fi
	
        if [[ "$arg" == "--reinstall" ]]
        then
                reinstall
        fi

	if [[ "$arg" == "--update" ]]
	then
		check_updates
	fi
done

check_commands

if [[ ! -f $config ]]
then

	clear

	while true
	do
		#Check if required dependency's are installed
		
		#Check build-essential
		check_dependency "dpkg -l build-essential >/dev/null 2>&1" "Please install 'build-essential'."
		#Check libreadline-dev if user didn't pass --disable-readline

		if [[ !disable_readline  ]]
		then
		    check_dependency "dpkg -l libreadline-dev >/dev/null 2>&1" "Please install 'libreadline-dev'."
		fi

		if [[ $dependency_problem -ne 0  ]]; then exit -1; fi

		read -p "Enter Default Directory for Shell(Default: '${DEFAULT_LOCATION}): " location
		if [[ -z $location ]]
		then
        		location="${DEFAULT_LOCATION}"
			break
		else
			read -p "Is this correct: ${location} -- (y/n): " option1
			if [[ "$option1" == "y"  || "$option1" == "yes" ]]
			then
				sed -i "s|${DEFAULT_LOCATION}|${location}|g" run.sh
				sed -i "s|${DEFAULT_LOCATION}|${location}|g" Src/globals.h
				sed -i "s|${DEFAULT_LOCATION}|${location}|g" Src/global_bash_var
				sed -i "s|${DEFAULT_LOCATION}|${location}|g" Bin/cmd_src/nano.sh
				break
			else
				#
				continue
			fi
		fi
	done
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
		disallow_shell_command "Bin/cmd_src/nano.sh"
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

	touch $config
fi
 
clear

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

