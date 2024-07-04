#pragma once


// CAndroidDlg 对话框

class CAndroidDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAndroidDlg)

public:
	CAndroidDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAndroidDlg();

// 对话框数据
	enum { IDD = IDD_ANDROID_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
