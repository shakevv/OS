#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	//if(argc != 2){
	//	errx(1, "err");
	//}
	
	//int p[2];
	if(execl("/bin/date", "date",(char*) NULL) == -1){
		err(1, "err");
	} else {
		printf("hello");
	}
	exit(0);
}
