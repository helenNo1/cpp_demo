#ifndef _LOAD_DRIVER_H_
#define _LOAD_DRIVER_H_


#include <windows.h>
#include <stdio.h>

extern SC_HANDLE schSCManager; // ����ȫ�ֵķ�����ƹ��������
VOID InitScManager();

BOOL InstallDriver(SC_HANDLE schSCManager, LPCTSTR DriverName, LPCTSTR ServiceExe);

BOOL StartDriver(SC_HANDLE schSCManager, LPCTSTR DriverName);

BOOL StopDriver(SC_HANDLE schSCManager, LPCTSTR DriverName);

BOOL RemoveDriver(SC_HANDLE schSCManager, LPCTSTR DriverName);
#endif
