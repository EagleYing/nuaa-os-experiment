#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
	int fd, size;
        //char source_path[100];
	//char target_path[100];
	char buf[100];
        //scanf("%s %s", source_path, target_path);
	//printf(source_path, target_path);
	fd = open(argv[1], O_RDONLY);
	size = read(fd, buf, sizeof(buf));
	//printf("%s", buf);
	close(fd);
	fd = open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
	write(fd, buf, size);
        close(fd);
	return 0;
}
	
