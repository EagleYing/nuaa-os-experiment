#include <stdio.h>  
#include <stdlib.h>  
#include <string.h> 
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h> 
  
#define MAX_SIZE 1024

char *argv[MAX_SIZE];
char source[MAX_SIZE];
  
int spilt_str(char *source, char *dist[])  
{  
	int i = 0;
    char *ptr;  
    char *p;    
    ptr = strtok(source, " ");  
    while (ptr != NULL) {   
		dist[i] = ptr;
		i++;
        ptr = strtok(NULL, " ");  
    }  
	return i;
}  

void shell_func(int argc, char *argv[])
{
	int i, random, pid, status;
	if(strcmp(argv[0], "echo") == 0){
		pid = fork();
		if(pid == 0){
			for(i=1; i<argc; i++){
				printf("%s ", argv[i]);
			}
			printf("\n");
			_exit(127);
		}else{
			wait(&status);
		}
	}else if(strcmp(argv[0], "ls") == 0){
		for(i = 1; i < argc; i++){
			pid = fork();
			if(pid == 0){
				printf("%d",i);
				printf("%s\n",argv[i]);
				execlp("/bin/ls", argv[0], argv[i], NULL);
				_exit(127);
			}else{
				wait(&status);
			}
		}
	}else if(strcmp(argv[0], "cd") == 0){
		//res = execlp("/usr/bin/cd", argv[0], argv[1], NULL, NULL);
		//cannot find directory include cd
		pid = fork();
		if(pid == 0){
			if(chdir(argv[1]) == -1){
				printf("cd:%s:no such directory\n", argv[1]);
				_exit(127);
			}else{
				_exit(127);
				//if no exit127 needs more than one exit
			}
		}else{
			wait(&status);
		}
	}else if(strcmp(argv[0], "pwd") == 0){
		pid = fork();
		char buf[MAX_SIZE];
		if(pid == 0){
			printf("%s\n", getcwd(buf, sizeof(buf)));
			_exit(127);
		}else{
			wait(&status);
		}
	}else if(strcmp(argv[0], "exit") == 0){
		random = rand()%100+1;
		_exit(random);
		//if using exit(i) it will exit the ternimal
	}else{
		printf("Error commond ! Try 'echo','ls','cd','pwd' and 'exit' to qiut\n");
	}
}

int main()
{
	while(1){
		printf("sh1>");
		//gets(source); dangerous
		fgets(source, MAX_SIZE, stdin);
		source[strlen(source)-1] = '\0';
		int argc;
		argc = spilt_str(source, argv);
		shell_func(argc, argv);
	}
	return 0;
}