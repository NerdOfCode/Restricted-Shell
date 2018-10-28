#!/bin/bash

#Safely go to parent directory, then perform a reinstall via Git

if [[ "$1" == "--help" ]]
then
        echo "This script reinstalls the Restricted-Shell..."
        echo "This script creates a clone of the current Restricted-Shell in ~/.r-shell/clones/{RANDOM}"
        exit 0
fi

root_check(){

        if [[ "$1" == "--no-root" ]]
        then
                return 
        fi

        if [[ "$(whoami)" != "root" ]]
        then
                echo "Error: Not Root!"
                echo "Pass '--no-root' if you're willing to risk it."
                exit -1
        fi


}

root_check $@

RANDOM_NUMB=$RANDOM

cd ../

mkdir -p ~/.r-shell/clones/

cp -r Restricted-Shell/ ~/.r-shell/clones/$RANDOM_NUMB

sudo rm -rf Restricted-Shell/*

git clone https://github.com/NerdOfCode/Restricted-Shell.git Restricted-Shell/ --quiet

cd Restricted-Shell/

#Reset Git completely 

#git reset --hard REV

#clear

echo "Please run ./run.sh"
echo "Cloned old R-Shell config in ~/.r-shell/clones/$RANDOM_NUMB"

exit 0
