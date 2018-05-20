// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder
#include "cbasetypes.h"
#include "des.h"


/// DES (Data Encryption Standard) algorithm, modified version.
/// @see http://www.eathena.ws/board/index.php?autocom=bugtracker&showbug=5099.
/// @see http://en.wikipedia.org/wiki/Data_Encryption_Standard
/// @see http://en.wikipedia.org/wiki/DES_supplementary_material


/// Bitmask for accessing individual bits of a byte.
static const uint8_t mask[8] = {
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
};


/// Initial permutation (IP).
static void IP(BIT64* src)
{
	BIT64 tmp = {{0}};

	static const uint8_t ip_table[64] = {
		58, 50, 42, 34, 26, 18, 10,  2,
		60, 52, 44, 36, 28, 20, 12,  4,
		62, 54, 46, 38, 30, 22, 14,  6,
		64, 56, 48, 40, 32, 24, 16,  8,
		57, 49, 41, 33, 25, 17,  9,  1,
		59, 51, 43, 35, 27, 19, 11,  3,
		61, 53, 45, 37, 29, 21, 13,  5,
		63, 55, 47, 39, 31, 23, 15,  7,
	};

	size_t i;
	for( i = 0; i < ARRAYLENGTH(ip_table); ++i )
	{
		uint8_t j = ip_table[i] - 1;
		if( src->b[(j >> 3) & 7] &  mask[j & 7] )
			tmp .b[(i >> 3) & 7] |= mask[i & 7];
	}

	*src = tmp;
}


/// Final permutation (IP^-1).
static void FP(BIT64* src)
{
	BIT64 tmp = {{0}};

	static const uint8_t fp_table[64] = {
		40,  8, 48, 16, 56, 24, 64, 32,
		39,  7, 47, 15, 55, 23, 63, 31,
		38,  6, 46, 14, 54, 22, 62, 30,
		37,  5, 45, 13, 53, 21, 61, 29,
		36,  4, 44, 12, 52, 20, 60, 28,
		35,  3, 43, 11, 51, 19, 59, 27,
		34,  2, 42, 10, 50, 18, 58, 26,
		33,  1, 41,  9, 49, 17, 57, 25,
	};

	size_t i;
	for( i = 0; i < ARRAYLENGTH(fp_table); ++i )
	{
		uint8_t j = fp_table[i] - 1;
		if( src->b[(j >> 3) & 7] &  mask[j & 7] )
			tmp .b[(i >> 3) & 7] |= mask[i & 7];
	}

	*src = tmp;
}


/// Expansion (E).
/// Expands upper four 8-bits (32b) into eight 6-bits (48b).
static void E(BIT64* src)
{
	BIT64 tmp = {{0}};

if( false )
{// original
	static const uint8_t expand_table[48] = {
		32,  1,  2,  3,  4,  5,
		 4,  5,  6,  7,  8,  9,
		 8,  9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32,  1,
	};

	size_t i;
	for( i = 0; i < ARRAYLENGTH(expand_table); ++i )
	{
		uint8_t j = expand_table[i] - 1;
		if( src->b[j / 8 + 4] &  mask[j % 8] )
			tmp .b[i / 6 + 0] |= mask[i % 6];
	}
}
else
{// optimized
	tmp.b[0] = ((src->b[7]<<5) | (src->b[4]>>3)) & 0x3f;	// ..0 vutsr
	tmp.b[1] = ((src->b[4]<<1) | (src->b[5]>>7)) & 0x3f;	// ..srqpo n
	tmp.b[2] = ((src->b[4]<<5) | (src->b[5]>>3)) & 0x3f;	// ..o nmlkj
	tmp.b[3] = ((src->b[5]<<1) | (src->b[6]>>7)) & 0x3f;	// ..kjihg f
	tmp.b[4] = ((src->b[5]<<5) | (src->b[6]>>3)) & 0x3f;	// ..g fedcb
	tmp.b[5] = ((src->b[6]<<1) | (src->b[7]>>7)) & 0x3f;	// ..cba98 7
	tmp.b[6] = ((src->b[6]<<5) | (src->b[7]>>3)) & 0x3f;	// ..8 76543
	tmp.b[7] = ((src->b[7]<<1) | (src->b[4]>>7)) & 0x3f;	// ..43210 v
}

	*src = tmp;
}


