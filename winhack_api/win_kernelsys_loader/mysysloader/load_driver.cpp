#include <windows.h>
#include <stdio.h>


#include "load_driver.h"



SC_HANDLE schSCManager;

VOID InitScManager() {

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (schSCManager == NULL)
    {
        MessageBoxW(NULL, L"Failed to open SCManager!", L"Error", MB_ICONERROR);

        return;
    }
}


BOOL InstallDriver(SC_HANDLE schSCManager, LPCTSTR DriverName, LPCTSTR ServiceExe)
{
    SC_HANDLE schService;
    DWORD dwError;

    schService = CreateService(schSCManager,          // SCManager database
        DriverName,            // name of service
        DriverName,            // name to display
        SERVICE_ALL_ACCESS,    // desired access
        SERVICE_KERNEL_DRIVER, // service type
        SERVICE_DEMAND_START,  // start type
        SERVICE_ERROR_NORMAL,  // error control type
        ServiceExe,            // service's binary
        NULL,                  // no load ordering group
        NULL,                  // no tag identifier
        NULL,                  // no dependencies
        NULL,                  // LocalSystem account
        NULL                   // no password
    );

    if (schService == NULL)
    {
        dwError = GetLastError();
        if (dwError == ERROR_SERVICE_EXISTS)
        {
            schService = OpenService(schSCManager, DriverName, SERVICE_ALL_ACCESS);
            if (schService == NULL)
                return FALSE;
        }
        else
            return FALSE;
    }

    CloseServiceHandle(schService);
    return TRUE;
}

BOOL StartDriver(SC_HANDLE schSCManager, LPCTSTR DriverName)
{
    SC_HANDLE schService;
    BOOL bResult = FALSE;

    schService = OpenService(schSCManager, DriverName, SERVICE_ALL_ACCESS);
    if (schService)
    {
        bResult = StartService(schService, 0, NULL);
        CloseServiceHandle(schService);
    }

    return bResult;
}

BOOL StopDriver(SC_HANDLE schSCManager, LPCTSTR DriverName)
{
    SC_HANDLE schService;
    BOOL bResult = FALSE;
    SERVICE_STATUS ss;

    schService = OpenService(schSCManager, DriverName, SERVICE_ALL_ACCESS);
    if (schService)
    {
        bResult = ControlService(schService, SERVICE_CONTROL_STOP, &ss);
        CloseServiceHandle(schService);
    }

    return bResult;
}

BOOL RemoveDriver(SC_HANDLE schSCManager, LPCTSTR DriverName)
{
    SC_HANDLE schService;
    BOOL bResult = FALSE;

    schService = OpenService(schSCManager, DriverName, SERVICE_ALL_ACCESS);
    if (schService)
    {
        bResult = DeleteService(schService);
        CloseServiceHandle(schService);
    }

    return bResult;
}

/*
int main(int argc, char* argv[])
{
    SC_HANDLE schSCManager;
    LPCTSTR driverPath = TEXT("C:\\Path\\To\\Driver.sys");
    LPCTSTR driverName = TEXT("DriverName");

    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (schSCManager == NULL)
    {
        printf("OpenSCManager failed! Error = %d\n", GetLastError());
        return 1;
    }

    if (!InstallDriver(schSCManager, driverName, driverPath))
    {
        printf("InstallDriver failed! Error = %d\n", GetLastError());
        return 1;
    }

    if (!StartDriver(schSCManager, driverName))
    {
        printf("StartDriver failed! Error = %d\n", GetLastError());
        return 1;
    }

    printf("Driver loaded successfully!\n");

    // Your driver is now running, do what you need here

    // Cleanup
    StopDriver(schSCManager, driverName);
    RemoveDriver(schSCManager, driverName);
    CloseServiceHandle(schSCManager);

    printf("Driver unloaded successfully!\n");

    return 0;
}
*/