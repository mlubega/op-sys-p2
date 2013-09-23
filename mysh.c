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
	//	input[strlen(input) - 1] = '\0';	

		int cch = strlen(input);
		printf("str len= %d\n", cch);
		printf("last char %c\n", input[cch-1]);
		printf("entered if");
		input[cch-1] = '\0';
		
		
		printf("input: %s", input);
		printf("strcmp %d", strcmp(input, exit));
		if(strcmp(input, exit) == 0){
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
