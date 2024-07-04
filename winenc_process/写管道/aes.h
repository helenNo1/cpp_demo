#ifndef _AES_H_
#define  _AES_H_
#include "stdafx.h"
#include<vector>
#include<string>



#define ENCLEN 3000
#define ENCNUM 1000
#define DECNUM 1008

extern char extstr[1000];

BOOL AesEncrypt(BYTE *pPassword, DWORD dwPasswordLength, BYTE *pData, DWORD &dwDataLength, DWORD dwBufferLength);


BOOL AesDecrypt(BYTE *pPassword, DWORD dwPasswordLength, BYTE *pData, DWORD &dwDataLength, DWORD dwBufferLength);


void Encbs(BYTE *ret, DWORD &num);

BOOL EncFile(char *filePath);

void Decbs(BYTE *ret, DWORD &num);

BOOL DecFile(char *filePath)  ;

void EncList(void * arg1) ;
void DecList(void * arg1) ;

BOOL DecFile(char *filePath)  ;
#endif