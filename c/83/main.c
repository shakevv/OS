#include <err.h>

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
int main(int argc, char* argv[]){
	//char command[5];
	if(argc == 1){
		strcpy(argv[1],  "echo");
	}
	if(argc > 2 || (argc == 2 && strlen(argv[1]) > 4)){
		errx(1, "invalid args...");
	}

	char buff[50];
	int i = 0;
	int len = 0;
	int cmd_num = 1;
	int total_len = 0;
	ssize_t read_size = 0;
	while((read_size = read(0, buff + i, 1) == 1)){
		len ++;
		if(len > 4 && buff[i] != 0x20 && buff[i] != 0x0A){
			errx(4, "invalid stdin...");
		}
		if(buff[i] == 0x20 || buff[i] == 0x0A){
			len = 0;
			cmd_num ++;
		}
		i ++;
		total_len ++;
	}
	if(read_size == -1){
		err(2, "read err");
	}

	int pos = 0;
	bool cmd1_ready = false;
	bool cmd2_ready = false;
	char cmd1[5];
	char cmd2[5]; 
	for(i = 0; i <= total_len; ++ i){
		if(i == (total_len - 1) && cmd2_ready == false){
			cmd1[pos + 1] = '\0';
			cmd2[0] = '\0';
			cmd1_ready = true;
			cmd2_ready = true;
		}
		if(!cmd1_ready){
			cmd1[pos] = buff[i];
			if(cmd1[pos] == 0x20 || cmd1[pos] == 0x0A){
				cmd1[pos] = '\0';
				cmd1_ready = true;
				pos = 0;
			}
		} else if(!cmd2_ready){
			cmd2[pos] = buff[i];
			if(cmd2[pos] == 0x20 || cmd2[pos] == 0x0A){
				cmd2[pos] = '\0';
				cmd2_ready = true;
				pos = 0;
			}
		} else{
			pid_t pid = fork();
			if(pid == -1){
				err(6, "fork err");
			}
			if(pid == 0){
				printf("\n\nstrlen: %ld\n\n", strlen(cmd1));
				char gogo[strlen(cmd1) + 1];
				strcpy(gogo, cmd1);
				printf("\n\n\n%s\n\n\n", cmd1);
				if(execlp(argv[1], argv[1], gogo,/* cmd2[0] != '\0' ? cmd2 : "",*/ 0) == -1){
					err(7, "execlp err");
				}
			}
			wait(NULL);
			cmd1_ready = false;
			cmd2_ready = false;
		}
			pos ++;
	}

	exit(0);
}
