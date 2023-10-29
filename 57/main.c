#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
	if(argc != 5){
		errx(1, "err");
	}

	struct info_t {
		uint16_t offset;
		uint8_t len;
		uint8_t reserved;
	}__attribute__((packed));

	int fdat1 = open(argv[1], O_RDONLY);
	int fidx1 = open(argv[2], O_RDONLY);
	
	struct stat s;
	if(fstat(fidx1, &s) == -1){
		err(2, "err");
	}
	if(s.st_size % sizeof(struct info_t) != 0){
		errx(3, "err");
	}

	int fdat2 = creat(argv[3], S_IWUSR | S_IRUSR);
	if(fdat2 == -1){
		err(4, "err");
	}

	int fidx2 = creat(argv[4], S_IWUSR | S_IRUSR);
	if(fidx2 == -1){
		err(5, "err");
	}

	struct info_t buff;
	ssize_t readsize;
	char b;

	while((readsize = read(fidx1, &buff, sizeof(buff))) > 0){
		if(lseek(fdat1, buff.offset, SEEK_SET) == -1){
			err(6, "err");
		}

		if(read(fdat1, &b, sizeof(b)) == -1){
			err(7, "err");
		}
		if(b < 'A' || b > 'Z'){
			continue;
		}
		if(write(fdat2, &b, sizeof(b)) == -1){
			err(8, "err");
		}

		for(uint8_t i = 0; i < buff.len - 1; ++ i){
			if(read(fdat1, &b, sizeof(b)) == -1){
				err(7, "err");
			}
			if(write(fdat2, &b, sizeof(b)) == -1){
				err(8, "err");
			}
		}
		
		if(write(fidx2, &buff, sizeof(buff)) == -1){
			err(9, "err");
		}
	}

	if(readsize == -1){
		err(10, "err");
	}

	close(fdat1);
	close(fdat2);
	close(fidx1);
	close(fidx2);

	exit(0);
}
