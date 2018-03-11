/*

Author: NerdOfCode
Updated: 3/10/18

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( void ){

        char buffer[64];

        getcwd(buffer,64);

        printf("%s\n",buffer);

	return 0;
}




