#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char *argv[])
{

	int rc = fork();

	if(rc == 0){
		//child
		char *argv[4];
		argv[0] = strdup("/bin/ls");
		argv[1] = strdup("-l");
		argv[2] = "NULL";

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

	return 0;

}
