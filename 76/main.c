#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <stdio.h>


int main(int argc, char* argv[1]){
	const char* m = "\nEnter command: ";
	ssize_t readsize = 0;
	char buff[32];
	while(true){
		write(1, m, strlen(m));
	if((readsize = read(0, buff, 31)) == -1){
		err(3, "err read");
	}
	buff[readsize - 1] = '\0';
	if(strcmp(buff, "exit") == 0){
		break;
	}
	if(fork() == 0){
		printf("len: %d", strlen(buff));
		write(1, buff, strlen(buff));
		if(execlp(buff, buff,0) == -1){
			err(4, "err exec");
		}
	}
	wait(NULL);
	}
	exit(0);
}
