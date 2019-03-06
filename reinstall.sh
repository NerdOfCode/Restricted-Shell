#!/bin/bash 
: '      
         This file is part of the Restricted-Shell distribution (https://github.com/NerdOfCode/Restricted-Shell).
         Copyright (c) 2019 NerdOfCode.
  
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
# Author: NerdOfCode
# Purpose: Perform a reinstallation of this Shell
# Updated: 1/30/18

# Location of where to automatically backup current configuration upon reinstall
# Assume that the location starts with --> ~/
readonly clone_location=".r-shell/clones/"

# ANSI color text #
readonly red_text="\033[0;31m"
readonly yellow_text="\033[0;33m"
readonly green_text="\033[0;32m"
readonly reset_text="\033[0m"

root_check(){
    if [[ "$(whoami)" != "root" ]] && [[ "$no_root" != true ]]
    then
        echo -e "${red_text}Error:${reset_text} unprivileged user"
        echo -e "\t${yellow_text}Note:${reset_text} Pass '--no-root' to bypass this"
        exit 2
    fi
}

## Generate the psuedo-random parent directory name of the actual old cloned config ##
create_config_backup(){
    # Absolute file path to the config
    local absolute_config_backup_location="${clone_location}$RANDOM/"
    if [[ -d ~/$clone_location ]]
    then
	if [[ ! -d ${absolute_config_backup_location} ]]
	then
	    echo -e "${yellow_text}Note:${reset_text} Cloning contents..."
	    mkdir -p ~/${absolute_config_backup_location}
	    cp -r ../Restricted-Shell/ ~/${absolute_config_backup_location}
	else
	    # A directory name collision has occured... Most likely :)
	    create_config_backup
	fi
    else
	echo -e "${yellow_text}Note:${reset_text} Creating config backup directory @ [~/${clone_location}]"
	mkdir -p ~/$clone_location
	
	if [[ -d ~/$clone_location ]]
	then
	    echo -e "${yellow_text}Note:${reset_text} Creating current config backup directory @ [~/${absolute_config_backup_location}]"
	    mkdir -p ~/$absolute_config_backup_location
	else
	    echo -e "${red_text}Error:${reset_text} Creating directory @ [~/${clone_location}]"
	    exit 2
	fi 
    fi
    echo -e "${green_text}Success:${reset_text} Created backup @ [~/${absolute_config_backup_location}]"
}

while [[ $# -gt 0 ]]
do
    case "$1" in
	-h | --help)
	    echo "This script reinstalls the Restricted-Shell..."
	    echo ""
	    echo "Current arguements include:"
	    echo -e "\t\t-h/--help --> Display this message."
	    echo -e "\t\t-nr/--no-root --> Run as non-privileged user."
	    exit 0
	    ;;
	-nr | --no-root)
	    echo -e "${yellow_text}Note:${reset_text} Running as non-privileged user"
	    no_root=true
	    ;;
	-nb | --no-backup)
	    echo -e "${red_text}Warning:${reset_text} Not creating backup"
	    no_backup=true
	    ;;
    esac
    shift
done

# Let's check the permissions of the running user
root_check

# Let's backup the current config, unless the user has specified else...
if [[ "$no_backup" != true ]]
then
    create_config_backup
else
    # Proceed with reinstall - No backup
    :
fi

read -p "Ready to reinstall(y/n): " yesno
while true
do
    if [[ "$yesno" == "y" ]] || [[ "$yesno" == "Y" ]]
    then
	# Ready to reinstall
	break
    elif [[ "$yesno" == "n" ]] || [[ "$yesno" == "N"  ]]
    then
	# Not quite
	echo -e "${yellow_text}Note:${reset_text} Exiting now"
	exit 0
	break
    else
	# Unrecognized option
	echo -e "${yellow_text}Note:${reset_text} Unrecognized option $yesno"
	continue
    fi
done

cd ../ && rm -rf Restricted-Shell/
git clone https://github.com/NerdOfCode/Restricted-Shell.git --quiet

if [[ -d "Restricted-Shell/"  ]]
then
    echo -e "${green_text}Success:${reset_text} Reinstalled"
else
    echo -e "${red_text}Error:${reset_text} An unknown problem has occured while cloning"
fi

# Reset previously directory setting
cd -
