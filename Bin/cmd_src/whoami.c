#include <stdio.h>
#include <unistd.h>
#include "../../Src/globals.h"

int main(int argc){

	if(argc >= 2){
		return 1;
	}

	char *username = getlogin();

	if(username == NULL){
		puts(RED_TEXT"Error: Unable to Receive Username"RESET);
	}else{
		printf("%s\n",username);
	}

	return 0;
}
