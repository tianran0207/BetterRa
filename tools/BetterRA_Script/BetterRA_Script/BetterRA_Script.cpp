

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* des.h provides the following functions and constants:
*
* generate_key, generate_sub_keys, process_message, ENCRYPTION_MODE, DECRYPTION_MODE
*
*/
#include "des.h"

void MD5_String(const char * string, char * output);


void file_encrypt(const char *inputfile, const char *outputfile, const char *enc_key, bool c_hex);
void file_decrypt(const char *inputfile, const char *outputfile, const char *dec_key);

int main(int argc, char* argv[]) {
	
	//char *input = "C:\\Users\\HOME_CTR\\Documents\\Visual Studio 2015\\Projects\\ConsoleApplication5\\ConsoleApplication5\\ReadMe.txt";
	//char *output = "C:\\Users\\HOME_CTR\\Documents\\Visual Studio 2015\\Projects\\ConsoleApplication5\\ConsoleApplication5\\test.txt";
	//char *key = "12345678";
	//
	//file_encrypt(input, output, key, (argc == 5 && strcmp(argv[5], "hex") == 0));
	//file_encrypt("C:\\Users\\HOME_CTR\\Documents\\Visual Studio 2015\\Projects\\ConsoleApplication5\\ConsoleApplication5\\ReadMe.txt"
	//	, "C:\\Users\\HOME_CTR\\Documents\\Visual Studio 2015\\Projects\\ConsoleApplication5\\ConsoleApplication5\\test.txt", "12344678", true);
	//file_decrypt("C:\\Users\\HOME_CTR\\Documents\\Visual Studio 2015\\Projects\\ConsoleApplication5\\ConsoleApplication5\\test.txt"
	//	, "C:\\Users\\HOME_CTR\\Documents\\Visual Studio 2015\\Projects\\ConsoleApplication5\\ConsoleApplication5\\test2.txt", "12344678");

	if (argc < 2) {
		printf("[加密文件] : -e 待加密文件 保存路径 秘钥 输出格式(HEX:BIT)\n");
		printf("[解密文件] : -d 待解密文件 保存路径 秘钥\n");
		return 1;
	}
	else if (strcmp(argv[1], "-e") == 0) { // Generate key file
		if (argc < 4) {
			printf("[提示] 指定的参数无效. 用法: -e 待加密文件 保存路径 秘钥(必须为8位) 输出16进制文件填(HEX)..\n");
			return 1;
		}
		if (strlen(argv[4]) != 8) {
			printf("[提示] 密码必须为8位..\n");
			return 1;
		}
		file_encrypt(argv[2], argv[3], argv[4], (argc == 6 && strcmp(argv[5], "HEX") == 0));
		return 0;
	}
	else if (strcmp(argv[1], "-d") == 0) {
		if (argc != 5) {
			printf("[提示] 指定的参数无效. 用法: -d 待解密文件 保存路径 秘钥(必须为8位)..\n");
			return 1;
		}
		if (strlen(argv[4]) != 8) {
			printf("[提示] 密码必须为8位..\n");
			return 1;
		}
		file_decrypt(argv[2], argv[3], argv[4]);
		return 0;
	}
	else {
		printf("[提示] 无效的指令: %s. 第一个参数必须为 [ -e | -d ].. -e 为加密. -d 为解密..\n", argv[1]);
		return 1;
	}

}

