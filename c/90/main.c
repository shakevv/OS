#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <err.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		errx(1, "err");
	}
	char* ptr;
	long n = strtol(argv[1], &ptr, 10);
	long d = strtol(argv[2], &ptr, 10);

	char m = 'm';
	char buff;
	char ding[5] = "DING\n";
	char dong[5] = "DONG\n";

	int a[2];
	int b[2];
	
	pipe(a);
	pipe(b);

	int pid = fork();

	if(pid == 0){
		close(a[0]);
		close(b[1]);
		while(read(b[0], &buff, 1) > 0){
			write(1, &dong, sizeof(dong));
			write(a[1], &m, 1);
		}
		close(a[1]);
		close(b[0]);
		exit(0);
	}

	close(a[1]);
	close(b[0]);

	write(1, &ding, sizeof(ding));
	write(b[1], &m, 1);
	
	while(read(a[0], &buff, 1) > 0){
		n--;
		if(n == 0){
			close(b[1]);
			close(a[0]);
			break;
		}
		sleep(d);
		write(1, &ding, sizeof(ding));
		write(b[1], &m, 1);
	}

	exit(0);
}
