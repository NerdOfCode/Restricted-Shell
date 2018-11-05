#include <stdlib.h>
#include <stdio.h>
#include "../../Src/globals.h"

int main( void ){

	if(system("@cls||clear")==-1){
		puts(RED_TEXT"Could not process..."RESET);
		return -1;
	}

	return 0;
}
