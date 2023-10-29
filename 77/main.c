#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <err.h>
//cut -d 7 /etc/passwd | sort | uniq -c
int main(int argc, char* argv[]){
	int a[2];
	if(pipe(a) == -1){
		err(1, "err pipe");
	}

	pid_t pid = fork();
	if(pid == -1){
		err(2, "err fork");
	}
	if(pid == 0){
		if(dup2(a[1], 1) == -1){
			err(3, "err dup2");
		}
		if(close(a[0]) == -1){
			err(4, "err close");
		}
		if(execlp("cut", "cut", "-d", ":", "-f", "7", "/etc/passwd", (char*) NULL) == -1){
			err(5, "err exec");
		}
	}

	if(close(a[1]) == -1){
		err(4, "err close");
	}

	int b[2];
	if(pipe(b) == -1){
		err(1, "err pipe");
	}

	pid_t pid2 = fork();
	if(pid2 == -1){
		err(2, "err fork");
	}
	if(pid2 == 0){
		if(dup2(a[0], 0) == -1){
			err(3, "err dup2");
		}
		if(close(b[0]) == -1){
			err(4, "err close");
		}
		if(dup2(b[1], 1) == -1){
			err(3, "err dup2");
		}
		if(execlp("sort", "sort", 0) == -1){
			err(5, "err exec");
		}
	}

	if(close(b[1]) == -1){
		err(4, "err close");
	}

	if(dup2(b[0], 0) == -1){
		err(3, "err dup2");
	}

	if(execlp("uniq", "uniq", "-c", (char*) NULL) == -1){
		err(5, "err exec");
	}
}



