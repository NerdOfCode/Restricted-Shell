all: shell commands

shell:
	gcc -o Src/shell Src/shell.c


commands:
	gcc -o Bin/test Bin/cmd_src/test.c
