#include <stdlib.h>
#include <fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>



int main(int argc, char* argv[]){
	if(argc != 3){
		errx(1, "err");
	}
	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		err(2, "err");
	}
	int fd2 = open(argv[2], O_RDONLY);
	if(fd2 == -1){
		err(2, "err");
	}
	int fd3 = open("output", O_CREAT | O_WRONLY | O_TRUNC,  S_IWUSR | S_IRUSR);
	if(fd3 == -1){
		err(2, "err");
	}

	struct stat s1;
	struct stat s2;
	if(fstat(fd1, &s1) == -1){
		err(3, "err");
	}
	if(fstat(fd2, &s2) == -1){
		err(3, "err");
	}
	if((s1.st_size * 2 * 8) != s2.st_size){
		errx(4, "err");
	}

	uint8_t byte;
	uint16_t buff;
	ssize_t readsize;
	uint8_t bits[8];
	bits[0] = 1;
	for(int i = 1; i < 8; ++ i){
		bits[i] = bits[i-1] << 1;
	}
	size_t pos = 0;
	while((readsize = read(fd1, &byte, sizeof(byte))) > 0){
		for(int i = 0; i < 8; ++ i){
			if((bits[i] & byte) == 0){
				++ pos;
				continue;
			} else{
				if(lseek(fd2, pos, SEEK_SET) == -1){
					err(4, "err");
				}
				if(read(fd2, &buff, sizeof(buff)) == -1){
					err(5, "err");
				}
				if(write(fd3, &buff, sizeof(buff)) == -1){
					err(6, "err");
				}
				pos ++ ;
			}
		}
	}
	if(readsize == -1){
		err(7, "err");
	}

	close(fd1);
	close(fd2);
	close(fd3);

	exit(0);
}

