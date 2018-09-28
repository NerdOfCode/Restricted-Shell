#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){

	char buffer[64];
	char dir[128] = "";

	if(argc < 2){
		puts("Please provide an argument: ");
		puts("	-->mkdir Hello-World/");
		return 1;
	}

        getcwd(buffer,64);

	strcat(dir,buffer);

	strcat(dir, "/");

	strcat(dir, argv[1]);

	mkdir(dir, 0755);

	return 0;

}
