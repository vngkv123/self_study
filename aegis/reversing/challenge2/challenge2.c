#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/mman.h>
#include <fcntl.h>

char data[] = "\x68\x64\x20\x01\x01\x81\x34\x24\x01\x01\x01\x01\x48\xb8\x5f\x63\x30\x6d\x70\x6c\x65\x74\x50\x48\xb8\x34\x6c\x6c\x65\x6e\x67\x65\x32\x50\x48\xb8\x41\x33\x67\x69\x73\x5f\x63\x68\x50\x48\xc7\xc7\x00\x00\x00\x00\x48\x89\xe6\x48\xc7\xc2\x1a\x00\x00\x00\x48\xc7\xc0\x01\x00\x00\x00\x0f\x05";

void __attribute__((constructor)) detection(void){
	if( ptrace(PTRACE_TRACEME, 0, 1, 0) == -1 ){
		printf("Debugging Detect\n");
		exit(-1);
	}
}

void verify(char *argv[]){
	void *page;
	printf("Do you know how to input non-alphabet value?\n");
	if( strncmp("\xef\xbe\xad\xde", argv[1], 4) == 0){
		page = mmap(0, 0x1000, PROT_EXEC | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1 , 0);
		if( page < 0 ){
			perror("mmap");
			exit(-1);
		}
		memcpy(page, data, sizeof(data));
		((void (*)())page)();
	}
	else{
		printf("Wrong!\n");
		exit(-1);
	}
}

int main(int argc, char *argv[]){
	if( argc != 2 ){
		printf("Usage : %s [key]\n", argv[0]);
		return 0;
	}
	verify(argv);
	return 0;
}
