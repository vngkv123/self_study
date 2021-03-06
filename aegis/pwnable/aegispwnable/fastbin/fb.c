#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

void *chunk_array[0x10];
int check = 0;

void initialize(){
	char buf;
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	setvbuf(stderr, 0, 2, 0);
	int fd = open("./banner.txt", O_RDONLY);
	if( fd < 0 ){ exit(-1); }
	while( read(fd, &buf, 1) )
		write(1, &buf, 1);
	close(fd);
}

void menu(){
	puts("1. Read");
	puts("2. Write");
	puts("3. Delete");
	write(1, "> ", 2);
}

void gRead(){
	int i;
	write(1, "> ", 2);
	for(i = 0; chunk_array[i]; i++);
	chunk_array[i] = malloc(0x48);
	read(0, chunk_array[i], 0x48);
}

void gWrite(){
	int index;
	write(1, "> ", 2);
	scanf("%d", &index);
	if( chunk_array[index] && index >= 0 && index <= 15 ){
		write(1, "> ", 2);
		write(1, chunk_array[index], strlen(chunk_array[index]));
	}
	else{
		puts("[-] Invalid index");
	}
}

void gClear(){
	int i;
	write(1, "> ", 2);
	scanf("%d", &i);
	if( chunk_array[i] && i >= 0 && i <= 15 ){
		free(chunk_array[i]);
		//chunk_array[i] = 0;
	}
}

void once(){
	if(check){ exit(-1); }
	int index;
    write(1, "> ", 2);
    scanf("%d", &index);
	fflush(stdout);
	fflush(stdin);
    if( chunk_array[index] && index >= 0 && index <= 15 ){
    	write(1, "> ", 2);
    	read(0, chunk_array[index], 0x100);
	}
	check = 1;
}

int main(int argc, char *argv[]){
	int choice;
	initialize();
	while(1){
		menu();
		scanf("%d", &choice);
		switch(choice){
			case 1:
				gRead();
				break;
			case 2:
				gWrite();
				break;
			case 3:
				gClear();
				break;
			case 0x1337:
				once();
				break;
			default:
				puts("[-] Invalid choice");
				break;
		}
	}
	return 0;
}
