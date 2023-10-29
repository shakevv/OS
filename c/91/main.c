#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <fcntl.h>

const char* L[] = {"tic ", "tac ", "toe\n"};

int main(int argc, char* argv[]){
	
	if(argc != 3){
		errx(1, "invalid number of args...");
	}

	char* ptr;
	long n = strtol(argv[1], &ptr, 10);
	long w = strtol(argv[2], &ptr, 10);
	
	int a[n + 1][2];

	for(long i = 0; i < n + 1; ++ i){
		pipe(a[i]);
	}
	long word = 0;
	uint8_t pnum = 0;

	for(long i = 0; i < n; ++ i){
		int pid = fork();
		if(pid == 0){
			pnum = i + 1;
			
			for(long j = 0; j < n + 1; ++ j){
				if(j == i + 1){
					close(a[j][0]);
					continue;
				}
				if(j == i ){
					close(a[j][1]);
					continue;
				}
				close(a[j][0]);
				close(a[j][1]);
			}

			while(read(a[i][0], &word, sizeof(word)) > 0){
				if(word == w){
					close(a[i + 1][1]);
					exit(0);
				}

				char x = '0' + pnum;
				write(1, &x, sizeof(x));
				write(1, L[word % 3], 4);
				word ++;
				write(a[i + 1][1], &word, sizeof(word));
			
			}

			exit(0);
		}

	}

	if(pnum == 0){
		for(long i = 0; i < n + 1; ++ i){
			if(i == 0){
				close(a[i][0]);
				continue;
			}
			if(i == n){
				close(a[i][1]);
				continue;
			}
			close(a[i][0]);
			close(a[i][1]);
		}

			char x = '0' + pnum;
			write(1, &x, sizeof(x));
			write(1, L[word], 4);
			word ++;
			write(a[0][1],&word, sizeof(word));


		while(read(a[n][0], &word, sizeof(word)) > 0){
			if(word == w){
				close(a[0][1]);
				exit(0);
			}
			x = '0' + pnum;
			write(1, &x, sizeof(x));
			write(1, L[word % 3], 4);
			word ++;
			write(a[0][1], &word, sizeof(word));
		}

//		printf("\n\nword: %ld\n\n", word);
		exit(0);
	}



}
