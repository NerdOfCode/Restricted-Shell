#include <stdio.h>
#include <unistd.h>
#include "../../Src/globals.h"

int main(void){
	char *username = getlogin();

	if(username == NULL){
		puts(RED_TEXT"Error: Unable to Receive Username"RESET);
	}else{
		printf("%s\n",username);
	}

	return 0;
}
