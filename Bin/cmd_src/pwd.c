#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Src/globals.h"

int main( int argc ){

        //If argc is greater or equal than 2 do not output pwd

	//Testing if user has correct access to pwd
        if(argc >= 2){
        	return 0;
        }

        char buffer[64];

        if(getcwd(buffer,64) == NULL){
		puts(RED_TEXT"Unable to get directory!"RESET);
		return -1;
	}

        printf("%s\n",buffer);

	return 0;
}
