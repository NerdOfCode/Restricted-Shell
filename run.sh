#!/bin/bash

#Author: NerdOfCode
#Purpose: Easily setup this Restricted-Shell

#Change to 0 to turn auto updates off
updates=1


echo "Updating repository..."

if [[ updates ]]
then
	git pull origin master
fi

make

cd Src/

clear

./shell