void file_encrypt(const char *inputfile, const char *outputfile, const char *enc_key, bool c_hex) {

	char *key = "KilPMwQ";
	char *buffer;
	char md5[33], header[128];
	int i, len;
	FILE *output_file, *input_file;

	input_file = fopen(inputfile, "rb");
	if (!input_file) {
		printf("[提示] 无法打开 '%s' 文件..\n", inputfile);
		return;
	}
	if (ferror(input_file)){
		printf("[提示] 读入文件 '%s' 失败..\n", inputfile);
		fclose(input_file);
		return;
	}

	fseek(input_file, 0, SEEK_END);
	len = ftell(input_file);
	buffer = (char *)malloc(len + 40 + 8 + 1);
	memset(buffer, 0, len + 40 + 8 + 1);
	fseek(input_file, 0, SEEK_SET);
	len = fread(buffer + 40, 1, len, input_file);
	buffer[len + 40] = '\0';

	if ((unsigned char)buffer[40] == 0xEF && (unsigned char)buffer[41] == 0xBB && (unsigned char)buffer[42] == 0xBF) {
		printf("[提示] 不支持 UTF-8 编码的文件, '%s' 已跳过..\n", inputfile);
		free(buffer);
		fclose(input_file);
		return;
	}

	if (buffer[40] == 'E' && buffer[41] == 'N' && buffer[42] == 'C') {
		printf("[提示] 是已加密文件 跳过'%s'..\n", inputfile);
		free(buffer);
		fclose(input_file);
		return;
	}

	MD5_String(buffer + 40, md5);
	for (i = 0; i < 40; i++) {
		if (i % 5 == 0)
			header[i] = enc_key[i / 5];
		else
			header[i] = md5[i - i / 5 - 1];
	}

	memcpy(buffer, header, 40);
	Encrypt(buffer, 40, key);
	Encrypt(buffer + 40, len, enc_key);

	if (len % 8 > 0) {
		len = len / 8 * 8 + 8;
	}

	output_file = fopen(outputfile, "wb");
	if (!output_file) {
		printf("[提示] 无法打开 '%s' 文件..\n", outputfile);
	}
	else if (ferror(output_file)) {
		printf("[提示] 写出文件 '%s'失败..\n", outputfile);
		fclose(output_file);
	}
	else if (c_hex) {
		char *hex = (char *)malloc((40 + len) * 2 + 1);
		ByteToHexStr((unsigned char *)buffer, hex, len + 40);
		hex[(len + 40) * 2] = '\0';
		fwrite("ENCHEX", 1, 6, output_file);
		fwrite(hex, 1, (len + 40) * 2, output_file);
		fclose(output_file);
		free(hex);
	} 
	else {
		fwrite("ENCBIT", 1, 6, output_file);
		fwrite(buffer, 1, len + 40, output_file);
		fclose(output_file);
	}
	
	free(buffer);
	fclose(input_file);
	return;
}


void file_decrypt(const char *inputfile, const char *outputfile, const char *dec_key) {


	char *key = "KilPMwQ";
	char *buffer;
	char md5[33], enc_md5[33], enc_key[9], type[7];
	int i, len;
	FILE *output_file = NULL, *input_file;

	input_file = fopen(inputfile, "rb");
	if (!input_file) {
		printf("[提示] 无法打开 '%s' 文件..\n", inputfile);
		return;
	}
	if (ferror(input_file)) {
		printf("[提示] 读入文件 '%s' 失败\n", inputfile);
		fclose(input_file);
		return;
	}


	fseek(input_file, 0, SEEK_END);
	len = ftell(input_file);
	if (len < 46) {
		printf("[提示] 文件的长度不足.. '%s'已跳过\n", inputfile);
		fclose(input_file);
		return;
	}
	fseek(input_file, 0, SEEK_SET);
	fread(type, 1, 6, input_file);
	type[6] = '\0';
	len = len - 6;
	buffer = (char *)malloc(len + 1);
	memset(buffer, 0, len + 1);
	len = fread(buffer, 1, len, input_file);

	buffer[len] = '\0';
	
	if ((unsigned char)buffer[0] == 0xEF && (unsigned char)buffer[1] == 0xBB && (unsigned char)buffer[2] == 0xBF) {
		printf("[提示] 不支持 UTF-8 编码的文件, '%s' 已跳过..\n", inputfile);
		fclose(input_file);
		free(buffer);
		return;
	}

	if (strcmp(type, "ENCHEX") == 0) {
		char *bit = (char *)malloc(len / 2 + 1);
		HexStrToByte(buffer, (unsigned char *)bit, len);
		free(buffer);
		buffer = bit;
		len = len / 2;
	}
	else if (strcmp(type, "ENCBIT") != 0) {
		printf("[提示] 不是加密文件 跳过'%s'..\n", inputfile);
		fclose(input_file);
		free(buffer);
		return;
	}

	Decrypt(buffer, 40, key);

	for (i = 0; i < 40; i++) {
		if (i % 5 == 0){
			enc_key[i / 5] = buffer[i];
		}
		else {
			enc_md5[i - i / 5 - 1] = buffer[i];
		}
	}
	enc_md5[32] = '\0';
	enc_key[8] = '\0';
	if (dec_key && strcmp(enc_key, dec_key) != 0) {
		printf("[提示] 密码验证失败, '%s' 已跳过..\n", inputfile);
		fclose(input_file);
		free(buffer);
		return;
	}

	Decrypt(buffer + 40, len - 40, enc_key);

	MD5_String(buffer + 40, md5);
	md5[32] = '\0';

	if (strcmp(md5, enc_md5) != 0) {
		printf("[提示] 文件已损坏, '%s' 已跳过..\n", inputfile);
		fclose(input_file);
		free(buffer);
		return;
	}

	if (outputfile) {
		output_file = fopen(outputfile, "wb");
		if (!output_file) {
			printf("[提示] 无法打开 '%s' 文件..\n", outputfile);
		}
		else if (ferror(output_file)) {
			printf("[提示] 写出文件 '%s'失败..\n", outputfile);
			fclose(output_file);
		}
		else {
			fwrite(buffer + 40, 1, strlen(buffer) - 40, output_file);
			fclose(output_file);
		}
	}

	free(buffer);
	fclose(input_file);
	return;
}




