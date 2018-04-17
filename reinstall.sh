#!/bin/bash

mv $0 ../

rm -rf *
rm -rf .*

git clone git@github.com:NerdOfCode/Restricted-Shell.git .

clear

echo "Please run ./run.sh"

exit 0
