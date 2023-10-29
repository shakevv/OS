#include <err.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){

	if(argc != 5){
		errx(1, "Invalid number of args...");
	}

	struct str {
		uint16_t offset;
		uint8_t len;
		uint8_t unused;
	}__attribute__((packed));

	struct stat s;
	
	int fd1dat = open(argv[1], O_RDONLY);
	if(fd1dat == -1){
		err(2, "Cannot open file %s", argv[1]);
	}
	if(fstat(fd1dat, &s) == -1){
		err(3, "fstat err");
	}
	if(s.st_size % sizeof(struct str) != 0){
		errx(2, "Invalid file format: %s", argv[1]);
	}

	int fd2dat = open(argv[2], O_WRONLY);
	if(fd2dat == -1){
		err(2, "Cannot open file %s", argv[2]);
	}
	if(fstat(fd2dat, &s) == -1){
		err(3, "fstat err");
	}
	if(s.st_size % sizeof(struct str) != 0){
		errx(2, "Invalid file format: %s", argv[2]);
	}
	
	int fd1idx = open(argv[3], O_RDONLY);
	if(fd1idx == -1){
		err(2, "Cannot open file %s", argv[3]);
	}

	int fd2idx = open(argv[4], O_WRONLY);
	if(fd2idx == -1){
		err(2, "Cannot open file %s", argv[4]);
	}
	
	ssize_t read_size;
	struct str info;
	char b;

	while((read_size = read(fd1idx, &info, sizeof(info))) > 0){
		if(lseek(fd1dat, info.offset, SEEK_SET) == -1){
			err(4, "lseek err");
		}
		
		if(read(fd1dat, &b, sizeof(b)) == -1){
			err(5, "read err");
		}

		if(b < 'A' || b > 'Z'){
			continue;
		}

		if(write(fd2dat, &b, sizeof(b)) == -1){
			err(6, "write err");
		}

		for(size_t i = 0; i < info.len; ++ i){
			if(read(fd1dat, &b, sizeof(b)) == -1){
				err(5, "read err");
			}
			if(write(fd2dat, &b, sizeof(b)) == -1){
				err(6, "write err");
			}
		}

		if(write(fd2idx, &info, sizeof(info)) == -1){
			err(6, "write err");
		}
	}

	if(read_size == -1){
		err(5, "read err");
	}

	close(fd1dat);
	close(fd1idx);
	close(fd2dat);
	close(fd2idx);

	exit(0);
}
