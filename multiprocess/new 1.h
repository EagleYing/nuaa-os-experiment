#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_SIZE 1024

char *argv[MAX_SIZE];
char source[MAX_SIZE];
char para[MAX_SIZE];
char pip[MAX_SIZE];
  
int spilt_str(char *source, char *dist[])  
{  
	int i = 0,tmp;
    char *ptr;  
    char *p;    
    ptr = strtok(source, "|");  
    while (ptr != NULL) {   
		dist[i] = ptr;
		i++;
        ptr = strtok(NULL, "|");  
    }  
	ptr = strtok(argv[0], " ");
	ptr = strtok(NULL, " ");
	strcpy(para, ptr);
	strcat(argv[0], " ");
	strcat(argv[0], para);
	return i;
}  

int main()
{
	int i;
	int fd[2];
	pid_t pid;
	char buf[MAX_SIZE];
	
	printf("sh1>");
	fgets(source, MAX_SIZE, stdin);
	source[strlen(source)-1] = '\0';
	int argc;
	argc = spilt_str(source, argv);

	pipe(fd);
	for(i = 0; i < argc - 1; i++){
		pid = fork();
		if(pid == 0){
			//input
			close(fd[1]);
			read(fd[0], pip, sizeof(pip));//do not need
			close(fd[0]);
			execl("/bin/sh", "sh", "-c", argv[i], (char *)0);
			printf("%s ", argv[i]);
		}else{
			close(fd[0]);
			strcat(argv[i + 1], " ");
			strcat(argv[i + 1], para);
			write(fd[1], para, sizeof(para));//do not need
			close(fd[1]);
			execl("/bin/sh", "sh", "-c", argv[i + 1], (char *)0);
			printf("%s ", argv[i + 1]);
		}
	}
	return 0;
}