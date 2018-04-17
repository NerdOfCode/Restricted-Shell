#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//If directory change success, return 0 and write to file
int write_change();

int main(int argc, char *argv[]){

	int result;
	char *copy = argv[1];

	if( argc > 1 ){
		result = chdir(copy);
	}else{
		puts("Usage: cd [dir here]");
		return -1;
	}

	//If successfully changes dir
	if(result == 0){
		write_change(copy);
	}else{
		puts("Directory not found!");
	}


	return 0;
}

int write_change(char *copy){
	FILE *fptr = fopen("Logs/cwd.db","w");

	//Protect against errors
	if(fptr == NULL){
		puts("File Access Error[FAE]...");
		puts("Error 1001...");
		return -1;
	}

	//Print the new dir to our file
	fprintf(fptr,"%s",copy);
	fclose(fptr);

	return 0;
}