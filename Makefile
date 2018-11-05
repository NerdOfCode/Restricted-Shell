C=	gcc -std=gnu11 -O

all: clean shell commands clone_bash

clean:
#Remove all binaries and also ignore error of trying to "rm" a directory
	-rm Bin/*

shell:
	rm -f Src/shell~

	$C -o Src/shell Src/shell.c -lreadline

commands:

	$C -o Bin/test Bin/cmd_src/test.c 
	$C -o Bin/clear Bin/cmd_src/clear.c
	$C -o Bin/pwd Bin/cmd_src/pwd.c 
	$C -o Bin/cd Bin/cmd_src/cd.c
	$C -o Bin/rm Bin/cmd_src/rm.c 
	$C -o Bin/whoami Bin/cmd_src/whoami.c
	$C -o Bin/hostname Bin/cmd_src/hostname.c
	$C -o Bin/date Bin/cmd_src/date.c
	$C -o Bin/flags Bin/cmd_src/flags.c
	$C -o Bin/history Bin/cmd_src/history.c
	$C -o Bin/alias Bin/cmd_src/alias.c
	$C -o Bin/mkdir Bin/cmd_src/mkdir.c
	$C -o Bin/version Bin/cmd_src/version.c
	$C -o Bin/cat Bin/cmd_src/cat.c

clone_bash:
	cp -f Bin/cmd_src/ls.sh Bin/ls
	cp -f Bin/cmd_src/nano.sh Bin/nano
#Make copied shell script executable
	chmod 755 Bin/ls
	chmod 755 Bin/nano
