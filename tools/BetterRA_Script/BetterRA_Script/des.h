

#ifndef _DES_H_
#define _DES_H_

typedef char ElemType;

int DES_MakeSubKeys(ElemType key[64], ElemType subKeys[16][48]);
int DES_EncryptBlock(ElemType plainBlock[8], ElemType subKeys[16][48], ElemType cipherBlock[8]);
int DES_DecryptBlock(ElemType cipherBlock[8], ElemType subKeys[16][48], ElemType plainBlock[8]);
int DES_Encrypt(char *plainFile, char *keyStr, char *cipherFile);
int DES_Decrypt(char *cipherFile, char *keyStr, char *plainFile);
void Encrypt(char *src, int len, const char *keyStr);
void Decrypt(char *src, int len, const char *keyStr);
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);
void HexStrToByte(const char* source, unsigned char* dest, int sourceLen);

#endif
