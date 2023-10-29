#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
	if(argc < 2) {
		err(1, "err");
	}
	
	int state = 0;
	if(strcmp(argv[1], "-n") == 0) {
		state = 1;
	}
	char buff[1];
	char prev = 'a';
	int fd[argc - state - 1];
	ssize_t readsize;
	struct stat s;
	int counte = 1;
	bool isFirst = true;
	char line[4];
	char space = ' '; 
	for(int i = 0; i < argc - state - 1; ++ i) {
		readsize = 0;
		if(strcmp(argv[i + 1 + state], "-") != 0){
			fd[i] = open(argv[i + 1 + state], O_RDONLY);
			if(fd[i] == -1) {
				err(2, "e taq greshka %s", argv[i + 1 + state]);
			}
		} else{
			fd[i] = 0;
		}
		while((readsize = read(fd[i], &buff[0], sizeof(uint8_t))) == 1) {
			if(isFirst == true && state == 1){
				char* line = convert(counte);
				size_t len = strlen(line);
				write(1, &line, len);
				write(1, &space, sizeof(space));
				isFirst = false;
			}
			if(state == 1 && prev == '\n'){
				char* line = convert(counte);
				size_t len = strlen(line);
				write(1, &line, len);
				write(1, &space, sizeof(space));
			}
			write(1,& buff[0], 1);
			if(buff[0] == '\n'){
				counte ++;
			}
			prev = buff[0];
		}
		if(readsize == -1){
			err(8, "err");
		}
	}

	for(int i = 0; i < argc - state - 1; ++ i) {
		if(fd[i] != 1){
			close(fd[i]);
		}
	}

	exit(0);
}






