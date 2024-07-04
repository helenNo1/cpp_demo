#include "stdafx.h"
#include <Windows.h>
#include <fstream>
#include "file.h"
#include "aes.h"
#include "err.h"
#include "th.h"
#include<vector>
#include<string>
#include<iostream>
using namespace std;


BOOL AesEncrypt(BYTE *pPassword, DWORD dwPasswordLength, BYTE *pData, DWORD &dwDataLength, DWORD dwBufferLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hCryptHash = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		// 获取CSP句柄
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
		if (FALSE == bRet)
		{
			ShowError("CryptAcquireContext");
			break;
		}

		// 创建HASH对象
		bRet = ::CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash);
		if (FALSE == bRet)
		{
			ShowError("CryptCreateHash");
			break;
		}

		// 对密钥进行HASH计算
		bRet = ::CryptHashData(hCryptHash, pPassword, dwPasswordLength, 0);
		if (FALSE == bRet)
		{
			ShowError("CryptHashData");
			break;
		}

		// 使用HASH来生成密钥
		bRet = ::CryptDeriveKey(hCryptProv, CALG_AES_128, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey);
		if (FALSE == bRet)
		{
			ShowError("CryptDeriveKey");
			break;
		}

		// 加密数据
		bRet = ::CryptEncrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength, dwBufferLength);
		if (FALSE == bRet)
		{
			ShowError("CryptEncrypt");
			break;
		}

	} while (FALSE);

	// 关闭释放
	if (hCryptKey)
	{
		::CryptDestroyKey(hCryptKey);
	}
	if (hCryptHash)
	{
		::CryptDestroyHash(hCryptHash);
	}
	if (hCryptProv)
	{
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}


// AES解密
BOOL AesDecrypt(BYTE *pPassword, DWORD dwPasswordLength, BYTE *pData, DWORD &dwDataLength, DWORD dwBufferLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hCryptHash = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		// 获取CSP句柄
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
		if (FALSE == bRet)
		{
			ShowError("CryptAcquireContext");
			break;
		}

		// 创建HASH对象
		bRet = ::CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash);
		if (FALSE == bRet)
		{
			ShowError("CryptCreateHash");
			break;
		}

		// 对密钥进行HASH计算
		bRet = ::CryptHashData(hCryptHash, pPassword, dwPasswordLength, 0);
		if (FALSE == bRet)
		{
			ShowError("CryptHashData");
			break;
		}

		// 使用HASH来生成密钥
		bRet = ::CryptDeriveKey(hCryptProv, CALG_AES_128, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey);
		if (FALSE == bRet)
		{
			ShowError("CryptDeriveKey");
			break;
		}

		// 解密数据
		bRet = ::CryptDecrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength);
		if (FALSE == bRet)
		{
			ShowError("CryptDecrypt");
			break;
		}

	} while (FALSE);

	// 关闭释放
	if (hCryptKey)
	{
		::CryptDestroyKey(hCryptKey);
	}
	if (hCryptHash)
	{
		::CryptDestroyHash(hCryptHash);
	}
	if (hCryptProv)
	{
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}

void Encbs(BYTE *ret, DWORD &num){
	AesEncrypt(
		(BYTE*)"password",
		8, 
		ret,
		num,
		ENCLEN
		);
}

void Decbs(BYTE *ret, DWORD &num){
	AesDecrypt(
		(BYTE*)"password",
		8, 
		ret,
		num,
		ENCLEN
		);
}

void EncList(void * arg1) {
	cout <<"line: " << __LINE__ <<" " << extstr << endl;
	char filePath[1000];		
	vector<string> *list =  (vector<string>*)arg1;
	while (	1)
	{
		WaitForSingleObject(g_Mutex, INFINITE);//等待互斥量 
		if(list->size()>0) {
			strcpy(filePath, list->back().c_str());
			list->pop_back();
		}else {
			break;
		}
		ReleaseMutex(g_Mutex);

		printf("%d todo enc file: %s\n", __LINE__, filePath);
		if(EncFile(filePath) == FALSE) {
			printf("%d, enc file fail: %s\n", __LINE__, filePath);
		}else {
			printf("%d, enc file success: %s\n", __LINE__, filePath);
		}

		
	} ;
		
}

BOOL EncFile(char *filePath){
	char *ss;
	ss = strstr(filePath, ".enc" )  ;
	if(ss  != NULL){
		return FALSE; 
	}

	DWORD dwDataLen = ENCNUM;
	BYTE readbs[ENCLEN] = {0};

	BinaryReadByte(filePath, ENCNUM, readbs);		
/*
	printf("read count[%d]\n", ENCNUM);
	for(int i =0;i<ENCNUM;i++) {
		printf("%x ",  readbs[i]);
	}
	printf("\n");
*/
	Encbs(readbs, dwDataLen);
/*
	printf("enc count[%d]\n", dwDataLen);
	for(int i =0;i<dwDataLen;i++) {
		printf("%x ",  readbs[i]);
	}
	printf("\n");
	*/
	//system("pause");

	WriteEncbs2Encfile(filePath, ENCNUM, dwDataLen, readbs);
	return TRUE;
}

BOOL DecFile(char *filePath)  {
	char *ss;
	ss = strstr(filePath, ".enc" )  ;
	if(ss  == NULL){
		return FALSE; 
	}
	BYTE readbs[ENCLEN] = {0};
	DWORD lold = DECNUM, l = DECNUM;

	BinaryReadByte(filePath, l, readbs);
	
	Decbs(readbs ,l);
	
	WriteDecbs2Decfile(filePath, lold, l, readbs);


	return TRUE;
}

void DecList(void * arg1) {
	cout <<"line: " << __LINE__ <<" " << extstr << endl;
	char filePath[1000];		
	vector<string> *list =  (vector<string>*)arg1;
	while (1)
	{
		WaitForSingleObject(g_Mutex, INFINITE);
		if(list->size()>0){
			strcpy(filePath, list->back().c_str());
			list->pop_back();
		}else {
			break;
		}
		ReleaseMutex(g_Mutex);
		printf("%d todo enc file: %s\n", __LINE__, filePath);
		if(DecFile(filePath) == FALSE){
			printf("%d, dec file fail: %s\n", __LINE__, filePath);
		}else {
			printf("%d, dec file success: %s\n", __LINE__, filePath);
		}
		
	} ;
		
}