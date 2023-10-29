
//cat /etc/passwd | cut -d '/' -f6 | sort


#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
	int a[2];
	if(pipe(a) == -1){
		err(1, "pipe err");
	}

	pid_t p1 = fork();
	if(p1 == -1){
		err(2, "fork err");
	}
	if(p1 == 0){
		close(a[0]);
		if(dup2(a[1], 1) == -1){
			err(3, "dup2 err");
		}
		if(execlp("cat", "cat", "/etc/passwd", 0) == -1){
			err(4, "execlp err");
		}
	}
	close(a[1]);

	int b[2];
	if(pipe(b) == -1){
		err(1, "pipe err");
	}
	pid_t p2 = fork();
	if(p2 == -1){
		err(2, "fork err");
	}
	if(p2 == 0){
		close(b[0]);
		if(dup2(a[0], 0) == -1){
			err(3, "dup2 err");
		}
		if(dup2(b[1], 1) == -1){
			err(3, "dup2 err");
		}
		if(execlp("cut", "cut", "-d", "/", "-f6", 0) == -1){
			err(4, "execlp err");
		}
	}
	close(b[1]);
	if(dup2(b[0], 0) == -1){
		err(3, "dup2 err");
	}
	if(execlp("sort", "sort", 0) == -1){
		err(4, "execlp err");
	}
}
