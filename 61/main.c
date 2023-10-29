#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]){
	int fd[argc - 1];
	for(int i = 1; i < argc; ++ i){
		if(strcmp(argv[i], "-") == 0){
			fd[i-1] = 0;
		} else { 
			fd[i-1] = open(argv[i], O_RDONLY);
		}
	}
	ssize_t readsize;
	uint8_t buff;

	for(int i = 0; i < argc - 1; ++ i){
		while((readsize = read(fd[i], &buff, sizeof(buff))) == sizeof(buff)){
			if(write(1, &buff, sizeof(buff)) == -1){
				err(1, "err");
			}
		}
		if(readsize == -1){
			err(2, "err");
		}
	}
	for(int i = 1; i < argc; ++ i){
		if(strcmp(argv[i], "-") == 0){
			continue;
		} else { 
			close(fd[i-1]);
		}
	}
	exit(0);
}
