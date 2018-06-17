/*

	Purpose: Designed to show the user any global settings that directly affect them
	Usage: flags

*/

#include <stdio.h>
#include "../../Src/globals.h"

int main( void ){

	if(LOGGING == 1){
		printf("Logging: ");
		printf(RED_TEXT"On\n"RESET);
	}else{
		puts("Logging: Off");
	}

	//TODO
	//Add other global flags as they're added to this shell

	return 0;

}
