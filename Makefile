all: shell commands

shell:
	if [ -a Src/shell ]; then rm Src/shell; fi
	gcc -o Src/shell Src/shell.c


commands:
	if [ -a Bin/test ]; then rm Bin/test; fi
	if [ -a Bin/clear ]; then rm Bin/clear; fi
	if [ -a Bin/pwd ]; then rm Bin/pwd; fi


	gcc -o Bin/test Bin/cmd_src/test.c
	gcc -o Bin/clear Bin/cmd_src/clear.c
	gcc -o Bin/pwd Bin/cmd_src/pwd.c