//-------------------------------------------------------
// Global variable
static unsigned int *pX;

// String Table
static const unsigned int T[] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, //0
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, //4
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, //8
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, //12
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, //16
	0xd62f105d,  0x2441453, 0xd8a1e681, 0xe7d3fbc8, //20
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, //24
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, //28
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, //32
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, //36
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085,  0x4881d05, //40
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, //44
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, //48
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, //52
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, //56
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391  //60
};

// ROTATE_LEFT   The left is made to rotate x [ n-bit ]. This is diverted as it is from RFC.
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

// The function used for other calculation
static unsigned int F(unsigned int X, unsigned int Y, unsigned int Z)
{
	return (X & Y) | (~X & Z);
}
static unsigned int G(unsigned int X, unsigned int Y, unsigned int Z)
{
	return (X & Z) | (Y & ~Z);
}
static unsigned int H(unsigned int X, unsigned int Y, unsigned int Z)
{
	return X ^ Y ^ Z;
}
static unsigned int I(unsigned int X, unsigned int Y, unsigned int Z)
{
	return Y ^ (X | ~Z);
}

static unsigned int Round(unsigned int a, unsigned int b, unsigned int FGHI,
	unsigned int k, unsigned int s, unsigned int i)
{
	return b + ROTATE_LEFT(a + FGHI + pX[k] + T[i], s);
}

static void Round1(unsigned int *a, unsigned int b, unsigned int c,
	unsigned int d, unsigned int k, unsigned int s, unsigned int i)
{
	*a = Round(*a, b, F(b, c, d), k, s, i);
}
static void Round2(unsigned int *a, unsigned int b, unsigned int c,
	unsigned int d, unsigned int k, unsigned int s, unsigned int i)
{
	*a = Round(*a, b, G(b, c, d), k, s, i);
}
static void Round3(unsigned int *a, unsigned int b, unsigned int c,
	unsigned int d, unsigned int k, unsigned int s, unsigned int i)
{
	*a = Round(*a, b, H(b, c, d), k, s, i);
}
static void Round4(unsigned int *a, unsigned int b, unsigned int c,
	unsigned int d, unsigned int k, unsigned int s, unsigned int i)
{
	*a = Round(*a, b, I(b, c, d), k, s, i);
}

