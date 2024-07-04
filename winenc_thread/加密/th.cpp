#include "stdafx.h"
#include "th.h"

HANDLE  g_Mutex = CreateMutex(NULL, FALSE, NULL);
unsigned  int __stdcall ThreadFun(PVOID pM){
	printf("线程id号为%4d的子线程说: hello world\n", GetCurrentThreadId());
	return 0;
}

void ct(const int threadnum, unsigned int (__stdcall *f)(PVOID) ) {
	printf("创建多个子线程\n");
	printf("-- by morewindows \n\n");
	HANDLE *handle= new HANDLE[threadnum] ;
	for(int i = 0;i<threadnum;i++) {
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, f, NULL, 0, NULL);
	}
	WaitForMultipleObjects(threadnum, handle, TRUE, INFINITE);
	delete handle;
}
void ctAes(int threadnum, void(*f)(void *),vector<const string> *fileExtList ) {
//	fileExtList.pop_back()	

	printf("创建加密子线程\n");
	printf("-- by morewindows \n\n");
	HANDLE *handle= new HANDLE[threadnum] ;
	for(int i = 0;i<threadnum;i++) {
		handle[i] = (HANDLE)_beginthread(f,0,(void*)fileExtList);
	}
	WaitForMultipleObjects(threadnum, handle, TRUE, INFINITE);
	delete handle;
}