#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void bubble(uint8_t* arr, size_t size) {
	for (int i = 0; i < size - 1; ++ i) {
		for (int j = 0; j < size - i - 1; ++ j) {
			if( arr[j] > arr[j + 1]) {
				uint8_t tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		errx(1, "err");
	}

	int fd = open(argv[1], O_RDWR);
	if (fd == -1) {
		err(2, "%");
	}

	struct stat s;

	if (fstat(fd, &s) == -1) {
		err(3, "stat err");
	}

	uint8_t* arr = malloc(s.st_size);
	if(arr == NULL) {
		close(fd);
		err(4, "err");
	}

	ssize_t read_data = read(fd, arr, s.st_size);
	if(read_data != s.st_size) {
		close(fd);
		free(arr);
		err(5, "err");
	}
	
	bubble(arr, s.st_size);

	if (lseek(fd, 0, SEEK_SET) == -1) {
		close(fd);
		free(arr);
		err(7, "err");
	}

	ssize_t write_size = write(fd, arr, s.st_size);
	if(write_size != s.st_size) {
		close(fd);
		free(arr);
		err(6, "err");
	}

	close(fd);
	free(arr);

	exit(0);
}

