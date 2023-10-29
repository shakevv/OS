

void bubble(uint32_t* arr, size_t size){
	for(size_t i = 0; i < size - 1; ++ i){
		for(size_t j = 0; j < size - 1 - i; ++ i){
			if(arr[j] > arr[j+1]){
				uint32_t temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}
}

int main(int argc, char* argv[]){
	if(argc != 3){
		errx(1, "err");
	}

uint32_t arr* = malloc( 2097152 );
if(arr == NULL){
	err(3, "err");
}

int fd1 = open(argv[1], O_RDONLY);

read(fd1, arr, 2097152 * 4);

bubble(arr);

int tempfd = open ("tmp", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

write(tempfd, arr, 2097152 * 4);

lseek(fd1, arr, 2097152 * 4);

read(fd1, arr, 2097152 * 4);

bubble(arr);

int fd2 = creat(argv[2], S_IWUSR | S_IRUSR);



