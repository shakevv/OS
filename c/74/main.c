#include <err.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		errx(1, "Invalid number of args...");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		err(2, "open err");
	}

	int fd2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd2 == -1){
		err(2, "open err");
	}

	uint8_t buff;
	ssize_t read_size = 0;
	uint16_t res = 0;
	uint8_t arr[8];
	arr[0] = 1;
	for(int i = 1; i < 8; ++ i){
		arr[i] = arr[i-1] * 2;
	}
	uint16_t zeros[8];
	uint16_t ones[8];
	zeros[0] = 1;
	ones[0] = 2;
	for(int i = 1; i < 8; ++ i){
		zeros[i] = zeros[i -1] << 2;
		ones[i] = ones[i-1] << 2;
	}
	while((read_size = read(fd1, &buff, sizeof(buff))) == sizeof(buff)){
		for(int i = 0; i < 8; ++ i){
			if((arr[i] & buff) == 0){
				res = res | zeros[i];
			} else{
				res = res | ones[i];
			}
		}

		uint8_t b[2];
	
		b[0] = res >> (0 * 8);
		b[1] = res >> (1 * 8);   //  ===	res = (res << 8) | (res >> 8);
		
		printf("b1: %x", b[0]);
		printf("b2: %x", b[1]);
		if(write(fd2, &b[1], sizeof(uint8_t)) == -1){
			err(3, "write err");
		}
		if(write(fd2, &b[0], sizeof(uint8_t)) == -1){
			err(3, "write err");
		}
		res = 0;
	}

	if(read_size == -1){
		err(4, "read err");
	}

	close(fd1);
	close(fd2);

	exit(0);
}
