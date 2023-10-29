#include <err.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
	int state = 0;
	char buff;
	char prev = '\n';
	int line = 1;
	char lines[2];
	ssize_t read_size = 0;
	if(argc > 1 && strcmp(argv[1], "-n") == 0){
		state = 1;
	}
	if(argc == 1 || (argc == 2 && strcmp(argv[1], "-n") == 0)){
		if(state){
			lines[0] = '0' + line;
			lines[1] = ' ';
			if(write(1, lines , sizeof(lines)) == -1){
				err(2, "write err");
			}
			line ++;
		}

		while((read_size = read(0, &buff, sizeof(buff))) > 0){	
			if(state && prev == '\n'){
				lines[0] = '0' + line;
				lines[1] = ' ';
				if(write(1, lines , sizeof(lines)) == -1){
					err(2, "write err");
				}
			line ++;
			}
			if(write(1, &buff, sizeof(buff)) == -1){
				err(1, "err write");
			}
			prev = buff;
		}
		if(read_size == -1){
			err(3, "read err");
		}
		exit(0);
	}

	for(int i = 1 + state; i < argc; i ++){
		printf("i=%d\n", i);
		int fd = open(argv[i], O_RDONLY);
		if(fd == -1){
			err(5, "open err");
		}

		while((read_size = read(fd, &buff, sizeof(buff))) > 0){	
			if(state && prev == '\n'){
				lines[0] = '0' + line;
				lines[1] = ' ';
				if(write(1, lines , sizeof(lines)) == -1){
					err(2, "write err");
				}
			line ++;
			}
			if(write(1, &buff, sizeof(buff)) == -1){
				err(1, "err write");
			}
			prev = buff;
		}
		if(read_size == -1){
			err(3, "read err");
		}
		close(fd);
	}
	
	exit(0);
}
