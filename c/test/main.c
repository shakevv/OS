#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>

int main(void){
	
	int a[2];
	pipe(a);

	int p1 = fork();
	if(p1 == 0){
		close(a[0]);
		int fd = open("f", O_RDONLY);
		uint8_t res1 = 0;
		uint8_t buff;

		read(fd, &buff, 1);
		res1 = res1 + buff;
		read(fd, &buff, 1);
		res1 = res1 + buff;
		read(fd, &buff, 1);
		res1 = res1 + buff;

		write(a[1], &res1, sizeof(res1));
		exit(0);
	}
	int p2 = fork();
	if(p2 == 0){
		close(a[0]);
		int fd = open("f", O_RDONLY);
		uint8_t res1 = 0;
		uint8_t buff;

		read(fd, &buff, 1);
		res1 = res1 + buff;
		read(fd, &buff, 1);
		res1 = res1 + buff;
		read(fd, &buff, 1);
		res1 = res1 + buff;

		write(a[1], &res1, sizeof(res1));
		exit(0);
	}
	int p3 = fork();
	if(p3 == 0){
		close(a[0]);
		int fd = open("f", O_RDONLY);
		uint8_t res1 = 0;
		uint8_t buff;

		read(fd, &buff, 1);
		res1 = res1 + buff;
		read(fd, &buff, 1);
		res1 = res1 + buff;
		read(fd, &buff, 1);
		res1 = res1 + buff;

		write(a[1], &res1, sizeof(res1));
		exit(0);
	}

	close(a[1]);
	uint8_t k;
	uint8_t result = 0;
	while(read(a[0], &k, 1) > 0){
		printf("\n\n%d\n\n", k);
		result += k;
	}
	printf("\n\n\nfinally: %d\n\n\n", result);
	write(1, &result, 1);
	exit(0);
}
