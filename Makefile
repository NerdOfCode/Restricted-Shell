all: shell commands

shell:
	gcc -o Src/shell Src/shell.c


commands:
	gcc -o Bin/test Bin/cmd_src/test.c
	gcc -o Bin/clear Bin/cmd_src/clear.c
	gcc -o Bin/pwd Bin/cmd_src/pwd.c
