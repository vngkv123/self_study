#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char cipher[] = {0x14, 0xf7, 0x4, 0x3, 0x3d, 0x9a, 0x6a, 0x9e, 0x1a, 0xbb, 0xdf, 0xd9, 0x81, 0x92, 0x8d, 0x21, 0x5c, 0xa1, 0x3c, 0xcc, 0x77, 0x84, 0x82, 0xe3, 0x11, 0x3f, 0xf};
unsigned char decrypt[27];

void decryption(unsigned int *key){
	for( int i = 0; i < 27; i++ ){
		decrypt[i] = cipher[i] ^ (key[i] & 0xff);
	}
}

void auth(){
	printf("Flag : [%s]\n", decrypt);
}

int main(int argc, char *argv[]){
	unsigned int key[27];
	int i;
	printf("[+] Challenge 1\n");
	printf("Input your key value\n");
	for( i = 0; i < 27; i++ ){
		scanf("%u", &key[i]);
	}

	decryption(key);
	auth();
	return 0;
}
