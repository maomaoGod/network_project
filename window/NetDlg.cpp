// NetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CSnet.h"
#include "NetDlg.h"
#include "afxdialogex.h"


// NetDlg 对话框

IMPLEMENT_DYNAMIC(NetDlg, CDialogEx)

NetDlg::NetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(NetDlg::IDD, pParent)
{
	status = 0;

}

NetDlg::~NetDlg()
{
}

void NetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mylist);
	DDX_Control(pDX, IDC_SPERETOR, mysparator);
}


BEGIN_MESSAGE_MAP(NetDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &NetDlg::OnSelchangeList1)
//	ON_WM_CTLCOLOR()
ON_WM_CTLCOLOR()
ON_WM_SIZE()
END_MESSAGE_MAP()


// NetDlg 消息处理程序



BOOL NetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//48D1CC
	m_hbrush = CreateSolidBrush(RGB(255, 255, 255));
	//DeleteObject(m_hbrush);
	this->GetClientRect(&Rect);
	mysparator.GetWindowRect(&ChildRect);
	ScreenToClient(&ChildRect);
	// TODO:  在此添加额外的初始化
	mylist.AddString(_T("   应用层"));
	mylist.AddString(_T("   运输层"));
	mylist.AddString(_T("   网络层"));
	mylist.AddString(_T("   链路层"));
	mylist.AddString(_T("   物理层"));
	int num = mylist.GetCount();
	Oldsel = 0;
	for (int i = 0; i < num; i++)
		mylist.SetItemHeight(i, 30);
	status = 1;
	myApplayer.Create(IDD_DIALOG1,this);
	myTrslayer.Create(IDD_DIALOG2, this);
	myNetlayer.Create(IDD_DIALOG3, this);
	myLinklayer.Create(IDD_DIALOG4, this);
	myPhylayer.Create(IDD_DIALOG5, this);
	myApplayer.MoveWindow(ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height());
	myTrslayer.MoveWindow(ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height());
	myApplayer.MoveWindow(ChildRect.left + 5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height());
	myTrslayer.MoveWindow(ChildRect.left + 5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height());
	myApplayer.MoveWindow(ChildRect.left + 5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height());
	myApplayer.SetWindowPos(NULL,ChildRect.left+5,ChildRect.top, Rect.right-ChildRect.right,ChildRect.Height(), SWP_SHOWWINDOW);
	myTrslayer.SetWindowPos(NULL, ChildRect.left+5,ChildRect.top, Rect.right-ChildRect.right,ChildRect.Height(), SWP_HIDEWINDOW);
	myNetlayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_HIDEWINDOW);
	myLinklayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_HIDEWINDOW);
	myPhylayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_HIDEWINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void NetDlg::OnSelchangeList1()  //当选择列表改变时切换对应对话框
{
	// TODO:  在此添加控件通知处理程序代码
	int Nowsel= mylist.GetCurSel();
	switch (Oldsel){
	case 0:myApplayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_HIDEWINDOW); break;
	case 1:myTrslayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_HIDEWINDOW); break;
	case 2:myNetlayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_HIDEWINDOW); break;
	case 3:myLinklayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_HIDEWINDOW); break;
	case 4:myPhylayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_HIDEWINDOW); break;
	default:;
	}
	switch (Nowsel){
	case 0:myApplayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_SHOWWINDOW); break;
	case 1:myTrslayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_SHOWWINDOW); break;
	case 2:myNetlayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_SHOWWINDOW); break;
	case 3:myLinklayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_SHOWWINDOW); break;
	case 4:myPhylayer.SetWindowPos(NULL, ChildRect.left+5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height(), SWP_SHOWWINDOW); break;
	default:;
	}
	Oldsel = Nowsel;
}


HBRUSH NetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)//设置列表框颜色
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_LISTBOX)
    {
		pDC->SetBkMode(TRANSPARENT);
		//此处设置字体的颜色   
		pDC->SetTextColor(RGB(0,0,0));
		return   m_hbrush;
	}
// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}



void NetDlg::OnSize(UINT nType, int cx, int cy)  //对话框大小变化时改变对应子框体的大小
{
	CDialogEx::OnSize(nType, cx, cy);
   // TODO:  在此处添加消息处理程序代码
	if (status){
		mysparator.ShowWindow(SW_HIDE);
		this->GetClientRect(&Rect);
		ChildRect.top = Rect.top;
		ChildRect.bottom = Rect.bottom;
		mysparator.MoveWindow(ChildRect.left,ChildRect.top,1,ChildRect.Height());
		mysparator.ShowWindow(SW_SHOW);
		mylist.MoveWindow(Rect.left, Rect.top, ChildRect.left - Rect.left-2, Rect.Height());
		myApplayer.MoveWindow(ChildRect.left + 5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height());
		myTrslayer.MoveWindow(ChildRect.left + 5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height());
		myApplayer.MoveWindow(ChildRect.left + 5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height());
		myTrslayer.MoveWindow(ChildRect.left + 5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height());
		myApplayer.MoveWindow(ChildRect.left + 5, ChildRect.top, Rect.right - ChildRect.right, ChildRect.Height());
	}
}
