#!/bin/bash

#Author: NerdOfCode
#Purpose: Easily setup this Restricted-Shell

#Change to 0 to turn auto updates off
updates=0

config=".config"

echo "Updating repository..."

if [[ "$updates" == "1" ]]
then
	git pull origin master
fi

#Create function
disallowed_bash(){
	echo "$(sed 's/^/#/' Bin/ls)" > $1
	echo "echo \"Command is disallowed by admin...\"" >> $1
}

disallowed_c(){
	contents="$(cat $1)"
	echo "/*" > $2
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
		disallowed_bash Bin/ls
	fi

	echo -e "\nThe 'pwd' command 'prints working directory'..."
	read -p "Would you like to allow the 'pwd' command(y/n): " option1

	if [[ "$option1" != "y" ]]
	then
		disallowed_c Bin/cmd_src/pwd.c

	fi

	touch $config
fi

make

cd Src/

clear

./shell
