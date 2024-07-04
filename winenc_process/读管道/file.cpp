#include "stdafx.h"
#include <Windows.h>
#include<fstream>
#include "file.h"

using namespace std;

void BinaryReadByte(const char *filePath, const int num, BYTE *src){
	ifstream f(filePath, ios::in|ios::binary);
	f.read((char*)src, num);	
	f.close();
}

void WriteEncbs2Encfile(char *filePath,const int oldnum, const int num, BYTE *src) {
	/*
	printf("src count[%d]\n", num);
	for(int i =0;i<num;i++) {
		printf("%x ",  src[i]);
	}
	printf("\n");
	*/
	//system("pause");


//	printf("strlen filePath %d\n",strlen(filePath) );

	bool bRet = true;
	char * noEncFilePath = new char[strlen(filePath) +1];
	strncpy(noEncFilePath, filePath, strlen(filePath));
	noEncFilePath[strlen(filePath)] = 0;

	//printf("1 filePath %s#\n", filePath);
	//printf("2 noEncFilePath %s#\n", noEncFilePath);
	strncat(filePath, ".enc", 4);	
	//printf("line:%d srcfile:%s encfile:%s\n", __LINE__, noEncFilePath , filePath);
	//printf("1 filePath %s! %d\n", filePath, strlen(filePath));
	//printf("2 noEncFilePath %s! %d\n", noEncFilePath, strlen(noEncFilePath));

	ofstream fo(filePath, ios::out|ios::binary);
	fo.write((char*)src, num);

	ifstream fi(noEncFilePath, ios::in|ios::binary);
	fi.seekg(oldnum, ios::beg);
	while(!fi.eof()) {
		BYTE szBuf[256] = {0};
		fi.read((char*)szBuf, sizeof(BYTE)*256);
		int realReadLen = fi.gcount();
		if(fo.bad()) {
			bRet = false;
			break;
		}	
		fo.write((char*)szBuf, realReadLen);
	}

	fi.close();
	fo.close();
	if(remove(noEncFilePath) != 0) {
		printf("É¾³ýÊ§°Ü\n");
	}
	delete []noEncFilePath;
}


void WriteDecbs2Decfile(char *filePath,const int oldnum, const int num, BYTE *src){
	/*
	printf("src count[%d]\n", num);
	for(int i =0;i<num;i++) {
		printf("%x ",  src[i]);
	}
	printf("\n");

*/
	bool bRet = true;
	char * noEncFilePath = new char[strlen(filePath)+1];
	strncpy(noEncFilePath, filePath, strlen(filePath));
	noEncFilePath[strlen(filePath)] = 0;

	//strncat(filePath, ".dec", 4);
	strncpy(filePath, filePath, strlen(filePath) -4);
	filePath[strlen(filePath)-4] =0;

	//printf("line:%d srcfile:%s decfile:%s\n", __LINE__ , noEncFilePath, filePath);
	//system("pause");

	ofstream fo(filePath, ios::out|ios::binary);
	fo.write((char*)src, num);

	ifstream fi(noEncFilePath, ios::in|ios::binary);
	fi.seekg(oldnum, ios::beg);
	while(!fi.eof()) {
		BYTE szBuf[256] = {0};
		fi.read((char*)szBuf, sizeof(BYTE)*256);
		int realReadLen = fi.gcount();
		if(fo.bad()) {
			bRet = false;
			break;
		}	
		fo.write((char*)szBuf, realReadLen);
	}

	fo.close();
	fi.close();
	if(remove(noEncFilePath) != 0) {
		printf("É¾³ýÊ§°Ü\n");
	}
	delete []noEncFilePath;
}
