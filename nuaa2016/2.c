#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
    int p1,p2;
    int fd1[2],fd2[2];
    
    int r1,r2,res1,res2;
    pipe(fd1);
    p1=fork();
    if(p1==0){
      close(fd1[0]);
      r1 = ((1+50)*50)/2;
      printf("%d\n",r1);
      write(fd1[1],&r1,sizeof(int));
      exit(0);
    }else{
      wait(p1);
    }

    pipe(fd2);
    p2=fork();
    if(p2==0){
      close(fd2[0]);
      r2 = ((51+100)*50)/2;
      printf("%d\n",r2);
      write(fd2[1],&r2,sizeof(int));
      exit(0);
    }else{
      wait(p2);
    }

    close(fd1[1]);
    read(fd1[0],&res1,sizeof(int));
    read(fd2[0],&res2,sizeof(int));
    printf("%d\n",res1+res2);
    return 0;
}
