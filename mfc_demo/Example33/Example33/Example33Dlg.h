
// Example33Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include  "JzmDlg.h"
#include  "AndroidDlg.h"

// CExample33Dlg 对话框
class CExample33Dlg : public CDialogEx
{
// 构造
public:
	CExample33Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EXAMPLE33_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;

    CJzmDlg m_jzmDlg;
       CAndroidDlg m_androidDlg;
	   afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
