
// Example26Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Example26.h"
#include "Example26Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CExample26Dlg 对话框




CExample26Dlg::CExample26Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExample26Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExample26Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_horiScrollbar);
}

BEGIN_MESSAGE_MAP(CExample26Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CExample26Dlg 消息处理程序

BOOL CExample26Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
m_horiScrollbar.SetScrollRange(1, 100);   
    // 设置水平滚动条的初始位置为20   
    m_horiScrollbar.SetScrollPos(20);   
    // 在编辑框中显示20   
    SetDlgItemInt(IDC_EDIT1, 20);   
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CExample26Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CExample26Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CExample26Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CExample26Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int pos = m_horiScrollbar.GetScrollPos();    // 获取水平滚动条当前位置  
  
    switch (nSBCode)   
    {   
        // 如果向左滚动一列，则pos减1  
        case SB_LINELEFT:   
            pos -= 1;   
            break;   
        // 如果向右滚动一列，则pos加1  
        case SB_LINERIGHT:   
            pos  += 1;   
            break;   
        // 如果向左滚动一页，则pos减10  
        case SB_PAGELEFT:   
            pos -= 10;   
            break;   
        // 如果向右滚动一页，则pos加10  
        case SB_PAGERIGHT:   
            pos  += 10;   
            break;   
        // 如果滚动到最左端，则pos为1  
        case SB_LEFT:   
            pos = 1;   
            break;   
        // 如果滚动到最右端，则pos为100  
        case SB_RIGHT:   
            pos = 100;   
            break;     
        // 如果拖动滚动块滚动到指定位置，则pos赋值为nPos的值  
        case SB_THUMBPOSITION:   
            pos = nPos;   
            break;   
        // 下面的m_horiScrollbar.SetScrollPos(pos);执行时会第二次进入此函数，最终确定滚动块位置，并且会直接到default分支，所以在此处设置编辑框中显示数值  
        default:   
            SetDlgItemInt(IDC_EDIT1, pos);  
            return;   
    }   
  
    // 设置滚动块位置  
    m_horiScrollbar.SetScrollPos(pos);  
  
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);  
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
