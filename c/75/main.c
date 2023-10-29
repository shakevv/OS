#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		errx(1, "invalid number of args...");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		err(2, "cannot open file %s", argv[1]);
	}
	
	int fd2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd2 == -1){
		err(2, "cannot open file %s", argv[2]);
	}

	struct stat s;
	if(fstat(fd1, &s) == -1){
		err(3, "fstat err");
	}

	if(s.st_size % 2 != 0){
		errx(4, "invaild file");
	}

	uint16_t buff;
	uint16_t zeros[8];
	zeros[0] = 1;
	for(int i = 1; i < 8; ++ i){
		zeros[i] = zeros[i-1] << 2;
	}
	uint16_t ones[8];
	ones[0] = 2;
	for(int i = 1; i < 8; ++ i){
		ones[i] = ones[i-1] << 2;
	}
	
	ssize_t read_size = 0;
	uint8_t res = 0;

	while((read_size = read(fd1, &buff, sizeof(buff))) == sizeof(buff)){

		uint16_t arr[2];

		arr[0] = buff << 8;
		arr[1] = buff >> 8;
		buff = arr[0] | arr[1];

		for(int i = 0; i < 8; ++ i){
			if((buff & ones[i]) != 0){
				res = res | (1  << i);
			}
		}

		if(write(fd2, &res, sizeof(res)) == -1){
			err(5, "write err");
		}
		res = 0;
	}

	if(read_size == -1){
		err(6, "read err");
	}

	close(fd1);
	close(fd2);

	exit(0);
}
