#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc ){

        //If argc is greater or equal than 2 do not output pwd

        if(argc >= 2){
          return 0;
        }

        char buffer[64];

        getcwd(buffer,64);

        printf("%s\n",buffer);

	      return 0;
}
