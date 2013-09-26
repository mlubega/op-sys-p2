#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{

	typedef enum { false, true } bool;
	char input[512];
	char error_message[30] = "An error has occurred\n";
	//char debugCheck[5] = "check";
	char newlinechar[2] = "\n";
	bool redirect = false;
	bool redirError = false;
	

	while(1){
	printf("mysh>");
	
		fgets(input, 512, stdin);
		//remove newline
		char *newline = strchr(input, '\n');
		if(newline != NULL){
			*newline = '\0'; 
		}

		char *argv[256]; //can have at most 512 bytes; 256 space-delimited char 
		char *redirArray[256];

		
		//no redir
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
		
		//check for redirection
		int i;
		for(i = 0; i < inputCount; i++){
		
			char *greaterthan = strchr(input, '>');
			//break up string with '>'
			if(greaterthan != NULL){
				//write(STDERR_FILENO, debugCheck, strlen(debugCheck));
				
				if(redirect == true){ //two '>' found
					redirError = true;
					write(STDERR_FILENO, error_message, strlen(error_message));
				}
				else{	//> found; break up string
					
					int redirCount = 1;
					char* redirToken;
					redirToken = strtok(input, ">");
					
					redirArray[0] = redirToken;
					while(redirToken != NULL){
						redirToken = strtok(NULL, ">");
						redirArray[redirCount] = redirToken;
						redirCount++;
					}
					
					//if no commands stuck onto '>'
					
					//if commands stuck together
					//put array back together
					argv[i] = redirArray[0]; //connected command
					if(argv[i + 1] != NULL){	//file name not connected
						argv[i + 1] = NULL;	//take out file name
					}
					
					int len = strlen(redirArray[1]);
					char outputFileName[(len)];
					strcpy(outputFileName, redirArray[1]);
					
					if(redirArray[2] != NULL){
						redirError = true;
						write(STDERR_FILENO, error_message, strlen(error_message));
					}
					
					write(STDOUT_FILENO, newlinechar, strlen(newlinechar)); 
					write(STDOUT_FILENO, argv[0], strlen(argv[0]));
		  			write(STDOUT_FILENO, newlinechar, strlen(newlinechar)); 					write(STDOUT_FILENO, argv[1], strlen(argv[1]));
		 			write(STDOUT_FILENO, newlinechar, strlen(newlinechar)); 					write(STDOUT_FILENO, outputFileName, strlen(outputFileName));

				}
			}//end if greater than sign
		}//end for

		//BUILT-IN COMMANDS
		  if(argv[1] == NULL && strcmp(argv[0], "exit") == 0){
			exit(0);
		  }
		  else if(argv[1] == NULL && strcmp(argv[0], "pwd") == 0){
		  char str[512];
		  getcwd(str, 512);

		  write(STDOUT_FILENO, str, strlen(str));
		  write(STDOUT_FILENO, newlinechar, strlen(newlinechar)); 
		  }
		  else if(argv[1] == NULL && strcmp(argv[0], "wait") == 0){
			//TODO
		  }
		  else if(argv[1] == NULL && strcmp(argv[0], "cd") == 0){
		  char *homename;
		  homename = getenv("HOME");
		  chdir(homename);
		  write(STDOUT_FILENO, homename, strlen(homename));
		  }
		  
		  //cd with an arg
		  else if(argv[1] != NULL && argv[2] == NULL &&  
		  		strcmp(argv[0], "cd") == 0) {
		    char *pathname;
		    pathname = argv[1];
		    if( chdir(pathname) == -1){
		    	 write(STDERR_FILENO, error_message, strlen(error_message));
		    }
		}
		else if(!redirError){
		

		//BASIC SHELL COMMANDS
		int rc = fork();
		if(rc == 0){

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
	  }//end else
	}// end while
	return 0;
//modifying for test
}
