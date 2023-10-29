#include <stdint.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
 
#define MAX_MEMORY (1024*1024*256)
#define MAX_ELEMENTS (MAX_MEMORY / sizeof(uint32_t))

void bubble(uint32_t* arr, size_t size){
	for(size_t i = 0; i < size - 1; ++ i){
		for(size_t j = 0; j < size - i - 1; ++ j){
			if(arr[j] > arr[j + 1]){
				uint32_t tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}

int main(int argc, char* argv[]){
	if(argc != 2){
		errx(1, "err");
	}

	int fd = open(argv[1], O_RDWR);
	if(fd == -1){
		err(2, "%s", argv[1]);
	}

	uint32_t* arr = malloc(MAX_MEMORY);
	if(arr == NULL){
		err(3, "err");
	}
	
	ssize_t readsize;
	if((readsize = read(fd, arr, MAX_MEMORY)) == -1){
		err(4, "err");
	}
	
	bubble(arr, readsize / sizeof(uint32_t));

	if(readsize == MAX_MEMORY){
		int tmpfd = creat("tmp", I_SWUSR | I_SRUSR); 
		if(tmpfd == -1){
			err(5, "err");
		}
		if(write(tmpfd, arr, readsize) == -1){
			err(6, "err");
		}
	}




	exit(0);
}
	
