#ifndef _FILE_H_
#define  _FILE_H_

#include "stdafx.h"
#include <Windows.h>

void BinaryReadByte(const char *filePath, const int num , BYTE *src);
void WriteEncbs2Encfile(char *filePath,const int oldnum, const int num, BYTE *src);
void WriteDecbs2Decfile(char *filePath,const int oldnum, const int num, BYTE *src);
#endif
