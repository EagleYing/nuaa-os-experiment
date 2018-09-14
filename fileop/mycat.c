#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char *argv[]){//no space
	int fd, size;
	//char path[100],buf[100];
	char buf[100];
	//gets(path); dangerous,maybe overflow
	//printf("cat ");
        //fgets(path, 100, stdin);
	//scanf("%s", path);
	fd = open(argv[1], O_RDONLY);
        size = read(fd, buf, sizeof(buf));
        close(fd);
        //printf("%s", buf);
	write(1, buf, size);
        return 0;
}
	
