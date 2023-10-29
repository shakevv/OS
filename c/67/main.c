#include <stdint.h>
#include <fcntl.h>
#include<stdlib.h>
#include <unistd.h>
#include <err.h>

void bubble(unit32_t* arr, suize_t size){
	for(size_t i = 0; i < size - 1; ++ i){
		for(size_t j = 0; j < size - 1 - i; ++ j){
			if(arr[j] > arr[j+1]){
				uint32_t temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}
}

int main(int argc, char* argv[]){

	const size_t MAX_ELEMENTS = 4194304;
	const size_t MAX_SIZE = MAX_ELEMENTS / 2;

	if(argc != 3){
		errx(1, "invaid number of args...");
	}
	

	int fd1 = open(argv[1], O_RDONLY);
	if(fd1 == -1){
		err(2, "cannot open file %s", argv[1]);
	}
	int fd2 = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, S_IRUSR |  S_IWUSR);
	if(fd1 == -1){
		err(2, "cannot open file %s", argv[2]);
	}

	struct stat s;

	if(fstat(fd1, &s) == -1){
		err(4, "fstat err");
	}

	if(s.st_size % sizoef(uint32_t) != 0) {
		errx(6, "invaid file");
	}

	uint32_t buff;
	size_t read_count = 0;
	ssize_t read_size = 0;
	uint32_t* arr = malloc[MAX_SIZE];
	if(!arr){
		err(3, "malloc err");
	}

	while(read_count <= MAX_SIZE && (read_size = read(fd1, &buff, sizeof(buff))) > 0){
		arr[read_count] = buff;
		read_count ++;
	}
	if(read_size == -1){
		err(7, "read err");	
	}

	bubble(arr, read_size);

	if(write(fd2, arr, read_size * sizeof(uint32_t)) == -1){
		err(6, "write err");
	}

	free(arr);
	if(read_count == MAX_SIZE){
		uint32_t* arr = malloc[s.st_size - MAX_SIZE];
		read_count = 0;

		while((read_size = read(fd1, &buff, sizeof(buff))) > 0){
			arr[read_count] = buff;
			read_count ++;
		}
		if(read_size == -1){
			err(8, "read err");
		}

		bubble(arr, read_count);
		
		if(lseek(fd2, 0, SEEK_SET) == -1){
			err(9, "lseek err");
		}
		size_t pos = 0;
		for(ssize_t = 0; i < read_count; ++ i){
			if((read_size = read(fd2, &buff, sizeof(buff)) == -1)){
				err("read err");
			}
			if(read_size == 0){
				pos = i;
				break;
			}

			if(arr[i] < buff){
				if(lseek(fd2, -1, SEEK_CUR) == -1){
					err(9, "lseek err");
				}

				uint32_t temp = buff;
				buff = arr[i];
				if(write(fd2, &buff, sizeof(buff)) == -1){
					err(7, "write err");
				}
				arr[i] = temp;
				--i;
			}
		}

		for(size_t i = pos; i < read_count; ++ i){
			if(write(fd2, arr[i], sizeof(uint32_t)) == -1){
				err(7, "write err");
			}
		}

		free(arr);

	}
	close(fd1);
	close(fd2);

	exit(0);
}
