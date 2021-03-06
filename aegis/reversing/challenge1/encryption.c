#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char cipher[27];

char *encrypt(char *plain, int len){
	unsigned char xor_key;
	for( int i = 0; i < len; i++ ){
		xor_key = rand() & 0xff;
		cipher[i] = plain[i] ^ xor_key;
	}
	return (char *)cipher;
}

int main(int argc, char *argv[]){
	printf("Reversing Challenge 1\n");
	//unsigned char plain_text[] = "s1mple r3versing ch4llenge!";
	unsigned char plain_text[] = "---------------------------";
	unsigned char *cipher = encrypt(plain_text, 27);

	for(int i = 0; i < 27; i++){
		printf("%#x, ", cipher[i]);
	}

	printf("\n");
	for(int i = 0; i < 27; i++){
		printf("%#x, ", plain_text[i]);
	}
	printf("\n");
	return 0;
}
