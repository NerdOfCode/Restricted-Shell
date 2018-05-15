#!/bin/bash 

#Author: NerdOfCode
#Purpose: Easily setup this Restricted-Shell

#Change to 0 to turn auto updates off
updates=0

config=".config"

#Change to 1 if you don't want to recompile if built
dont_compile=0

if [[ "$updates" == "1" ]]
then
	echo "Updating repository..."
	git pull origin master
fi

#Shortcut to make sure all shell scripts are in fact executable
chmod +x Bin/*

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

check_exec

if [[ ! -f $config ]]
then

	clear
	#Prompt user to allow what commands
	echo -e "Please choose what commands to allow your users to use below: \n"

	echo "The 'ls' command basically lists files and folders in a directory"
	read -p  "Would you like to allow the 'ls' command(y/n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallow_shell_command "Bin/ls"
	fi

	echo -e "\nThe 'pwd' command 'prints working directory'..."
	read -p "Would you like to allow the 'pwd' command(y/n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallow_c_command "Bin/cmd_src/pwd.c"

	fi

	touch $config
fi

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

