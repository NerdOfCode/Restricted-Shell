#!/bin/bash 

#Author: NerdOfCode
#Purpose: Easily setup this Restricted-Shell

#Change to 0 to turn auto updates off
updates=0

config=".config"

if [[ "$updates" == "1" ]]
then
	echo "Updating repository..."
	git pull origin master
fi

#Shortcut to make sure all shell scripts are in fact executable
chmod +x Bin/*

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
		dissallow_c_command "Bin/pwd"

	fi

	touch $config
fi

make

cd Src/

clear

#if [ ! -f Src/shell ]
#then
#	echo "An error occurred in compiling shell.c ... Please scroll up for errors"
#	exit -1
#fi

./shell
