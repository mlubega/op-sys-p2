#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char *argv[])
{
	char input[512];
	char error_message[30] = "An error has occurred\n";

	while(1){
	printf("mysh>");
	
		fgets(input, 512, stdin);
		//remove newline
		char *newline = strchr(input, '\n');
		if(newline != NULL){
			*newline = '\0'; 
		}

		//printf("input: %s\n", input);

		//BUILT-IN COMMANDS
		if(strcmp(input, "exit") == 0){
			exit(0);
			//goes to error  message
		}
		if(strcmp(input, "pwd") == 0){
		  char str[512];
		  getcwd(str, 512);
		  write(STDOUT_FILENO, str, strlen(str));
		}
		if(strcmp(input, "wait") == 0){


		}
		if(strcmp(input, "cd") == 0){
		  char *homename;
		  homename = getenv("HOME");
		  write(STDOUT_FILENO, homename, strlen(homename));
		  // prints (/u/m/a/maimunaAn error has occurred)
		}

		//Question:  will below this need to be in a if/elseif/else to prevent
		//from printing the error message when all we have done is 
		// process a built-in command?


		//BASIC SHELL COMMANDS
		int rc = fork();
		if(rc == 0){
	
		//child
		char *argv[256]; //can have at most 512 bytes; 256 space-delimited char 
	//	argv[0] = strdup("/bin/ls");
	//	argv[1] = strdup("-l");
	//	argv[2] = NULL;

		//working on parsing input:
		int inputCount = 1;
		char* token;
		token = strtok(input, " ");
		argv[0] = token;
		while(token != NULL){
			token = strtok(NULL, " ");
			argv[inputCount] = token;
			inputCount++;
		}
		argv[inputCount] = NULL;
		execvp(argv[0], argv);
		write(STDERR_FILENO, error_message, strlen(error_message));

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
//modifying for test
}
