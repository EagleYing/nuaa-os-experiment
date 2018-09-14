#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define SIZE 20
void cat(char *s){
    
    
    int pid,j=0,i=0;
    int fd[2],ffd;
    char commandlist[20][100],*p,command1[2][50],arg1[10][50],arg2[10][50],k;
    char buf[32],*argv[20]={"sort","input.txt",0};
    //printf("%s\n",s);
    p=strtok(s,"|");
   
    while(p){
		//printf("%s---...\n",p);
        i=1;
       
        
        strcpy(commandlist[j],p);
        if(commandlist[j][0]==' '){
        while (commandlist[j][i]!='\0') {
            commandlist[j][i-1]=commandlist[j][i];
            i++;
        }
            
            
        commandlist[j][i-1]='\0';
        if(commandlist[j][i-2]==' ')
            commandlist[j][i-2]='\0';

        }
         //printf("%s\n",commandlist[j]);
        
        p=strtok(NULL,"|");
        j++;
        
    }
    //printf("j : %d\n",j);
    if(j>3){
	//printf("arg %s %s %s\n",arg2[0],arg2[1],arg2[2]);
   
    p=strtok(commandlist[0]," <");
    //if(commandlist[0][2]=='<')
    p=strtok(NULL," <");
    
    strcpy(command1[0],p);
    
    //printf("%s ---\n",p);
    p=strtok(commandlist[j-1]," >");
    p=strtok(NULL," >");
    
    strcpy(command1[1],p);
    
    //printf("%s -asd--- command :%s %s %s %s \n",p,command1[0],commandlist[1],commandlist[2],command1[1]);
    //printf("command :%s %s \n",command1[0],commandlist[1]);

    pipe(fd);
    pid = fork();
	//printf("pid : %d\n",pid);
    ffd=open(command1[1], O_CREAT|O_RDWR, 0666);
    
   // dup2(ffd, fd[0]);
    if (pid == 0) {
		//printf("pidd %d\n",pid);
        //printf("child :%s %s \n",commandlist[1],command1[0]);
        // child
        sleep(1); // let parent execute first
        close(1);
		
        dup2(fd[1], 1);
       
        close(fd[0]);//关闭读端
      //  close(fd[1]);
     	//printf("child :%s %s 456456\n",commandlist[1],command1[0]);
        execlp(commandlist[1], commandlist[1],"-u",command1[0], NULL);
		
        _exit(0);
    }
    // parent will block
  //  dup2(ffd, fd[0]);
    dup2(fd[0],0);
    dup2(ffd,1);
    //close(fd[0]);
  //  close(1);
    close(fd[1]);//关闭写端
    
    close(ffd);
    //  printf("unip");
    //   execlp("sort", "sort", NULL);
    read(0, buf, sizeof(buf));
	//printf("buf: %s--",buf);
    execlp("cat", "cat", NULL);  
    }

}

int main()
{
    char a[]="cat <input.txt | sort | uniq | cat >output.txt",c[10],d[100],pid;
	char *l;

    int oldfd,i,j,flag=0;

	printf("%s\n",a);
      pid=fork();
        if(pid==0){           
            cat(a);
        }
    return 0;
}

