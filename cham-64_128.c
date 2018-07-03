/*
==============================================================================
Name : CHAM.c

Author : JiHwan Lim

Version : 1.0

Copyright : jhim147605@gmail.com  /  010-4001-1510

Description : implementation of CHAM by C-Language (Reference Code)

==============================================================================
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
#define ROUND_NUM 80

u16 ROTL16(u16 K, int n) {
	return (K<<n)|(K>>(16-n));
}

void KS(u16* K, u16* RK) { // Key Scheduling
	int i = 0;
	
	for (i = 0; i < 8; i++) {
		RK[i] = (K[i]) ^ ROTL16(K[i], 1) ^ ROTL16(K[i], 8);
		RK[(i + 8) ^ 1] = (K[i]) ^ ROTL16(K[i], 1) ^ ROTL16(K[i], 11);
	}
}

void Encrypt(u16* X, u16* RK) {
	int i = 0;
	u16 x = 0, x0 = X[0], x1 = X[1], x2 = X[2], x3 = X[3];

	for (i = 0; i < ROUND_NUM; i++) {
		if ((i & 1) == 0) { // if i is even number
			x = ROTL16((x0 ^ i) + (ROTL16(x1, 1) ^ RK[i & 15]), 8);
		}
		else {           // if i is odd number
			x = ROTL16((x0 ^ i) + (ROTL16(x1, 8) ^ RK[i & 15]), 1);
		}
		x0 = x1;
		x1 = x2;
		x2 = x3;
		x3 = x;
	}
	X[0] = x0; X[1] = x1; X[2] = x2; X[3] = x3;
}

int main(void) {
	u16 K[8] = { 0x0100, 0x0302, 0x0504, 0x0706, 0x0908, 0x0b0a, 0x0d0c, 0x0f0e }; // Private Key
	u16 RK[16] = {0,}; // RoundKey
	u16 X[4] = { 0x1100, 0x3322, 0x5544, 0x7766 }; // X is plain Text
	int i = 0;

	printf("Plain Text : ");
	for (i = 0; i < 4; i++) {
		printf("0x%x ", X[i]); // print plain text
	}
	printf("\n");
	
	KS(K, RK); // Key Scheduling process
	
	printf("Round Key : ");
	for (i = 0; i < 16; i++) {
		printf("0x%x ", RK[i]);
	}
	printf("\n");

	Encrypt(X, RK); // Encrypt plain Text

	printf("Cipher Text : ");
	for (i = 0; i < 4; i++) {
		printf("0x%x ", X[i]); // print encrypted text
	}

	printf("\n");

	//////////////////////////Decrypt////////////////////////////
	//Decrypt(X, RK);
	printf("Decrypted Text : ");

	printf("\n\n");
	printf("Right cipher : 0x453c 0x63bc 0xdcfa 0xbf4e\n");

	return 0;
}