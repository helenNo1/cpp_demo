#ifndef _TH_H_
#define _TH_H_

#include "stdafx.h"
#include<Windows.h>
#include<process.h>
#include <vector>

using namespace std;
unsigned  int __stdcall ThreadFun(PVOID pM);

extern HANDLE  g_Mutex ;
void ct(int threadnum, unsigned int (__stdcall *f)(PVOID) ) ;
void ctAes(int threadnum, void(*f)(void *),vector<const string> *fileExtList  ) ;
#endif