/// Transposition (P-BOX).
static void TP(BIT64* src)
{
	BIT64 tmp = {{0}};

	static const uint8_t tp_table[32] = {
		16,  7, 20, 21,
		29, 12, 28, 17,
		 1, 15, 23, 26,
		 5, 18, 31, 10,
		 2,  8, 24, 14,
		32, 27,  3,  9,
		19, 13, 30,  6,
		22, 11,  4, 25,
	};

	size_t i;
	for( i = 0; i < ARRAYLENGTH(tp_table); ++i )
	{
		uint8_t j = tp_table[i] - 1;
		if( src->b[(j >> 3) + 0] &  mask[j & 7] )
			tmp .b[(i >> 3) + 4] |= mask[i & 7];
	}

	*src = tmp;
}


/// Substitution boxes (S-boxes).
/// NOTE: This implementation was optimized to process two nibbles in one step (twice as fast).
static void SBOX(BIT64* src)
{
	BIT64 tmp = {{0}};

	static const uint8_t s_table[4][64] = {
		  {
			0xef, 0x03, 0x41, 0xfd, 0xd8, 0x74, 0x1e, 0x47,  0x26, 0xef, 0xfb, 0x22, 0xb3, 0xd8, 0x84, 0x1e,
			0x39, 0xac, 0xa7, 0x60, 0x62, 0xc1, 0xcd, 0xba,  0x5c, 0x96, 0x90, 0x59, 0x05, 0x3b, 0x7a, 0x85,
			0x40, 0xfd, 0x1e, 0xc8, 0xe7, 0x8a, 0x8b, 0x21,  0xda, 0x43, 0x64, 0x9f, 0x2d, 0x14, 0xb1, 0x72,
			0xf5, 0x5b, 0xc8, 0xb6, 0x9c, 0x37, 0x76, 0xec,  0x39, 0xa0, 0xa3, 0x05, 0x52, 0x6e, 0x0f, 0xd9,
		},{
			0xa7, 0xdd, 0x0d, 0x78, 0x9e, 0x0b, 0xe3, 0x95,  0x60, 0x36, 0x36, 0x4f, 0xf9, 0x60, 0x5a, 0xa3,
			0x11, 0x24, 0xd2, 0x87, 0xc8, 0x52, 0x75, 0xec,  0xbb, 0xc1, 0x4c, 0xba, 0x24, 0xfe, 0x8f, 0x19,
			0xda, 0x13, 0x66, 0xaf, 0x49, 0xd0, 0x90, 0x06,  0x8c, 0x6a, 0xfb, 0x91, 0x37, 0x8d, 0x0d, 0x78,
			0xbf, 0x49, 0x11, 0xf4, 0x23, 0xe5, 0xce, 0x3b,  0x55, 0xbc, 0xa2, 0x57, 0xe8, 0x22, 0x74, 0xce,
		},{
			0x2c, 0xea, 0xc1, 0xbf, 0x4a, 0x24, 0x1f, 0xc2,  0x79, 0x47, 0xa2, 0x7c, 0xb6, 0xd9, 0x68, 0x15,
			0x80, 0x56, 0x5d, 0x01, 0x33, 0xfd, 0xf4, 0xae,  0xde, 0x30, 0x07, 0x9b, 0xe5, 0x83, 0x9b, 0x68,
			0x49, 0xb4, 0x2e, 0x83, 0x1f, 0xc2, 0xb5, 0x7c,  0xa2, 0x19, 0xd8, 0xe5, 0x7c, 0x2f, 0x83, 0xda,
			0xf7, 0x6b, 0x90, 0xfe, 0xc4, 0x01, 0x5a, 0x97,  0x61, 0xa6, 0x3d, 0x40, 0x0b, 0x58, 0xe6, 0x3d,
		},{
			0x4d, 0xd1, 0xb2, 0x0f, 0x28, 0xbd, 0xe4, 0x78,  0xf6, 0x4a, 0x0f, 0x93, 0x8b, 0x17, 0xd1, 0xa4,
			0x3a, 0xec, 0xc9, 0x35, 0x93, 0x56, 0x7e, 0xcb,  0x55, 0x20, 0xa0, 0xfe, 0x6c, 0x89, 0x17, 0x62,
			0x17, 0x62, 0x4b, 0xb1, 0xb4, 0xde, 0xd1, 0x87,  0xc9, 0x14, 0x3c, 0x4a, 0x7e, 0xa8, 0xe2, 0x7d,
			0xa0, 0x9f, 0xf6, 0x5c, 0x6a, 0x09, 0x8d, 0xf0,  0x0f, 0xe3, 0x53, 0x25, 0x95, 0x36, 0x28, 0xcb,
		}
	};

	size_t i;
	for( i = 0; i < ARRAYLENGTH(s_table); ++i )
	{
		tmp.b[i] = (s_table[i][src->b[i*2+0]] & 0xf0)
		         | (s_table[i][src->b[i*2+1]] & 0x0f);
	}

	*src = tmp;
}


