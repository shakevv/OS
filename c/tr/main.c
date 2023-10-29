#include <fcntl.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		errx(1, "invalid number of args...");
	}

	int	a[2];
	if(pipe(a) == -1){
		err(2, "pipe err");
	}

	pid_t pid1 = fork();
	if(pid1 == -1){
		err(3, "fork err");
	}
	if(pid1 == 0){
		close(a[0]);
		if(dup2(a[1], 1) == -1){
			err(7, "dup2 err");
		}
		if(execlp("cat", "cat", argv[1], 0) == -1){
			err(8, "execlp err");
		}
	}
	close(a[1]);
	
	int b[2];
	if(pipe(b) == -1){
		err(3, "pipe err");
	}
	
	pid_t pid2 = fork();
	if(pid2 == -1){
		err(10, "fork err");
	}

	if(pid2 == 0){
		if(dup2(a[0], 0) == -1){
			err(11, "duo2 err");
		}
		close(b[0]);
		if(dup2(b[1], 1) == -1){
			err(12, "dup2 err");
		}
		if(execlp("sort", "sort", 0) == -1){
			err(14, "execlp err");
		}
	}

	close(b[1]);
	if(dup2(b[0], 0) == -1){
		err(16, "dup2 err");
	}

	if(execlp("uniq", "uniq", "-c", 0) == -1){
		err(17, "execlp err");
	}
}
