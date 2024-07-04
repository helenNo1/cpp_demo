// 启动多个进程exe.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "windows.h"
#include <stdio.h>
#include <tchar.h>
#include<vector>

int _tmain(int argc, _TCHAR* argv[])
{
	if( argc != 2 )
	{
		printf("Usage: %s [cmdline]\n", argv[0]);
		return 0;
	}

	HANDLE *handle= new HANDLE[10] ;
	std::vector<PROCESS_INFORMATION> plist;	

	for(int i=0;i<10;i++) {
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		// Start the child process. 
		if( !CreateProcess( NULL,   // No module name (use command line)
			argv[1],        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi )           // Pointer to PROCESS_INFORMATION structure
			) 
		{
			printf( "CreateProcess failed (%d).\n", GetLastError() );
			return 0;
		}
		handle[i] = pi.hProcess;
		plist.push_back(pi);
	}

	//等待所有子进程结束
	WaitForMultipleObjects(10, handle, TRUE, INFINITE);

	for(int i=0;i<10;i++) {
		CloseHandle( plist[i].hProcess );
		CloseHandle( plist[i].hThread );
	}

	delete handle;

	return 0;
}

