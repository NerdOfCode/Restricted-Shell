#include <stdio.h>
#include <time.h>

int main( int argc, char *argv[] ){

	time_t current_time;
	time(&current_time);

	printf("The local time is: %s",ctime(&current_time));

	return 0;
}
