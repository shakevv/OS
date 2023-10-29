#include <stdint.h>
#include <err.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>

bool check (uint16_t op, uint32_t a, uint32_t b) {
	if(op == 0) {
		return a <= b;
	}
	return a >= b;
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		errx(1, "err");
	}

	int fd1 = open(argv[1],O_RDWR);
	if(fd1 == -1) {
		err(2, "%s", argv[1]);
	}

	int fd2 = open(argv[2], O_RDONLY);
	if(fd2 == -1) {
		close(fd1);
		err(3, "%s", argv[2]);
	}

	struct s1 {
		uint32_t magic;
		uint32_t count;
	}__attribute__((packed));

	struct s1 header;

	
	if(read(fd1, &header, sizeof(struct s1)) != sizeof(struct s1)) {
		close(fd1);
		close(fd2);
		errx(4, "err");
	}

	if(header.magic != 0x21796F4A) {
		close(fd1);
		close(fd2);
		errx(5, "magic err");
	}

	struct stat st;

	if(fstat(fd1, &st) == -1) {
		close(fd1);
		close(fd2);
		err(6, "stat err");
	}

	if(header.count * sizeof(uint64_t) != st.st_size - sizeof(struct s1)) {
		close(fd1);
		close(fd2);
		errx(7, "size err");
	}

	struct s2 {
		uint32_t magic1;
		uint16_t magic2;
		uint16_t reserved;
		uint64_t count;
	}__attribute__((packed));

	struct s2 str2;

	if(read(fd2, &str2, sizeof(struct s2)) == -1) {
		close(fd1);
		close(fd2);
		errx(8, "comparator header err");
	}

	if(str2.magic1 != 0xAFBC7A37 || str2.magic2 != 0x1C27) {
		errx(9, "comparator magic numbers err");
	}

	if(fstat(fd2, &st) == -1) {
		close(fd1);
		close(fd2);
		err(10, "err");
	}

	struct s3 {
		uint16_t type;
		uint16_t reserved[3];
		uint32_t offset1;
		uint32_t offset2;
	}__attribute__((packed));

	if(str2.count * sizeof(struct s3) != st.st_size - sizeof(struct s2)) {
		close(fd1);
		close(fd2);
		errx(11, "comparator size doesn't match count");
	}

	struct s3 comp_buff;
	uint64_t numbers_buff[2];

	for(size_t i = 0; i < str2.count; ++ i) {
	ssize_t	rsize = read(fd2, &comp_buff, sizeof(struct s3));
		if(rsize != sizeof(struct s3)) {
			close(fd1);
			close(fd2);
			err(12, "read err");
		}

		if(lseek(fd1, comp_buff.offset1, SEEK_SET) != comp_buff.offset1) {
			close(fd1);
			close(fd2);
			err(20, "read err");
		}
		if(read(fd1, numbers_buff, sizeof(uint64_t)) != sizeof(uint64_t)) {
			close(fd1);
		   	close(fd2);
			err(47, "err");
		}

		if(lseek(fd1, comp_buff.offset2, SEEK_SET) != comp_buff.offset2) {
			close(fd1);
			close(fd2);
			err(49, "err");
		}
		if(read(fd1, numbers_buff + 1, sizeof(uint64_t)) != sizeof(uint64_t)) {
		   	close(fd1);
		   	close(fd2);
			err(48, "err");
		}

		if(!check(comp_buff.type, numbers_buff[0], numbers_buff[1])) {
			uint64_t temp = numbers_buff[0];

			if(lseek(fd1, comp_buff.offset1, SEEK_SET) == -1) {
				close(fd1);
				close(fd2);
				err(50, "err");
			}
			if(write(fd1, numbers_buff + 1, sizeof(uint64_t)) == -1) {
				close(fd1);
				close(fd2);
				err(51, "err");
			}

			if(lseek(fd1, comp_buff.offset2, SEEK_SET) == -1) {
				close(fd1);
				close(fd2);
				err(52, "err");
			}
			if(write(fd1, &temp, sizeof(uint64_t)) != sizeof(uint64_t)) {
				close(fd1);
				close(fd2);
				err(55, "err");
			}
		}
	}

	close(fd1);
	close(fd2);

	exit(0);
}
