#!/bin/bash

#Author: NerdOfCode
#Purpose: Easily setup this Restricted-Shell

#Change to 0 to turn auto updates off
updates=1

config=".config"

echo "Updating repository..."

if [[ updates ]]
then
	git pull origin master
fi


if [[ ! -f $config ]]
then

	clear
	#Prompt user to allow what commands
	echo -e "Please choose what commands to allow your users to use below: \n"

	echo "The 'ls' command basically lists files and folders in a directory"
	read -p  "Would you like to allow the 'ls' command(y/n): " option1

	if [[ "$option1" == "n" ]]
	then
		echo "$(sed 's/^/#/' Bin/ls)" > Bin/ls
		echo "echo \"Command is disallowed by admin...\"" >> Bin/ls
	fi

	echo -e "\nThe 'pwd' command 'prints working directory'..."
	read -p "Would you like to allow the 'pwd' command(y/n): " option1

	if [[ "$option1" == "n" ]]
	then
		contents="$(cat Bin/cmd_src/pwd.c)"
		echo "/*" > Bin/cmd_src/pwd.c
		echo "$contents" >> Bin/cmd_src/pwd.c
		echo "*/" >> Bin/cmd_src/pwd.c
		echo "#include <stdio.h>" >> Bin/cmd_src/pwd.c
		echo "int main(void){" >> Bin/cmd_src/pwd.c
		echo "printf(\"Command disallowed by admin...\n\");}" >> Bin/cmd_src/pwd.c

	fi

	touch $config
fi

make

cd Src/

clear

./shell

