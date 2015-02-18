#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>


int main(int argc, char *argv[])
{

	typedef enum { false, true } bool;
	char input[512];
	char *fileName;
	char error_message[30] = "An error has occurred\n";
	//char check[5] = "check";
	char newlinechar[2] = "\n";
	bool redirect;
	bool runinbkgd = false;
	char *pypath = "/usr/bin/python";
	char *batchFileName;
	FILE *batchFile = NULL;
	bool batchOpened;
	batchOpened = false;
	char *line;

	while(1){
		//reset global vars
		fileName = NULL;
		redirect = false;

		//not in batch mode
		if(argc == 1){
			write(STDOUT_FILENO, "mysh> ", strlen("mysh> "));
			// recieve input line
			fgets(input, 520, stdin);
		//	if(read(STDIN_FILENO) == 0){ //found EOF
			if(feof(stdin)){	//check for eof
				exit(0);
				return 0;
			}
			//write(STDOUT_FILENO, input, strlen(input));
		}
		else if(argc == 2){	//batch mode
			//do once; open batch file

			if(!batchOpened){
				batchFileName = argv[1];
				batchFile = fopen(batchFileName, "r");
				if(batchFile == NULL){
					write(STDERR_FILENO,
					error_message, strlen(error_message));	
					return 1;;
				}
				else{
					batchOpened = true;
				}
			}
			
			line = fgets(input, 520, batchFile);
		
			if(line == NULL){
				exit(0);
				return 0;
			}
			else{
				if(strcmp(line, "\n") != 0){
				write(STDOUT_FILENO, line, strlen(line));
				}
			}
		}//end else batch mode
		else if(argc > 2){	//incorrect #args
			write(STDERR_FILENO, error_message, strlen(error_message));	

			return 1;
		}


		if(strlen(input) > 513){
			write(STDERR_FILENO, error_message, strlen(error_message));
			
			continue;
		}

		//remove newline from input
		char *newline = strchr(input, '\n');
		if(newline != NULL){
			*newline = '\0';
		}


		char *argArr[256]; //can have at most 512 bytes; 256 space-delimited char 

	
	
		//check for redir
		char *greaterthan = strchr(input, '>');
		if(greaterthan != NULL){
			redirect = true;
			*greaterthan = '\0';
			fileName = (greaterthan + 1);
			
			//parse file name
			char* fileToken;
			fileToken = strtok(fileName, " ");
			fileName = fileToken;
			char* extraToken = strtok(NULL, " ");
			if(extraToken != NULL){
				if(strcmp(extraToken, "&") != 0){
				
				write(STDERR_FILENO, error_message, strlen(error_message));
				continue;
				}
			}
			
			char *ampersand = strchr(fileName, '&');
		
			if(ampersand != NULL){
				runinbkgd = true;
	//		strtok(argArr[inputCount - 2], "&");
				*ampersand = '\0';
			}
			
			
		}
		
		//parse command
		int inputCount = 1;
		char* token;
		token = strtok(input, " ");
		argArr[0] = token;
		if (argArr[0] == NULL){
			continue;
		}
		while(token != NULL){
			token = strtok(NULL, " ");
			argArr[inputCount] = token;
			inputCount++;
		}
		

		argArr[inputCount] = NULL;
		//check for python script
		char *pyarray[256];
		bool ispyscript = false;
		
		if( strstr(argArr[0], ".py\0") != NULL){
			pyarray[0] = "python";
			int i;
			ispyscript = true;
			for ( i = 0; i <= inputCount; i++){
				pyarray[i+1] = argArr[i];
			} 
		
		
		}
		//check for background job
		if(strcmp(argArr[inputCount -2], "&") == 0){
			runinbkgd = true;
			argArr[inputCount -2] = NULL;
		}

		//BUILT-IN COMMANDS
		  if(argArr[1] == NULL && strcmp(argArr[0], "exit") == 0){
			exit(0);
			return 0;
		  }
		  else if(strcmp(argArr[0], "pwd") == 0){
		      if(argArr[1] == NULL){
		      char str[512];
			  getcwd(str, 512);

			  write(STDOUT_FILENO, str, strlen(str));
			  write(STDOUT_FILENO, newlinechar, strlen(newlinechar)); 
		      }
		      else{
		    	 write(STDERR_FILENO, error_message, strlen(error_message));
			continue;
		      }
		 }
		  else if(argArr[1] == NULL && strcmp(argArr[0], "wait") == 0){
		  	pid_t pid;
		  	while(pid = waitpid(-1, NULL, 0)){
				if(errno == ECHILD){
					break;
				}
			}
		  }
		  else if(argArr[1] == NULL && strcmp(argArr[0], "cd") == 0){
		  char *homename;
		  homename = getenv("HOME");
		  chdir(homename);
		 // write(STDOUT_FILENO, homename, strlen(homename));
		  }
		  
		  //cd with an arg
		  else if(argArr[1] != NULL && argArr[2] == NULL &&  
		  		strcmp(argArr[0], "cd") == 0) {
		    char *pathname;
		    pathname = argArr[1];
		    if( chdir(pathname) == -1){
		    	 write(STDERR_FILENO, error_message, strlen(error_message));
			 continue;   
		   }
		}
		else{
		

		//BASIC SHELL COMMANDS
		int rc = fork();
		if(rc == 0){
		//set up redirection
			if(redirect){
				close(STDOUT_FILENO);
				int status = open(fileName, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
				if(status == -1){
					write(STDERR_FILENO, error_message,
						strlen(error_message));
					continue;
				}
			}
			if(ispyscript){
				execvp(pyarray[0], pyarray);
			}else{
			/*
				for(arrCount = 0; arrCount < inputCount-1;
					arrCount++){
					write(STDOUT_FILENO, argArr[arrCount], 
						strlen(argArr[arrCount]));
				}
	*/
			//	write(STDERR_FILENO, check, strlen(check));
				execvp(argArr[0], argArr);
			}
			write(STDERR_FILENO, error_message, strlen(error_message));
			
		}
		else if(rc > 0){
			//parent
			if(!runinbkgd){
				(void) wait(NULL);
			//	runinbkgd = false;
			}
			runinbkgd = false;
		}
		else{	//fork error
		
			write(STDERR_FILENO, error_message, strlen(error_message));
		}
		
	  }//end else
	
	}// end while
	
	if(batchOpened){
	 	fclose(batchFile);
	
}
	return 0;
//modifying for test
}
