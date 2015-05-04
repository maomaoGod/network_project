// MainFrm.cpp : CMainFrame 类的实现 , 实现传输层,网络层,链路层的共用。
//
#include "stdafx.h"
#include "NetProtocol.h"
#include "MainFrm.h"
#include "CMyIP.h"
#include "string.h"
#include "Tools.h"

struct _iphdr IP_HEADER;
struct Msg IP_data;
struct IP_Msg MyIP;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define SERVE 1000
#define CLIENT 2000
extern void PrintView(CString e);
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_COPYDATA()
	ON_MESSAGE(CHECKHWND, OnCheck)
	ON_MESSAGE(TRANSTOAPP, OnTrans2App)
	ON_MESSAGE(IPTOTRANS, OnIP2Trans)
	ON_MESSAGE(LINKTOIP, OnLink2IP)
	ON_MESSAGE(TRANSTOIP, OnTrans2IP)
	ON_MESSAGE(IPTOLINK, OnIP2Link)
	ON_MESSAGE(LINKSEND, OnLinkSend)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO:  在此添加成员初始化代码
	if (!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)packcap, (LPVOID) this, NULL, NULL))
		AfxMessageBox(_T("创建抓包线程失败！"));
	numprocess = 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// TODO:  如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

BOOL CALLBACK lpEnumHwnd(HWND hwnd, LPARAM lParam)//遍历所有窗口，寻找客户端和服务器程序
{
	CString Client, Serve;
	Client = _T("华中科技大学网络实验平台");
	Serve = _T("NetServe");
	TCHAR str[100];
	::GetWindowText(hwnd, str, 100);
	if (Client.Compare(str) == 0 || Serve.Compare(str) == 0)
		(AfxGetApp()->m_pMainWnd)->SendMessage(CHECKHWND, (WPARAM)&hwnd, lParam);
	return 1;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (pwnd2port.find(pWnd) == pwnd2port.end()){  //应用程序注册
		EnumWindows(lpEnumHwnd, (LPARAM)pWnd);
		return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
	}
	if (pCopyDataStruct != NULL){//接受来自应用程序的消息
		LPCTSTR pszText = (LPCTSTR)(pCopyDataStruct->lpData);
		DWORD   dwLength = (DWORD)(pCopyDataStruct->cbData);
		CString mystr;
		memcpy(mystr.GetBuffer(dwLength / sizeof(TCHAR)), pszText, dwLength);
		mystr.ReleaseBuffer();
		PrintView(mystr);
		HWND swnd = ::FindWindow(NULL, _T("华中科技大学网络实验平台"));
		::SendMessage(port2hwnd[pwnd2port[pWnd]], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	}
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

LRESULT CMainFrame::OnCheck(WPARAM wparam, LPARAM lparam)
{
	HWND mywnd = *((HWND *)wparam);
	PPwnd = ::FindWindow(NULL, _T("华中科技大学网络实验平台"));
	int index;
	TCHAR str[100];
	::GetWindowText(mywnd, str, 100);
	for (index = 0; index < numprocess; index++)
	if (port2hwnd[index] == mywnd)
		break;
	if (index == numprocess&&pwnd2port.find((CWnd *)lparam) == pwnd2port.end()){ //一个窗口只能注册一次
		pwnd2port[(CWnd *)lparam] = numprocess;
		port2hwnd[numprocess] = mywnd;
		numprocess = numprocess + 1;
	}
	return 0;
}

LRESULT CMainFrame::OnTrans2App(WPARAM wparam, LPARAM lparam) //传输层解包传输数据到应用层的接口
{ //使用sendmessage向应用程序发送消息
	//example 向端口号为0的应用程序发送pCopyDataStruct数据  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//应用层发往传输层的数据在OnCopyData中获取
	//CString mystr = *((CString *)wparam);
	//int port = *((int *)lparam);



	// UDP
	if (true/* edited later */)
	{
		
	}

	// TCP

	return 0;
}

LRESULT CMainFrame::OnIP2Trans(WPARAM wparam, LPARAM lparam) //网络层解包传输到传输层的接口
{ //
	///< 根据链路层发送的数据进行剥离得到报文长度以及偏移, 比较偏移量是否等于报文长度
	///< 若发现分片缺失或者检验和出错则 return FALSE;
	///< 若是则数据成功接收 进行少量的检验和检查, 若没有错误
	///< 则将IP_msg结构剥离出Msg结构

	if (!ip.IP2Trans(wparam, lparam))
		return true;
	return false;
}

LRESULT CMainFrame::OnLink2IP(WPARAM wparam, LPARAM lparam) //链路层解包传输数据网络层的接口
{//
	return 0;
}

LRESULT CMainFrame::OnTrans2IP(WPARAM wparam, LPARAM lparam) //传输层打包数据发送到网络层的接口
{ //使用sendmessage向应用程序发送消息
	//example 向端口号为0的应用程序发送pCopyDataStruct数据  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//应用层发往传输层的数据在OnCopyData中获取
	CString mystr = *((CString *)wparam);
	unsigned int src_port = *((unsigned int *)lparam);
	
	// 切分传递的数据，格式为“IP+目的端口号+DATA”
	int split_ip = mystr.Find(_T(':'), 0);
	int split_port = mystr.Find(_T(' '), split_ip);
	using namespace Tools;
	unsigned int dst_ip = IP_string2uint(Tstr::CS2S(mystr.Mid(0, 
split_ip)));
	unsigned int dst_port = port_string2uint(Tstr::CS2S(mystr.Mid(split_ip
+1, split_port-split_ip-1)));
	unsigned int src_ip = getIP();

	// 先转化为多字节，再计算长度，以免计算少了字节数
	// 这里可能需要日后更改为应用层自己转码，这样理论上可以减少传输的字节数
	CString temp_data = mystr.Mid(split_ip);
	unsigned int data_len = CStringA(temp_data).GetLength();

	// UDP
	if (true/* edited later */)
	{
		struct udp_message new_udp_msg;
		// 填入UDP报文段结构
		new_udp_msg.udp_src_port = src_port;
		new_udp_msg.udp_dst_port = dst_port;
		new_udp_msg.udp_msg_length = 8+data_len;
		new_udp_msg.udp_app_data = temp_data;
		new_udp_msg.udp_checksum = udpmakesum((u16)data_len, (u16)
src_port, (u16)dst_port, data_len%2, (u16 *)&temp_data);

		// UDP无拥塞控制
		OnIP2Link((WPARAM)&new_udp_msg, lparam);
	}
	// TCP
	else
	{
		struct tcp_message new_tcp_msg;
	}

	return 0;
}

LRESULT CMainFrame::OnIP2Link(WPARAM wparam, LPARAM lparam) //网络层打包数据发送到链路层接口
{
	///< 将运输层送来的Msg结构和IP地址插入到IP_msg结构中,
	///< 如果信息超过容量就进行分片处理, 
	///< 调用链路层的发送函数如果发送失败 return FALSE;
	///< 否则 return TRUE;
	if (!ip.IP2Link(wparam, lparam))
		return true;
	return false;
}

LRESULT CMainFrame::OnLinkSend(WPARAM wparam, LPARAM lparam) //链路层打包数据发送出去接口
{
	if(send((struct IP_Msg *)wparam,(unsigned short)lparam)!=0)
	{
		printf("error sending datagram!\n");
	}
	return;
}

DWORD WINAPI CMainFrame::packcap(LPVOID lParam)
{
	return 0;

}