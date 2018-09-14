#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

int mysys(char *parameter)
{
    /****************************************************
    *imolement this func without using system
    *printf("--------------------------------------");
    *system("echo HELLO WORLD");
	*printf("--------------------------------------");
	*system("ls /");
	*printf("--------------------------------------");
	*****************************************************/
    pid_t pid;
	int status;
    //printf("%d ", getpid());
    //printf("%d ",fork());
	if(parameter == NULL){
		return 1;
	}

	pid = fork();
	if(pid < 0){
		status = -1;//fail to fork
	}else if(pid == 0){
		//Normal subprocesses
		execl("/bin/sh", "sh", "-c", parameter, (char *)0);
		//Unnormal exection of subprocesses
		_exit(127);
	}else{
		while(waitpid(pid, &status, 0) < 0){
			if(errno != EINTR){
				status = -1;
				break;
			}
		}
	}
	return status;
}

int main()
{
	printf("-------------------------------------------------------------\n");
	mysys("echo hello world");
	printf("-------------------------------------------------------------\n");
	mysys("ls /");
	printf("-------------------------------------------------------------\n");
	return 0;
}
