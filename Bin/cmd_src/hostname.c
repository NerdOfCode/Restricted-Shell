/*

	Gets hostname upon call

*/

#include <stdio.h>
#include <stdlib.h>
#include "../../Src/globals.h"

int main( void ){

	char *hostname = getenv(HOSTNAME);
	puts(hostname);

	return 0;
}
