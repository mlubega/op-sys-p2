#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char *argv[])
{
	char input[512];
	char exit[5] = "exit";
	exit[5] = '\0';

	while(1){
	
	int rc = fork();
	printf("mysh>");
	if(rc == 0){
		//parse input
		fgets(input, 512, stdin);
		//remove newline
		char *newline = strchr(input, '\n');
		if(newline != NULL){
			*newline = '\0'; //overwrite newline
		}

		//printf("input: %s\n", input);
		
		printf("strcmp %d", strcmp(input, "exit"));
		if(strcmp(input, "exit") == 0){
			printf("entered exit check");
//			exit(0);
		}
		//child
		char *argv[4];
		argv[0] = strdup("/bin/ls");
		argv[1] = strdup("-l");
		argv[2] = NULL;

		execvp(argv[0], argv);
		printf("exec failed");

	}
	else if(rc > 0){
		//parent
		(void) wait(NULL);
	}
	else{
		printf("error in calling fork()\n");
	}
	}
	return 0;

}
