#include<stdio.h>
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
int main()
{
	char command[LEN];
	char splitArray[HEIGHT][WIDTH]={{'\0'}};
	printf("%s",">>");
	gets(command);
	split(command,splitArray);
	int i=0;
	for(i=0;splitArray[i][0]!='\0';i++)
	{
		puts(splitArray[i]);
	}
	int p[2];
	pipe(p);
	int j=0;
	for(j=0;splitArray[j+1][0]!='\0';j++)
	{
		if(fork()==0)
		{
			close(0);
			close(p[0]);
			close(p[1]);
			dup(p[0]);
			system(splitArray[j]);
		}else
		{
			close(1);
			close(p[0]);
			close(p[1]);
			dup(p[1]);
			system(splitArray[j+1]);
		}
	}
}
