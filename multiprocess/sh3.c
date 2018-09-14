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
	int i = 0; 
    char *ptr;  
    char *p;    
    ptr = strtok(source, "|");  
    while (ptr != NULL) {  
		dist[i] = ptr;
		i++;
        ptr = strtok(NULL, "|");  
    }  
	printf("%s",para);
	return i;
}  

int main()
{
	int i,status,pid,pid_out,p2;
	int fd[2];
	char buf[6],file[1024],file2[1024],para[1024];
	char *op[1024],*f;
	
	printf("sh1>");
	fgets(source, MAX_SIZE, stdin);
	source[strlen(source)-1] = '\0';
	int argc;
	argc = spilt_str(source, argv);
	//printf("%d\n",argc);
	
	/*for(i=0; i<argc; i++){
		printf("%s ", argv[i]);
		printf("\n");
	}*/
	//printf("%s\n",argv[argc]);
	
	char *ptr;
	char *p;
    ptr = strtok(argv[0], "<");  
    while (ptr != NULL) {  
		strcpy(file,ptr);
		i++;
        ptr = strtok(NULL, "<");  
    }  
	
	ptr = strtok(file, " ");  
	strcpy(file,ptr);
	//printf("ptr:%s\n",file);
	
	ptr = strtok(argv[argc-1], ">");  
    while (ptr != NULL) {  
		strcpy(file2,ptr);
		i++;
        ptr = strtok(NULL, ">");  
    }  
	
	ptr = strtok(file2, " ");  
	strcpy(file2,ptr);
	//printf("ptr:%s\n",file2);
	
	op[0] = "cat ";
	//printf("ptr:%s\n",op[0]);
	for(i=1;i<argc;i++){
		op[i]=argv[i];
	}
	
	//op[argc-1] = "cat ";
	/*strcpy(para,op[0]);
	//para = op[0];
	strcat(para,file);
	printf("%s",para);*/
	
	/*for(i=0; i<argc; i++){
		strcpy(para,op[i]);
		strcat(para,file);
		if(i == argc - 1){
			strcat(para, " >");
			strcat(para,file2);
		}
		printf("%s",para);
		system(para);
	}*/
	
	pipe(fd);
	pid = fork();
		if(pid == 0){
		  //child
		  //dup2(fd[1],1);
		  close(fd[0]);
		  write(fd[1],file,9);
		  exit(0);
		}
	close(fd[1]);
	read(fd[0],buf,sizeof(buf));
	strcat(buf,"txt");

	for(i=0; i<argc; i++){
		//printf("%s ",op[i]);
		if((strcmp(op[i]," uniq ")!=0)&&i!=argc-1&&strcmp(op[i]," sort ")!=0){
			//printf("%s ",op[i]);
			strcpy(para,op[i]);
			strcat(para,buf);
			strcat(para, " >");
			strcat(para,file2);
		}else if(strcmp(op[i]," sort ")==0&&strcmp(op[i]," uniq ")!=0){
			strcpy(para,op[i]);
			strcat(para,buf);
			strcat(para, " >");
			strcat(para,"buf.txt");
		}else if(strcmp(op[i]," uniq ")==0){
			strcpy(para,op[i]);
			strcat(para,"buf.txt");
			strcat(para, " >");
			strcat(para,file2);
		}else{
			strcpy(para,op[i]);
			strcat(para,file2);
		}
			//printf("%s",para);
			//printf("\n");
			system(para);
	}

	//system("cat input.txt"); 
	
		/*pipe(fd);
		pid = fork();
		if(pid == 0){
		  //child
		  //dup2(fd[1],1);
		  close(fd[0]);
		  write(fd[1],file,sizeof(file));
		  exit(0);
		}*/
		
		/*for(i = 1; i < argc - 1; i++)
		{
			dup2(fd[0], 0);
			close(fd[1]);
			close(fd[0]);
			
		}*/
		
		 //execlp("uniq", "uniq", NULL);



	/*pipe(fd);
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
	}*/
/*#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<stdlib.h>

#define LEN 256
#define WIDTH 256
#define HEIGHT 10

void split(char source[],char dest[HEIGHT][WIDTH])
{
    char *p;
    p=strsep(&source,"|");
    int i=0;
    for(i=0;p[i]!='\0';i++)
    {
        dest[0][i]=p[i];
    }
    dest[0][i]='\0';
    int j=1;
    while(p)
    {
        p=strsep(&source,"|");
        if(p)
        {
            for(i=0;p[i]!='\0';i++)
            {
                dest[j][i]=p[i];
            }
            dest[j][i]='\0';
            j++;
        }
    }
}

	
	main(){
		char commond[LEN];
		char splitArray[HEIGHT][WIDTH]={{'\0'}};
		printf("%s",">>");
		gets(commond);
		split(commond,splitArray);
		
		int i = 0;
		for(i = 0;splitArray[i][0]!='\0';i++){
			puts(splitArray[i]);
		}
		int p[2];
		pipe(p);
		int j = 0;
		for(j=0;splitArray[j+1][0]!='\0';j++){
			if(fork()==0){
				close(0);
				close(p[0]);
				close(p[1]);
				dup(p[0]);
				printf(splitArray[j]);
				system(splitArray[j]);
			}else{close(1);
			close(p[0]);
			close(p[1]);
			dup(p[1]);
			printf(splitArray[j + 1]);
			system(splitArray[j+1]);
			}
		}*/
	
/*#include <stdio.h>
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
	for(i=0; i<argc; i++){
		printf("%s ", argv[i]);
	}
	
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
	}*/
	return 0;
}









