// AndroidDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Example33.h"
#include "AndroidDlg.h"
#include "afxdialogex.h"


// CAndroidDlg 对话框

IMPLEMENT_DYNAMIC(CAndroidDlg, CDialogEx)

CAndroidDlg::CAndroidDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAndroidDlg::IDD, pParent)
{

}

CAndroidDlg::~CAndroidDlg()
{
}

void CAndroidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAndroidDlg, CDialogEx)
END_MESSAGE_MAP()


// CAndroidDlg 消息处理程序
