// ACMNetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NetSet.h"
#include "ACMNet.h"
#include "ACMNetDlg.h"
#include "afxdialogex.h"
#include "Tools.h"

using namespace Tools;

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


// CACMNetDlg 对话框


CACMNetDlg::CACMNetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CACMNetDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_CONN);
}

void CACMNetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMAND, mycmd);
}

BEGIN_MESSAGE_MAP(CACMNetDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SEND, &CACMNetDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_CANCEL, &CACMNetDlg::OnBnClickedCancel)
	ON_COMMAND(ID_SETTING, &CACMNetDlg::OnSetting)
	ON_COMMAND(ID_SHOWSET, &CACMNetDlg::OnShowset)
END_MESSAGE_MAP()


// CACMNetDlg 消息处理程序

BOOL CACMNetDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CACMNetDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CACMNetDlg::OnPaint()
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
HCURSOR CACMNetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CACMNetDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//AfxMessageBox(" 是否退出软件 \n", MB_OKCANCEL, MB_ICONQUESTION);
	if (AfxMessageBox(_T(" 是否退出软件 \n"), MB_OKCANCEL, MB_ICONQUESTION) == IDOK){
		CRect rectDlg;
		this->GetWindowRect(rectDlg);
		while (rectDlg.TopLeft().y < rectDlg.BottomRight().y){//rectDlg.Height() > 30
			//rectDlg.Height() -= 10;
			rectDlg.TopLeft().y += 3;
			rectDlg.BottomRight().y -= 3;
			this->MoveWindow(rectDlg);
		}
		CDialogEx::OnClose();
	}
	return;
	
}


void CACMNetDlg::OnBnClickedSend()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR  CMD[1024];
	int index;
	index=mycmd.GetSel();
	mycmd.GetLine(index, CMD);
	GetDlgItem(IDC_COMMAND)->GetWindowText(CMD,1024);
	GetDlgItem(IDC_EDIT1)->SetWindowText(CMD);
}


void CACMNetDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_COMMAND)->SetWindowText(_T(""));
}


void CACMNetDlg::OnSetting()
{
	// TODO:  在此添加命令处理程序代码
	NetSet  Dlg;
	Dlg.DoModal();
}


void CACMNetDlg::OnShowset()
{
	// TODO:  在此添加命令处理程序代码
	
}
