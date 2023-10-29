#include <err.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if(argc != 4){
	//	errx(1, "invalid number of args...");
	}

	struct header {
		uint16_t magic;
		uint16_t filetype;
		uint32_t count;
	}__attribute__((packed));

	uint16_t buff;

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		err(2, "cannot open file %s", argv[1]);
	}

	int fd2 = open(argv[2], O_RDONLY);
	if(fd2 == -1){
		err(2, "cannot open file %s", argv[2]);
	}

	int fd3 = open(argv[3], O_RDONLY);
	if(fd3 == -1){
		err(2, "cannot open file %s", argv[3]);
	}

	struct header h;
	struct stat s;

	if(fstat(fd1, &s) == -1){
		err(3, "fstat err");
	}

	if(s.st_size < 8){
		errx(4, "invalid file");
	}

	if(fstat(fd2, &s) == -1){
		err(3, "fstat err");
	}

	if(s.st_size < 8){
		errx(4, "invalid file");
	}

	if(fstat(fd3, &s) == -1){
		err(3, "fstat err");
	}

	if(s.st_size < 8){
		errx(4, "invalid file");
	}

	ssize_t read_size = 0;


	if((read_size = read(fd1, &h, sizeof(h))) == -1){
		err(5, "read err");
	}
	if(h.magic != 0x4D5A){
		errx(4, "invalid file");
	}
	uint16_t type = 1;
	if(((h.filetype << 8) | (h.filetype >> 8)) != type){
		errx(4, "invalid file");
	}
	uint8_t arr[4];
	arr[0] = h.count
	uint32_t len1 = h.count << 8 | h.count >> 8 | h.count;

	int fd2 = creat(argv[2], S_IWUSR | S_IRUSR);
	write(fd2, &buff, sizeof(buff));

	printf("\nbuff is: %x\n", buff);
	close(fd1);
	close(fd2);
	exit(0);



}
