#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>


void bubble(uint8_t* arr, size_t size);

int main(int argc, char* argv[]){

	if(argc != 2){
		errx(1, "Invalid number of args...");
	}

	int fd = open(argv[0], O_RDWR);
	if(fd == -1) {
		err(2, "Cannot open file %s", argv[0]);
	}

	struct stat s;
	if(fstat(fd, &s) == -1){
		err(3, "stat err");
	}

	uint8_t* arr = malloc(s.st_size);
	if(!arr){
		close(fd);
		err(4, "malloc err");
	}

	ssize_t read_size = read(fd, arr, s.st_size);
	if(read_size != s.st_size){
		close(fd);
		free(arr);
		err(5, "err read");
	}

	bubble(arr, s.st_size);

	if(lseek(fd, 0, SEEK_SET) == -1){
		close(fd);
		free(arr);
		err(7, "lseek err");
	}

	ssize_t write_size = write(fd, arr, s.st_size);
	if(write_size != s.st_size){
		close(fd);
		free(arr);
		err(8, "write err");
	}

	close(fd);
	free(arr);

	exit(0);
}



void bubble(uint8_t* arr, size_t size){
	for(size_t i = 0; i < size - 1; ++ i){
		for(size_t j = 0; j < size - i - 1; ++ j){
			if(arr[j] < arr[j+1]){
				uint8_t tmp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}
}


