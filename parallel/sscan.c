#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <dirent.h>
#include <pthread.h>  
#include <assert.h>  
#include <sys/stat.h>
#include <string.h>

int count;

int isFolder(char* path)
{
	struct stat s_buf;
	stat(path,&s_buf);
	if(S_ISDIR(s_buf.st_mode))
		return 1;
	return 0;
}

void * readcount(char* file)
{
	FILE *f;
	if((f=fopen(file,"r"))==NULL){
		printf("Can not open file %s\n",file);
	}
    char buf[1024];
    int i=0;
    
    while (fgets(buf,1024, f)){
    	if(strstr(buf,"define")){
    		i++;
		} 
    }
	fclose(f);
	count+=i;
}

char* connPath(char*path,char*file)
{
	char * p = (char*)malloc(strlen(path)+strlen(file)+3);
	strcpy(p,path);
	strcat(p,"/");
	strcat(p,file);
	return p;
}

void* calculate(char* path)
{
	int error;
    DIR *dir;
    struct dirent entry;
    struct dirent *result;
    struct stat buf;
    dir = opendir(path);

    while(1){
        result = readdir(dir);
		if (result == NULL){
        	break;
		}     
        int flag=0;
        char *p = connPath(path,result->d_name);
		int len;
		if(isFolder(p)&&(strcmp(result->d_name, ".") != 0)&& (strcmp(result->d_name, "..") != 0)){
			calculate(p);
		}
		else{
			int len = strlen(result->d_name);
			if(result->d_name[len-1]=='c'&&result->d_name[len-2]=='.'){
				flag=1;	
			}
    		if(flag==1){
				readcount(connPath(path,result->d_name));
		 	} 
		}
    }
    closedir(dir);
}
int main (int argc, char **argv)  
{  
	count=0;
    char a[]="/mnt/shared/os/parallel";
	printf("%s define : ",a);
    calculate(a);
    printf("%d\n",count);
    return 0;  
}  