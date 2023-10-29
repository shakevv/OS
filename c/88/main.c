#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		errx(1, "invalid number of args...");
	}

	int fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		err(2, "open err");
	}

	struct s {
		uint64_t name;
		uint32_t offset;
		uint32_t length;
	}__attribute__((packed));

	struct s arr[8];

	int count = 0;
	ssize_t read_size = 0;
	uint16_t childres;
	uint16_t finalres = 0;
	int a[2];
	if(pipe(a) == -1){
		err(3, "pipe err");
	}

	while((read_size = read(fd, &arr[count], sizeof(struct s))) == sizeof(struct s)){

		pid_t pid = fork();
		if(pid == -1){
			err(4, "fork err");
		}

		if(pid == 0){
			close(a[0]);
			int fd1 = open((char*) arr[count].name, O_RDONLY);
			if(fd1 == -1){
				err(2, "open err");
			}

			uint16_t buff;
			uint16_t res = 0;
			ssize_t rdsz = 0;

			if(lseek(fd, arr[count].offset, SEEK_SET) == -1){
				err(7, "lseek err");
			}

			while((rdsz = read(fd1, &buff, sizeof(buff))) == sizeof(buff) && arr[count].length --){
				res = res ^ buff;
			}
			if(rdsz == -1){
				err(5, "read err");
			}
			if(write(a[1], &res, sizeof(res)) == -1){
				err(6, "write err");
			}
			close(a[1]);
			exit(0);
		}

		close(a[1]);
		count ++;
	}
	
	if(read_size == -1){
		err(5, "read err");
	}
		
	while((read_size = read(a[0], &childres, sizeof(childres))) == sizeof(childres)){
		finalres = finalres ^ childres;
	}

	if(read_size == -1){
		err(5, "read err");
	}


	if(write(1, &finalres, sizeof(finalres)) == -1){
		err(6, "write err");
	}

	exit(0);
}
