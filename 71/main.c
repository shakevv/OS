#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		errx(1, "err");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		err(2, "err");
	}
	int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR);
	if(fd2 == -1){
		err(2, "err");
	}

	uint16_t ones[8];
	ones[0] = 2;

	for(int i = 1; i < 8; ++ i){
		ones[i] = ones[i-1] << 2;
	}

	ssize_t readsize;
	uint16_t buff;
	uint8_t res;
	while((readsize = read(fd1, &buff, sizeof(buff))) == sizeof(buff)){
		res = 0;
		buff = (buff << 8) | (buff >> 8);
		for(int i = 0; i < 8; ++ i){
			if((buff & ones[i]) != 0){
				res = (res | (1 << i));
			}
		}
		if(write(fd2, &res, sizeof(res)) == -1){
			err(5, "err");
		}
	}
	if(readsize == -1){
		err(6, "err");
	}
	if(readsize > 0){
		errx(7, "err");
	}
	close(fd1);
	close(fd2);

	exit(0);
}














