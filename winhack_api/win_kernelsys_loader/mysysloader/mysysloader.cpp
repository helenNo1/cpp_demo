#include <windows.h>
#include "load_driver.h"

// 全局变量
HINSTANCE hInst;
HWND hWnd;
HWND hBtnSelectFile;
HWND hBtnInstall;
HWND hBtnUninstall;
HWND hBtnStart;
HWND hBtnStop;
HWND hTxtFilePath;
OPENFILENAME ofn;

// 函数声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void InitControls(HWND hWnd);
void OnBtnSelectFileClicked(HWND hWnd);
void OnBtnInstallClicked(HWND hWnd);
void OnBtnUninstallClicked(HWND hWnd);
void OnBtnStartClicked(HWND hWnd);
void OnBtnStopClicked(HWND hWnd);

// 主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    InitScManager();
    MSG msg;
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance,
                      LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
                      (HBRUSH)(COLOR_WINDOW + 1), NULL, L"MyWindowClass", LoadIcon(NULL, IDI_APPLICATION) };

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"窗口注册失败！", L"错误", MB_ICONERROR);
        return 1;
    }

    hInst = hInstance;
    hWnd = CreateWindowEx(0, L"MyWindowClass", L"Driver Management", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        MessageBox(NULL, L"窗口创建失败！", L"错误", MB_ICONERROR);
        return 1;
    }

    InitControls(hWnd);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// 初始化窗口控件
void InitControls(HWND hWnd)
{
    hTxtFilePath = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        12, 12, 260, 23, hWnd, NULL, hInst, NULL);

    hBtnSelectFile = CreateWindow(L"BUTTON", L"Select File", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        12, 40, 100, 30, hWnd, NULL, hInst, NULL);

    hBtnInstall = CreateWindow(L"BUTTON", L"Install", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        120, 40, 80, 30, hWnd, NULL, hInst, NULL);

    hBtnUninstall = CreateWindow(L"BUTTON", L"Uninstall", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        210, 40, 80, 30, hWnd, NULL, hInst, NULL);

    hBtnStart = CreateWindow(L"BUTTON", L"Start", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        12, 80, 80, 30, hWnd, NULL, hInst, NULL);

    hBtnStop = CreateWindow(L"BUTTON", L"Stop", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        120, 80, 80, 30, hWnd, NULL, hInst, NULL);

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = new TCHAR[MAX_PATH];
    ofn.lpstrFile[0] = L'\0';
    ofn.lpstrFilter = L"Driver Files (*.sys)\0*.sys\0All Files (*.*)\0*.*\0";
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = L"Select a Driver File";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
}

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 分析菜单选择:
        switch (wmId)
        {
        case BN_CLICKED:
            if ((HWND)lParam == hBtnSelectFile)
            {
                OnBtnSelectFileClicked(hWnd);
            }
            else if ((HWND)lParam == hBtnInstall)
            {
                OnBtnInstallClicked(hWnd);
            }
            else if ((HWND)lParam == hBtnUninstall)
            {
                OnBtnUninstallClicked(hWnd);
            }
            else if ((HWND)lParam == hBtnStart)
            {
                OnBtnStartClicked(hWnd);
            }
            else if ((HWND)lParam == hBtnStop)
            {
                OnBtnStopClicked(hWnd);
            }
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Select File 按钮点击事件处理函数
void OnBtnSelectFileClicked(HWND hWnd)
{
    if (GetOpenFileName(&ofn) == TRUE)
    {
        SetWindowText(hTxtFilePath, ofn.lpstrFile);
    }
}

// Install 按钮点击事件处理函数
void OnBtnInstallClicked(HWND hWnd)
{
    // 获取文件路径并实现安装驱动的逻辑
    
        LPCTSTR driverPath = ofn.lpstrFile; // 假设这里是获取选择的文件路径
    InstallDriver(schSCManager, L"MyDriver", driverPath); // 调用安装驱动函数
}

// Uninstall 按钮点击事件处理函数
void OnBtnUninstallClicked(HWND hWnd)
{
    // 实现卸载驱动的逻辑
    RemoveDriver(schSCManager, L"MyDriver"); // 调用卸载驱动函数

}

// Start 按钮点击事件处理函数
void OnBtnStartClicked(HWND hWnd)
{
    // 实现启动驱动的逻辑
    StartDriver(schSCManager, L"MyDriver"); // 调用启动驱动函数

}

// Stop 按钮点击事件处理函数
void OnBtnStopClicked(HWND hWnd)
{
    // 实现停止驱动的逻辑
    StopDriver(schSCManager, L"MyDriver"); // 调用停止驱动函数

}