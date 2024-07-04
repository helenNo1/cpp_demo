// 加密.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include<stdio.h>
#include <Windows.h>
#include <process.h>
#include <iostream>
#include "aes.h"
#include "err.h"
#include "file.h"
#include "dir.h"
#include "th.h"

#define MYTHNUM 5

using namespace std;

#define BUF_SIZE 4096
// 定义管道名 , 如果是跨网络通信 , 则在圆点处指定服务器端程序所在的主机名
#define EXAMP_PIPE   "\\\\.\\PIPE\\EB3F2E4B_52E2_40F9_A17D_B4A2588F23AB"
int _tmain(int argc, _TCHAR* argv[])
{

	// 创建命名管道
    HANDLE hPipe = NULL;
    hPipe = CreateNamedPipe(
        EXAMP_PIPE,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE |
        PIPE_READMODE_MESSAGE |
        PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        BUF_SIZE,
        BUF_SIZE,
        0,
        NULL);
 
    if (hPipe == INVALID_HANDLE_VALUE)
    {
        cout << "Create Read Pipe Error" << endl;
        return FALSE;
    }
 
 
    cout << "Wait for the connection" << endl;
 
    // 等待客户端的连接
    if (!ConnectNamedPipe(hPipe, NULL))
    {
        cout << "Connect Failed" << endl;
        return FALSE;
    }
 
    DWORD dwReturn = 0;
    char szBuffer[BUF_SIZE] = { 0 };
 
    // 读取客户端数据
    memset(szBuffer, 0, BUF_SIZE);

	while(1) {
		if (ReadFile(hPipe, szBuffer, BUF_SIZE, &dwReturn, NULL))
		{
			szBuffer[dwReturn] = '\0';
		//	cout << szBuffer << endl;
			
			if(EncFile(szBuffer)) {
				cout << "enc or dec " << szBuffer<< " succ" << endl;
			}else {
				cout << "enc or dec" << szBuffer<< " fail" << endl;
			}
			
		}
		else
		{
			cout << "Read Failed" << endl;
			break;
		}
		//Sleep(5000);
	}

 
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
/*
	if(argc != 2) {
		cout << "加密.exe xxx.png";
		exit(1);
	}
	if(EncFile(argv[1]) ==FALSE) {
		exit(1);
	}
*/
	return 0;
}

