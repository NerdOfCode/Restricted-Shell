all: shell commands

shell:
	rm -f Src/shell~

	gcc -o Src/shell Src/shell.c

commands:

	rm -f Bin/test~
	rm -f Bin/clear~
	rm -f Bin/pwd~
	rm -f Bin/cd~
	rm -f Bin/rm~

	gcc -o Bin/test Bin/cmd_src/test.c 
	gcc -o Bin/clear Bin/cmd_src/clear.c
	gcc -o Bin/pwd Bin/cmd_src/pwd.c 
	gcc -o Bin/cd Bin/cmd_src/cd.c
	gcc -o Bin/rm Bin/cmd_src/rm.c 
