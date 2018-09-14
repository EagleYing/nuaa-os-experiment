#include <stdio.h>  
#include <stdlib.h>  
#include <string.h> 
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h> 
#include <fcntl.h>
  
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
		//printf("ptr=%s\n", dist[i]);
		i++;
        ptr = strtok(NULL, " ");  
    }  
	return i;
}  

void shell_func(int argc, char *argv[])
{
	int i, j, random, flag = 0, tmp = 0, pid, status;
	char buf[MAX_SIZE];
	if(strcmp(argv[0], "echo") == 0){
		pid = fork();
		if(pid == 0){
			for(i=1; i<argc; i++){
				if(strcmp(argv[i], ">") == 0){
					flag = 1;
					tmp = i;
				}
			}
			//printf("%d %d\n",flag,tmp);
			if(flag == 0){
					for(i=1; i<argc; i++){
						//printf("%s ", argv[i]);
						write(1,argv[i],strlen(argv[i]));
						write(1," ",1);
					}
					printf("\n");
			}else if(flag == 1){
				//printf("%s\n",argv[tmp + 1]);
					FILE *fp = fopen(argv[tmp + 1], "w+");	
					for(j=1; j<tmp; j++){
						strcpy(buf, argv[j]);
						strcat(buf, " ");
						fwrite(buf, strlen(argv[j]) + 1, 1, fp);
					}
					fclose(fp);
			}
		_exit(127);
		}else{
			wait(&status);
		}
	}else if(strcmp(argv[0], "ls") == 0){
		pid = fork();
		if(pid == 0){
			execlp("/bin/ls", argv[0], argv[1], NULL, NULL, NULL);
			_exit(127);
		}else{
			wait(&status);
		}
	}else if(strcmp(argv[0], "cd") == 0){
		//res = execlp("/usr/bin/cd", argv[0], argv[1], NULL, NULL);
		//cannot find directory include cd
			if(chdir(argv[1]) == -1){
				printf("cd:%s:no such directory\n", argv[1]);
			}
	}else if(strcmp(argv[0], "pwd") == 0){
	        pid = fork();
			if(pid == 0){
				char buf[MAX_SIZE];
				printf("%s\n", getcwd(buf, sizeof(buf)));
				_exit(127);
			}else{
				wait(&status);
			}	
	}else if(strcmp(argv[0], "exit") == 0){
		random = rand()%100+1;
		_exit(random);
		//if using exit(i) it will exit the ternimal
	}else if(strcmp(argv[0] ,"cat") == 0){
		    pid = fork();
			if(pid == 0){
				int size,fd;
				//FILE *fp = fopen(argv[1], "r");
				fd = open(argv[1], O_RDONLY);
				if(fd == 0){
					printf("No such file!\n");
				}else{
					size = read(fd, buf, sizeof(buf));
					write(1, buf, size);
					printf("\n");
					close(fd);
				}
			_exit(127);
			}else{
				wait(&status);
			}		
	}else{
		printf("Error commond ! Try 'echo','ls','cd','pwd' and 'exit' to qiut\n");
	}
}

int main()
{
	while(1){
		printf("sh2>");
		//gets(source); dangerous
		fgets(source, MAX_SIZE, stdin);
		source[strlen(source)-1] = '\0';
		int argc;
		argc = spilt_str(source, argv);
		shell_func(argc, argv);
	}
	return 0;
}