#include <err.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv []){
	if(argc != 3){
		errx(1, "err");
	}
	int fd1 = open(argv[1], O_RDONLY);
	int fd2 = open(argv[2], O_WRONLY);
	if(fd1 == -1 || fd2 == -1){
		err(2, "err");
	}
	uint8_t arr[8] = {1, 2, 4, 8, 16, 32, 64, 128};
	uint16_t zeros[8];
	uint16_t ones[8];
	zeros[0] = 1;
	ones[0] = 2;
	uint16_t res;
	for(int i = 1; i < 8; i ++){
		zeros[i] = zeros[i-1] << 2;
		ones[i] = ones[i-1] << 2;
	}
	uint8_t buff;
	ssize_t readsize;
	while((readsize = read(fd1, &buff, sizeof(buff))) == sizeof(uint8_t)){
		res = 0;
		for(int i = 0; i < 8; i++){
			if((buff & arr[i]) == 0){
				res = res | zeros[i];
			} else{
				res = res | ones[i];
			}
		}
		res = (res << 8) | (res >> 8);
		if(write(fd2, &res, sizeof(res)) == -1){
			err(7, "err");
		}
	}
	if(readsize == -1){
		err(8, "err");
	}

	close(fd1);
	close(fd2);

	exit(0);
}
