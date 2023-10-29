#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <err.h>
#include <err.h>
//find dir -type f -printf"%T@ %p" | sort -n  -k 1,1 | tail -n 1 | cut -d ' ' -f 2

int main(int argc, char* argv[]){
	if(argc != 2){
		errx(1, "err");
	}

int a[2];
if(pipe(a) == -1){
	err(2, "err pipe");
}

pid_t pid1 = fork();
if(pid1 == -1){
	err(3, "err fork");
}
if(pid1 == 0){
	dup2(a[1], 1);
	close(a[0]);
	execlp("find", "find", argv[1],  "-type", "f", "-printf", "%T@ %p\\n", 0);
}
close(a[1]);
int b[2];
pipe(b);
if(fork() == 0){
	dup2(a[0], 0);
	dup2(b[1], 1);
	close(b[0]);
	execlp("sort", "sort", "-n", "-k", "1,1", 0);
}
close(b[1]);
int c[2];
pipe(c);
if(fork() == 0){
	dup2(b[0], 0);
	dup2(c[1], 1);
	close(c[0]);
	execlp("tail", "tail", "-n", "1", 0);
}
close(c[1]);
dup2(c[0], 0);
execlp("cut", "cut", "-d", " ", "-f", "2");
}
