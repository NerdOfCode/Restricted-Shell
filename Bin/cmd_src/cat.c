#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../Src/globals.h"

#define LBUFFER 4096

int file_exists();

int main(int argc, char *argv[]){

  FILE *fptr = fopen(argv[1],"r");

  if(file_exists(argv[1])){
    char line[LBUFFER] = "";
    while(fgets(line, LBUFFER, fptr)){
      printf("%s",line);
    }
  }else{
    puts(RED_TEXT"File Not Found!"RESET);
  }

  fclose(fptr);
	
  return 0;
}


// Values:
// 1   => File exists
// 0  => File does not exists
int file_exists(char *filename){
  if(access(filename, F_OK) != -1){
    //File exists
    return 1;
  }else{
    return 0;
  }

}
