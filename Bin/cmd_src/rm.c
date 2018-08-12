#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h> /* PATH_MAX */
#include "../../Src/globals.h"

int main(int argc, char **argv){

	//Will hold the exit value of the remove() function
	int remove_status = 0;

	//Add trailing null to PATH_MAX
	char buffer[PATH_MAX - 1];

	char *path = realpath(argv[1],buffer);

	//Check to make sure user isn't removing any of the shells files

	//Path exists
	if(path){

		if(strstr(buffer,DEFAULT_LOCATION) != NULL){

			puts(RED_TEXT"Access Denied: 1004"RESET);
			exit(EXIT_FAILURE);

		}else{

			remove_status = remove(buffer);

		}

	} else {
		puts(RED_TEXT"Error 1005"RESET);
		//Print to stderr
		perror("realpath");
		exit(EXIT_FAILURE);
	}
	
	if ( remove_status != 0 ){

		puts(RED_TEXT"An error has occurred..."RESET);
		exit(EXIT_FAILURE);

	}

	return 0;
}
