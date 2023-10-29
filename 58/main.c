#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){
	if(argc != 4){
		errx(1, "err");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		err(2, "err");
	}

	int fd2 = open(argv[2], O_RDONLY);
	if(fd2 == -1){
		err(3, "err");
	}

	int fd3 = creat(argv[3], S_IWUSR | S_IRUSR);
	if(fd3 == -1){
		err(4, "err");
	}

	struct stat s1;
	struct stat s2;

	if(fstat(fd1, &s1) == -1){
		err(5, "err");
	}

	if(fstat(fd2, &s2) == -1){
		err(6, "err");
	}

	if(s1.st_size != s2.st_size){
		errx(7, "err");
	}

	struct str_t {
		uint16_t offset;
		uint8_t original;
		uint8_t new;
	}__attribute__((packed));

	struct str_t buff;
	ssize_t readsize;
	ssize_t readsize2 = -1;
	buff.offset = 0;
	
	while(((readsize = read(fd1, &buff.original, sizeof(uint8_t))) > 0) && ((readsize2 = read(fd2, &buff.new, sizeof(uint8_t))))){
		if(buff.original == buff.new){
			buff.offset ++;
			continue;
		}
		if(write(fd3, &buff, sizeof(buff)) == -1){
			err(9, "err");
		}
		buff.offset ++;
	}

	if(readsize == -1){
		err(10, "err");
	}
	if(readsize2 == -1){
		err(10, "err");
	}

	close(fd1);
	close(fd2);
	close(fd3);

	exit(0);
}


	