/// DES round function.
/// XORs src[0..3] with TP(SBOX(E(src[4..7]))).
static void RoundFunction(BIT64* src)
{
	BIT64 tmp = *src;
	E(&tmp);
	SBOX(&tmp);
	TP(&tmp);

	src->b[0] ^= tmp.b[4];
	src->b[1] ^= tmp.b[5];
	src->b[2] ^= tmp.b[6];
	src->b[3] ^= tmp.b[7];
}


void des_decrypt_block(BIT64* block)
{
	IP(block);
	RoundFunction(block);
	FP(block);
}


void des_decrypt(unsigned char* data, size_t size)
{
	BIT64* p = (BIT64*)data;
	size_t i;

	for( i = 0; i*8 < size; i += 8 )
		des_decrypt_block(p);
}

#include <string.h>

#define PLAIN_FILE_OPEN_ERROR -1  
#define KEY_FILE_OPEN_ERROR -2  
#define CIPHER_FILE_OPEN_ERROR -3  
#define OK 1     

//初始置换表IP  
int IP_Table[64] = {
	57,49,41,33,25,17 ,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7,
	56,48,40,32,24,16, 8,0,
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6 };
//逆初始置换表IP^-1  
int IP_1_Table[64] = {
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41, 9,49,17,57,25,
	32,0,40, 8,48,16,56,24 };

//扩充置换表E  
int E_Table[48] = {
	31, 0, 1, 2, 3, 4,
	 3, 4, 5, 6, 7, 8,
	 7, 8, 9,10,11,12,
	11,12,13,14,15,16,
	15,16,17,18,19,20,
	19,20,21,22,23,24,
	23,24,25,26,27,28,
	27,28,29,30,31, 0 };

//置换函数P  
int P_Table[32] = {
	15, 6,19,20,28,11,27,16,
	 0,14,22,25, 4,17,30, 9,
	 1, 7,23,13,31,26, 2, 8,
	18,12,29, 5,21,10, 3,24 };

