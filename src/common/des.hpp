// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef	_DES_HPP_
#define	_DES_HPP_

#include "cbasetypes.hpp"

/// One 64-bit block.
typedef struct BIT64 { uint8_t b[8]; } BIT64;

void des_decrypt_block(BIT64* block);
void des_decrypt(unsigned char* data, size_t size);

typedef char ElemType;

int DES_MakeSubKeys(ElemType key[64], ElemType subKeys[16][48]);
int DES_EncryptBlock(ElemType plainBlock[8], ElemType subKeys[16][48], ElemType cipherBlock[8]);
int DES_DecryptBlock(ElemType cipherBlock[8], ElemType subKeys[16][48], ElemType plainBlock[8]);
void Encrypt(char *src, int len, const char *keyStr);
void Decrypt(char *src, int len, const char *keyStr);
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen);
void HexStrToByte(const char* source, unsigned char* dest, int sourceLen);

#endif // _DES_HPP_
