#include <stdio.h>
#include <string.h>
#include "../../Src/globals.h"

int main(int argc, char **argv){

	int remove_status = 0;

	remove_status = remove(argv[1]);

	if ( remove_status != 0 ){
		puts(RED_TEXT"An error has occurred..."RESET);
		return -1;
	}

	return 0;
}