//S盒  
int S[8][4][16] = { //S1
	{ { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 },
	{ 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
	{ 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
	{ 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 } },
	//S2  
	{ { 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 },
	{ 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 },
	{ 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 },
	{ 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 } },
	//S3  
	{ { 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 },
	{ 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 },
	{ 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 },
	{ 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 } },
	//S4  
	{ { 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 },
	{ 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 },
	{ 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 },
	{ 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 } },
	//S5  
	{ { 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 },
	{ 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 },
	{ 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 },
	{ 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 } },
	//S6  
	{ { 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 },
	{ 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 },
	{ 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 },
	{ 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 } },
	//S7  
	{ { 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 },
	{ 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 },
	{ 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 },
	{ 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 } },
	//S8  
	{ { 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 },
	{ 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 },
	{ 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 },
	{ 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 } } };


//置换选择1  
int PC_1[56] = {
	56,48,40,32,24,16, 8,
	 0,57,49,41,33,25,17,
	 9, 1,58,50,42,34,26,
	18,10, 2,59,51,43,35,
	62,54,46,38,30,22,14,
	 6,61,53,45,37,29,21,
	13, 5,60,52,44,36,28,
	20,12, 4,27,19,11, 3 };

//置换选择2  
int PC_2[48] = {
	13,16,10,23, 0, 4, 2,27,
	14, 5,20, 9,22,18,11, 3,
	25, 7,15, 6,26,19,12, 1,
	40,51,30,36,46,54,29,39,
	50,44,32,46,43,48,38,55,
	33,52,45,41,49,35,28,31 };

//对左移次数的规定  
int MOVE_TIMES[16] = { 1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1 };

int ByteToBit(ElemType ch, ElemType bit[8]);
int BitToByte(ElemType bit[8], ElemType *ch);
int Char8ToBit64(ElemType ch[8], ElemType bit[64]);
int Bit64ToChar8(ElemType bit[64], ElemType ch[8]);
int DES_PC1_Transform(ElemType key[64], ElemType tempbts[56]);
int DES_PC2_Transform(ElemType key[56], ElemType tempbts[48]);
int DES_ROL(ElemType data[56], int time);
int DES_IP_Transform(ElemType data[64]);
int DES_IP_1_Transform(ElemType data[64]);
int DES_E_Transform(ElemType data[48]);
int DES_P_Transform(ElemType data[32]);
int DES_SBOX(ElemType data[48]);
int DES_XOR(ElemType R[48], ElemType L[48], int count);
int DES_Swap(ElemType left[32], ElemType right[32]);

void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)
{
	int i;
	unsigned char highByte, lowByte;

	for (i = 0; i < sourceLen; i++)
	{
		highByte = source[i] >> 4;
		lowByte = source[i] & 0x0f;

		highByte += 0x30;

		if (highByte > 0x39)
			dest[i * 2] = highByte + 0x07;
		else
			dest[i * 2] = highByte;

		lowByte += 0x30;
		if (lowByte > 0x39)
			dest[i * 2 + 1] = lowByte + 0x07;
		else
			dest[i * 2 + 1] = lowByte;
	}
	return;
}

#include <ctype.h>

void HexStrToByte(const char* source, unsigned char* dest, int sourceLen)
{
	int i;
	unsigned char highByte, lowByte;

	for (i = 0; i < sourceLen; i += 2)
	{
		highByte = toupper(source[i]);
		lowByte = toupper(source[i + 1]);

		if (highByte > 0x39)
			highByte -= 0x37;
		else
			highByte -= 0x30;

		if (lowByte > 0x39)
			lowByte -= 0x37;
		else
			lowByte -= 0x30;

		dest[i / 2] = (highByte << 4) | lowByte;
	}
	return;
}

//字节转换成二进制  
int ByteToBit(ElemType ch, ElemType bit[8]) {
	int cnt;
	for (cnt = 0; cnt < 8; cnt++) {
		*(bit + cnt) = (ch >> cnt) & 1;
	}
	return 0;
}

//二进制转换成字节  
int BitToByte(ElemType bit[8], ElemType *ch) {
	int cnt;
	for (cnt = 0; cnt < 8; cnt++) {
		*ch |= *(bit + cnt) << cnt;
	}
	return 0;
}

//将长度为8的字符串转为二进制位串  
int Char8ToBit64(ElemType ch[8], ElemType bit[64]) {
	int cnt;
	for (cnt = 0; cnt < 8; cnt++) {
		ByteToBit(*(ch + cnt), bit + (cnt << 3));
	}
	return 0;
}

//将二进制位串转为长度为8的字符串  
int Bit64ToChar8(ElemType bit[64], ElemType ch[8]) {
	int cnt;
	memset(ch, 0, 8);
	for (cnt = 0; cnt < 8; cnt++) {
		BitToByte(bit + (cnt << 3), ch + cnt);
	}
	return 0;
}

//生成子密钥  
int DES_MakeSubKeys(ElemType key[64], ElemType subKeys[16][48]) {
	ElemType temp[56];
	int cnt;
	DES_PC1_Transform(key, temp);//PC1置换  
	for (cnt = 0; cnt < 16; cnt++) {//16轮跌代，产生16个子密钥  
		DES_ROL(temp, MOVE_TIMES[cnt]);//循环左移  
		DES_PC2_Transform(temp, subKeys[cnt]);//PC2置换，产生子密钥  
	}
	return 0;
}

//密钥置换1  
int DES_PC1_Transform(ElemType key[64], ElemType tempbts[56]) {
	int cnt;
	for (cnt = 0; cnt < 56; cnt++) {
		tempbts[cnt] = key[PC_1[cnt]];
	}
	return 0;
}

//密钥置换2  
int DES_PC2_Transform(ElemType key[56], ElemType tempbts[48]) {
	int cnt;
	for (cnt = 0; cnt < 48; cnt++) {
		tempbts[cnt] = key[PC_2[cnt]];
	}
	return 0;
}

//循环左移  
int DES_ROL(ElemType data[56], int time) {
	ElemType temp[56];

	//保存将要循环移动到右边的位  
	memcpy(temp, data, time);
	memcpy(temp + time, data + 28, time);

	//前28位移动  
	memcpy(data, data + time, 28 - time);
	memcpy(data + 28 - time, temp, time);

	//后28位移动  
	memcpy(data + 28, data + 28 + time, 28 - time);
	memcpy(data + 56 - time, temp + time, time);

	return 0;
}

//IP置换  
int DES_IP_Transform(ElemType data[64]) {
	int cnt;
	ElemType temp[64];
	for (cnt = 0; cnt < 64; cnt++) {
		temp[cnt] = data[IP_Table[cnt]];
	}
	memcpy(data, temp, 64);
	return 0;
}

//IP逆置换  
int DES_IP_1_Transform(ElemType data[64]) {
	int cnt;
	ElemType temp[64];
	for (cnt = 0; cnt < 64; cnt++) {
		temp[cnt] = data[IP_1_Table[cnt]];
	}
	memcpy(data, temp, 64);
	return 0;
}

//扩展置换  
int DES_E_Transform(ElemType data[48]) {
	int cnt;
	ElemType temp[48];
	for (cnt = 0; cnt < 48; cnt++) {
		temp[cnt] = data[E_Table[cnt]];
	}
	memcpy(data, temp, 48);
	return 0;
}

//P置换  
int DES_P_Transform(ElemType data[32]) {
	int cnt;
	ElemType temp[32];
	for (cnt = 0; cnt < 32; cnt++) {
		temp[cnt] = data[P_Table[cnt]];
	}
	memcpy(data, temp, 32);
	return 0;
}

//异或  
int DES_XOR(ElemType R[48], ElemType L[48], int count) {
	int cnt;
	for (cnt = 0; cnt < count; cnt++) {
		R[cnt] ^= L[cnt];
	}
	return 0;
}

//S盒置换  
int DES_SBOX(ElemType data[48]) {
	int cnt;
	int line, row, output;
	int cur1, cur2;
	for (cnt = 0; cnt < 8; cnt++) {
		cur1 = cnt * 6;
		cur2 = cnt << 2;

		//计算在S盒中的行与列  
		line = (data[cur1] << 1) + data[cur1 + 5];
		row = (data[cur1 + 1] << 3) + (data[cur1 + 2] << 2)
			+ (data[cur1 + 3] << 1) + data[cur1 + 4];
		output = S[cnt][line][row];

		//化为2进制  
		data[cur2] = (output & 0X08) >> 3;
		data[cur2 + 1] = (output & 0X04) >> 2;
		data[cur2 + 2] = (output & 0X02) >> 1;
		data[cur2 + 3] = output & 0x01;
	}
	return 0;
}

//交换  
int DES_Swap(ElemType left[32], ElemType right[32]) {
	ElemType temp[32];
	memcpy(temp, left, 32);
	memcpy(left, right, 32);
	memcpy(right, temp, 32);
	return 0;
}

//加密单个分组  
int DES_EncryptBlock(ElemType plainBlock[8], ElemType subKeys[16][48], ElemType cipherBlock[8]) {
	ElemType plainBits[64];
	ElemType copyRight[48];
	int cnt;

	Char8ToBit64(plainBlock, plainBits);
	//初始置换（IP置换）  
	DES_IP_Transform(plainBits);

	//16轮迭代  
	for (cnt = 0; cnt < 16; cnt++) {
		memcpy(copyRight, plainBits + 32, 32);
		//将右半部分进行扩展置换，从32位扩展到48位  
		DES_E_Transform(copyRight);
		//将右半部分与子密钥进行异或操作  
		DES_XOR(copyRight, subKeys[cnt], 48);
		//异或结果进入S盒，输出32位结果  
		DES_SBOX(copyRight);
		//P置换  
		DES_P_Transform(copyRight);
		//将明文左半部分与右半部分进行异或  
		DES_XOR(plainBits, copyRight, 32);
		if (cnt != 15) {
			//最终完成左右部的交换  
			DES_Swap(plainBits, plainBits + 32);
		}
	}
	//逆初始置换（IP^1置换）  
	DES_IP_1_Transform(plainBits);
	Bit64ToChar8(plainBits, cipherBlock);
	return 0;
}

//解密单个分组  
int DES_DecryptBlock(ElemType cipherBlock[8], ElemType subKeys[16][48], ElemType plainBlock[8]) {
	ElemType cipherBits[64];
	ElemType copyRight[48];
	int cnt;

	Char8ToBit64(cipherBlock, cipherBits);
	//初始置换（IP置换）  
	DES_IP_Transform(cipherBits);

	//16轮迭代  
	for (cnt = 15; cnt >= 0; cnt--) {
		memcpy(copyRight, cipherBits + 32, 32);
		//将右半部分进行扩展置换，从32位扩展到48位  
		DES_E_Transform(copyRight);
		//将右半部分与子密钥进行异或操作  
		DES_XOR(copyRight, subKeys[cnt], 48);
		//异或结果进入S盒，输出32位结果  
		DES_SBOX(copyRight);
		//P置换  
		DES_P_Transform(copyRight);
		//将明文左半部分与右半部分进行异或  
		DES_XOR(cipherBits, copyRight, 32);
		if (cnt != 0) {
			//最终完成左右部的交换  
			DES_Swap(cipherBits, cipherBits + 32);
		}
	}
	//逆初始置换（IP^1置换）  
	DES_IP_1_Transform(cipherBits);
	Bit64ToChar8(cipherBits, plainBlock);
	return 0;
}

void Encrypt(char *src, int len, const char *keyStr) {
	ElemType code[8];
	ElemType keyBlock[8];
	ElemType bKey[64];
	ElemType subKeys[16][48];
	int i;

	//设置密钥  
	memcpy(keyBlock, keyStr, 8);
	//将密钥转换为二进制流  
	Char8ToBit64(keyBlock, bKey);
	//生成子密钥  
	DES_MakeSubKeys(bKey, subKeys);

	for (i = 0; i < len; i = i + 8) {
		DES_EncryptBlock((ElemType *)(src + i), subKeys, code);
		memcpy(src + i, code, 8);
	}
}

void Decrypt(char *src, int len, const char *keyStr) {
	ElemType code[8];
	ElemType keyBlock[8];
	ElemType bKey[64];
	ElemType subKeys[16][48];
	int i;

	//设置密钥  
	memcpy(keyBlock, keyStr, 8);
	//将密钥转换为二进制流  
	Char8ToBit64(keyBlock, bKey);
	//生成子密钥  
	DES_MakeSubKeys(bKey, subKeys);

	for (i = 0; i < len; i = i + 8) {
		DES_DecryptBlock((ElemType *)(src + i), subKeys, code);
		memcpy(src + i, code, 8);
	}
}