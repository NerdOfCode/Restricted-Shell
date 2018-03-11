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
fi

make

cd Src/

clear

./shell