static void MD5_Round_Calculate(const unsigned char *block,
	unsigned int *A2, unsigned int *B2, unsigned int *C2, unsigned int *D2)
{
	//create X It is since it is required.
	unsigned int X[16]; //512bit 64byte
	int j, k;

	//Save A as AA, B as BB, C as CC, and and D as DD (saving of A, B, C, and D)
	unsigned int A = *A2, B = *B2, C = *C2, D = *D2;
	unsigned int AA = A, BB = B, CC = C, DD = D;

	//It is a large region variable reluctantly because of calculation of a round. . . for Round1...4
	pX = X;

	//Copy block(padding_message) i into X
	for (j = 0, k = 0; j<64; j += 4, k++)
		X[k] = ((unsigned int)block[j])         // 8byte*4 -> 32byte conversion
		| (((unsigned int)block[j + 1]) << 8) // A function called Decode as used in the field of RFC
		| (((unsigned int)block[j + 2]) << 16)
		| (((unsigned int)block[j + 3]) << 24);


	//Round 1
	Round1(&A, B, C, D, 0, 7, 0); Round1(&D, A, B, C, 1, 12, 1); Round1(&C, D, A, B, 2, 17, 2); Round1(&B, C, D, A, 3, 22, 3);
	Round1(&A, B, C, D, 4, 7, 4); Round1(&D, A, B, C, 5, 12, 5); Round1(&C, D, A, B, 6, 17, 6); Round1(&B, C, D, A, 7, 22, 7);
	Round1(&A, B, C, D, 8, 7, 8); Round1(&D, A, B, C, 9, 12, 9); Round1(&C, D, A, B, 10, 17, 10); Round1(&B, C, D, A, 11, 22, 11);
	Round1(&A, B, C, D, 12, 7, 12); Round1(&D, A, B, C, 13, 12, 13); Round1(&C, D, A, B, 14, 17, 14); Round1(&B, C, D, A, 15, 22, 15);

	//Round 2
	Round2(&A, B, C, D, 1, 5, 16); Round2(&D, A, B, C, 6, 9, 17); Round2(&C, D, A, B, 11, 14, 18); Round2(&B, C, D, A, 0, 20, 19);
	Round2(&A, B, C, D, 5, 5, 20); Round2(&D, A, B, C, 10, 9, 21); Round2(&C, D, A, B, 15, 14, 22); Round2(&B, C, D, A, 4, 20, 23);
	Round2(&A, B, C, D, 9, 5, 24); Round2(&D, A, B, C, 14, 9, 25); Round2(&C, D, A, B, 3, 14, 26); Round2(&B, C, D, A, 8, 20, 27);
	Round2(&A, B, C, D, 13, 5, 28); Round2(&D, A, B, C, 2, 9, 29); Round2(&C, D, A, B, 7, 14, 30); Round2(&B, C, D, A, 12, 20, 31);

	//Round 3
	Round3(&A, B, C, D, 5, 4, 32); Round3(&D, A, B, C, 8, 11, 33); Round3(&C, D, A, B, 11, 16, 34); Round3(&B, C, D, A, 14, 23, 35);
	Round3(&A, B, C, D, 1, 4, 36); Round3(&D, A, B, C, 4, 11, 37); Round3(&C, D, A, B, 7, 16, 38); Round3(&B, C, D, A, 10, 23, 39);
	Round3(&A, B, C, D, 13, 4, 40); Round3(&D, A, B, C, 0, 11, 41); Round3(&C, D, A, B, 3, 16, 42); Round3(&B, C, D, A, 6, 23, 43);
	Round3(&A, B, C, D, 9, 4, 44); Round3(&D, A, B, C, 12, 11, 45); Round3(&C, D, A, B, 15, 16, 46); Round3(&B, C, D, A, 2, 23, 47);

	//Round 4
	Round4(&A, B, C, D, 0, 6, 48); Round4(&D, A, B, C, 7, 10, 49); Round4(&C, D, A, B, 14, 15, 50); Round4(&B, C, D, A, 5, 21, 51);
	Round4(&A, B, C, D, 12, 6, 52); Round4(&D, A, B, C, 3, 10, 53); Round4(&C, D, A, B, 10, 15, 54); Round4(&B, C, D, A, 1, 21, 55);
	Round4(&A, B, C, D, 8, 6, 56); Round4(&D, A, B, C, 15, 10, 57); Round4(&C, D, A, B, 6, 15, 58); Round4(&B, C, D, A, 13, 21, 59);
	Round4(&A, B, C, D, 4, 6, 60); Round4(&D, A, B, C, 11, 10, 61); Round4(&C, D, A, B, 2, 15, 62); Round4(&B, C, D, A, 9, 21, 63);

	// Then perform the following additions. (let's add)
	*A2 = A + AA;
	*B2 = B + BB;
	*C2 = C + CC;
	*D2 = D + DD;

	//The clearance of confidential information
	memset(pX, 0, sizeof(X));
}

static void MD5_String2binary(const char * string, unsigned char * output)
{
	unsigned char padding_message[64];
	unsigned char *pstring;


	unsigned int string_byte_len,
		string_bit_len,
		copy_len,
		msg_digest[4];
	unsigned int *A = &msg_digest[0],
		*B = &msg_digest[1],
		*C = &msg_digest[2],
		*D = &msg_digest[3];
	int i;

	*A = 0x67452301;
	*B = 0xefcdab89;
	*C = 0x98badcfe;
	*D = 0x10325476;

	//1-1
	string_byte_len = (unsigned int)strlen(string);
	pstring = (unsigned char *)string;

	for (i = string_byte_len; 64 <= i; i -= 64, pstring += 64)
		MD5_Round_Calculate(pstring, A, B, C, D);

	//1-3
	copy_len = string_byte_len % 64;
	strncpy((char *)padding_message, (char *)pstring, copy_len);
	memset(padding_message + copy_len, 0, 64 - copy_len);
	padding_message[copy_len] |= 0x80;



	if (56 <= copy_len) {
		MD5_Round_Calculate(padding_message, A, B, C, D);
		memset(padding_message, 0, 56);
	}

	string_bit_len = string_byte_len * 8;
	memcpy(&padding_message[56], &string_bit_len, 4);

	if (UINT_MAX / 8 < string_byte_len) {
		unsigned int high = (string_byte_len - UINT_MAX / 8) * 8;
		memcpy(&padding_message[60], &high, 4);
	}
	else
		memset(&padding_message[60], 0, 4); //In this case, it is good for a higher rank at 0.

											//Step 4.Process Message in 16-Word Blocks (calculation of MD5)
	MD5_Round_Calculate(padding_message, A, B, C, D);

	//Step 5.Output (output)
	memcpy(output, msg_digest, 16);
}

void MD5_Binary(const char * string, unsigned char * output)
{
	MD5_String2binary(string, output);
}

void MD5_String(const char * string, char * output)
{
	unsigned char digest[16];

	MD5_String2binary(string, digest);
	sprintf(output, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
		digest[0], digest[1], digest[2], digest[3],
		digest[4], digest[5], digest[6], digest[7],
		digest[8], digest[9], digest[10], digest[11],
		digest[12], digest[13], digest[14], digest[15]);
}