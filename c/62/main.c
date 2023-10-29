#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]){

	if(argc != 4){
		errx(1, "Invalid number of args...");
	}

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		err(2, "Cannot open file %s", argv[1]);
	}

	int fd2 = open(argv[2], O_RDONLY);
	if(fd2 == -1){
		err(2, "Cannot open file %s", argv[2]);
	}

	int fd3 = open(argv[3], O_CREAT | O_WRONLY | O_TRUNC, S_IWUSR | S_IRUSR);
	if(fd3 == -1){
		err(2, "Cannot create file %s", argv[3]);
	}

	struct stat s1;
	struct stat s2;

	if(fstat(fd1, &s1) == -1){
		err(3, "stat err");
	}

	if(fstat(fd2, &s2) == -1){
		err(3, "stat err");
	}

	if(s1.st_size != s2.st_size){
		errx(4, "invalid files...");
	}

	struct diff_t {
		uint16_t offset;
		uint8_t original;
		uint8_t new;
	}__attribute__((packed));

	struct diff_t diff;
	uint8_t buff1;
	uint8_t buff2;
	ssize_t read_size1 = 0;
	ssize_t read_size2 = 0;
	uint16_t offset = 0;

	while(((read_size1 = read(fd1, &buff1, sizeof(buff1))) > 0) && (read_size2 = read(fd2, &buff2, sizeof(buff2))) > 0){
		if(buff1 == buff2){
		offset ++;
			continue;
		}

		diff.offset = offset;
		diff.original = buff1;
		diff.new = buff2;

		if(write(fd3, &diff, sizeof(diff)) == -1){
			err(5, "write err");
		}

		offset ++;
		printf("offset: %d\n old: %d\n new: %d\n\n", diff.offset, diff.original, diff.new);
	}

	if(read_size1 == -1 || read_size2 == -1){
		err(6, "read err");
	}

	close(fd1);
	close(fd2);
	close(fd3);

	exit(0);
}
