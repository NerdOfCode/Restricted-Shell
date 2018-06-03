/*

	Display local time of system

*/


#include <stdio.h>
#include <time.h>

int main( void ){

	time_t current_time;
	time(&current_time);

	printf("The local time is: %s\n",ctime(&current_time));

	return 0;
}
