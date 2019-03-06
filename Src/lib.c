#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Function:
    char *remove_char_until

  Parameters:
    *specified_buffer => Pointer towards string
    remove_char[] => The character to remove up until

  Status:
    Deprecated 3/5/2019
 */

char *remove_char_until(char *specified_buffer,char remove_char[]){
  char specified_buffer_copy[512] = "";
  static char remove_char_result[512] = "";

  // Get rid of any previous results
  memset(remove_char_result,0,sizeof(remove_char_result));
  
  strncpy(specified_buffer_copy,specified_buffer,strlen(specified_buffer));
 
  int highest = 0, i = 0;

  for(; i <= strlen(specified_buffer_copy) - 1; i++)
    if(specified_buffer_copy[i] == remove_char[strlen(remove_char) - 1])
      highest = i;
    
  
  i=0;
  for(; highest <= strlen(specified_buffer_copy) - 1; highest++,i++)
    remove_char_result[i] = specified_buffer_copy[highest];

  return remove_char_result;
}
