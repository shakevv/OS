#include <fcntl.h>
#include <err.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main(int argc, char* argv[]){
	if(argc == 2 && argv[0]){
	}
		pid_t pid = fork();
		if(pid == -1){
			err(2, "fork err");
		}
		if(pid == 0){
			if(execlp(argv[1], argv[1], 0) == -1){
				err(3, "command cant be run");
			}
		}
wait(NULL);
	exit(0);
}
