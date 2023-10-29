#include<err.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		errx(1, "err");
	}

	int fd1 = open(argv[1], O_RDONLY);
	int fd2 = open(argv[2], O_RDONLY);
	int fd3 = creat("f3", S_IRUSR | S_IWUSR);

	if(fd1 == -1 || fd2 == -1 || fd3 == -1){
		err(2, "cannot open file");
	}


	struct buff_t {
		uint32_t from;
		uint32_t len;
	}__attribute__((packed));

	struct buff_t buff;
	uint32_t numBuff;
	ssize_t readsize;
	ssize_t readsize2;
	while( (readsize = read(fd1, &buff, sizeof(buff))) == sizeof(buff)){
		if(readsize != sizeof(buff)){
			errx(3, "Invalid file format");
		}
		if(lseek(fd2, buff.from, SEEK_SET) == -1){
			err(4, "err");
		}
		for(size_t i = 0; i < buff.len; i ++){
			readsize2 = read(fd2, &numBuff, sizeof(numBuff));
				err(6, "err");
			if(readsize2 == -1){
				err(8, "err");
			}
			if(readsize2 != sizeof(numBuff)){
				errx(8, "bad file format");
			}
			if(write(fd3, &numBuff, sizeof(numBuff)) == -1){
				err(7, "err");
			}	
		}
	}
	if(readsize == -1){
		err(10, "err");
	}	
	if(readsize > 0){
		errx(11, "format err");
	}	

	close(fd1);
	close(fd2);
	close(fd3);

	exit(0);
}
			
