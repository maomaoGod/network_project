
// MainFrm.cpp : CMainFrame 类的实现 , 实现传输层,网络层,链路层的共用。
//
#include "stdafx.h"
#include "NetProtocol.h"
#include "MainFrm.h"
#include "MainFrmTransTools.h"
#include "Tools.h"

typedef unsigned char Byte;
typedef unsigned short Ushort;
typedef int Bool;
#define MAXSIZE 1480

int flag = 0; ///< 标志是TCP或UDP

/**
*@class <_iphdr>
*@brief 存放IP首部的结构体
*@author ACM2012
*@note
*自己定义的IP报文首部
*/
typedef struct _iphdr //定义IP首部 
{
	unsigned short ih_version;  ///< 标识了数据包的IP版本号,一共4位, 0100表示IPv4, 0110表示IPv6
	unsigned short ih_len;	    ///< 16位数据报长度
	unsigned short ih_ident;	///< 数据报的16位标识
	unsigned short ih_flags;    ///< 数据报的标志
	unsigned short ih_offset;	///< 数据报的片偏移
	unsigned short ih_TTL;		///< 数据报的寿命
	unsigned short ih_protl;	///< 数据报的协议
	unsigned short ih_sum;		///< 16位首部检验和
	unsigned int ih_saddr;		///< 32位源IP
	unsigned int ih_daddr;		///< 32位目的IP
} IP_HEADER;

/**
*@class <_udphdr>
*@brief 存放UDP首部的结构体
*@author ACM1201
*@note
*自己定义的UDP首部的结构
*/
typedef struct _udphdr //定义UDP首部 
{
	unsigned short uh_saddr;	///<16位源IP
	unsigned short uh_daddr;	///<16位目的IP
	unsigned short uh_sport;	///<16位源端口
	unsigned short uh_dport;	///<16位目的端口
	unsigned short uh_len;		///<16位长度
	unsigned short uh_sum;		///<16位校验和
} UDP_HEADER;

/**
*@class <_tcphdr>
*@brief 存放TCP首部的结构体
*@author ACM1201
*@note
*自己定义的TCP首部的结构
*/
typedef struct _tcphdr //定义TCP首部 
{
	unsigned short th_saddr;	///<16位源IP
	unsigned short th_daddr;	///<16位目的IP
	unsigned short th_sport;	///<16位源端口
	unsigned short th_dport;	///<16位目的端口
	unsigned short th_len;		///<16位长度
	unsigned short th_sum;		///<16位校验和
	unsigned short th_num;      ///<序号
	unsigned short th_ack;		///<确认号
} TCP_HEADER;

struct UDP_Msg{
	UDP_HEADER *udphdr;				///<udp头部
	char *data;					///<数据
};
struct UDP_Msg MyUDP;

struct TCP_Msg{
	TCP_HEADER *tcphdr;				///<tcp头部
	char *data;					///<数据
};
struct TCP_Msg MyTCP;

struct IP_Msg{
	IP_HEADER *iphdr;			///<指向ip首部的指针
	unsigned short ih_sport;	///<16位源端口
	unsigned short ih_dport;	///<16位目的端口
	unsigned short ih_num;      ///<序号
	unsigned short ih_ack;		///<确认号
	char *data;					///<数据
};
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
	ON_MESSAGE(APPTOTRANS, OnApp2Trans)
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
           LPCTSTR pszText =    (LPCTSTR)(pCopyDataStruct->lpData);
           DWORD   dwLength = (DWORD)(pCopyDataStruct->cbData);
           CString mystr;
           memcpy(mystr.GetBuffer(dwLength/sizeof(TCHAR)), pszText, dwLength);
           mystr.ReleaseBuffer();
		   SendMessage(APPTOTRANS, (WPARAM)&mystr,(LPARAM)&pwnd2port[pWnd]);
		   //HWND swnd = ::FindWindow(NULL,_T("华中科技大学网络实验平台"));
           //::SendMessage(port2hwnd[pwnd2port[pWnd]], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
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
//传输层完成该函数，函数return前最后一句为
LRESULT CMainFrame::OnApp2Trans(WPARAM wparam, LPARAM lparam) //传输层解包传输数据到应用层的接口
{   //使用sendmessage向应用程序发送消息
	//example 向端口号为0的应用程序发送pCopyDataStruct数据  ::SendMessage(port2hwnd[port], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//应用层发往传输层的数据在OnCopyData中获取
	SendMessage(TRANSTOIP, wparam, lparam);
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
	if (MyIP.iphdr->ih_protl)
	{
		int offset = 0, ident = 0;
		ident++;
		MyTCP.tcphdr->th_saddr = MyIP.iphdr->ih_saddr;
		MyTCP.tcphdr->th_daddr = MyIP.iphdr->ih_daddr;
		MyTCP.tcphdr->th_len = MyIP.iphdr->ih_len;
		MyTCP.tcphdr->th_sport = MyIP.ih_sport;
		MyTCP.tcphdr->th_dport = MyIP.ih_dport;
		MyTCP.tcphdr->th_sum = MyIP.iphdr->ih_sum;
		MyTCP.tcphdr->th_num = MyIP.ih_num;
		MyTCP.tcphdr->th_ack = MyIP.ih_ack;
		while (MyTCP.tcphdr->th_len - offset > 0)
		{
			if (MyIP.iphdr->ih_ident == ident)
			{
				if (MyIP.iphdr->ih_offset == offset)
				{
					if (MyIP.iphdr->ih_flags){
						strncpy(MyTCP.data + offset, MyIP.data, MAXSIZE);
						offset = offset + MAXSIZE / 8;
					}
					else{
						strncpy(MyTCP.data + offset, MyIP.data, MyTCP.tcphdr->th_len - offset);
						offset = MyTCP.tcphdr->th_len;
					}
				}
			}
		}
		(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOTRANS, wparam, lparam);
	}
	else
	{
		int offset = 0, ident = 0;
		ident++;
		MyUDP.udphdr->uh_saddr = MyIP.iphdr->ih_saddr;
		MyUDP.udphdr->uh_daddr = MyIP.iphdr->ih_daddr;
		MyUDP.udphdr->uh_len = MyIP.iphdr->ih_len;
		MyUDP.udphdr->uh_sport = MyIP.ih_sport;
		MyUDP.udphdr->uh_dport = MyIP.ih_dport;
		MyUDP.udphdr->uh_sum = MyIP.iphdr->ih_sum;
		while (MyUDP.udphdr->uh_len - offset > 0)
		{
			if (MyIP.iphdr->ih_ident == ident)
			{
				if (MyIP.iphdr->ih_offset == offset)
				{
					if (MyIP.iphdr->ih_flags){
						strncpy(MyTCP.data + offset, MyIP.data, MAXSIZE);
						offset = offset + MAXSIZE / 8;
					}
					else{
						strncpy(MyTCP.data + offset, MyIP.data, MyUDP.udphdr->uh_len - offset);
						offset = MyUDP.udphdr->uh_len;
					}
				}
			}
		}
		(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOTRANS, wparam, lparam);
	}
	return 0;
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
	unsigned int dst_ip = IP_string2uint(Tstr::CS2S(mystr.Mid(0, split_ip)));
	unsigned int dst_port = port_string2uint(Tstr::CS2S(mystr.Mid(split_ip+1, split_port-split_ip-1)));
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
		new_udp_msg.udp_checksum = udpmakesum((u16)data_len, (u16)src_port, (u16)dst_port, data_len%2, (u16 *)&temp_data);

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
	if (flag)
	{
		int offset = 0, ident = 0;
		ident++;
		while (MyTCP.tcphdr->th_len - offset > MAXSIZE)
		{
			MyIP.iphdr->ih_saddr = MyTCP.tcphdr->th_saddr;
			MyIP.iphdr->ih_daddr = MyTCP.tcphdr->th_daddr;
			MyIP.iphdr->ih_flags = 1;
			MyIP.iphdr->ih_ident = ident;
			MyIP.iphdr->ih_len = MyTCP.tcphdr->th_len;
			MyIP.iphdr->ih_offset = offset;
			offset = offset + MAXSIZE / 8;
			MyIP.iphdr->ih_protl = 1;
			MyIP.iphdr->ih_sum = MyTCP.tcphdr->th_sum;
			MyIP.iphdr->ih_TTL = MAXSIZE;
			MyIP.iphdr->ih_version = 4;
			MyIP.ih_sport = MyTCP.tcphdr->th_sport;
			MyIP.ih_dport = MyTCP.tcphdr->th_dport;
			MyIP.ih_num = MyTCP.tcphdr->th_num;
			MyIP.ih_ack = MyTCP.tcphdr->th_ack;
			strncpy(MyIP.data, MyTCP.data, MAXSIZE);
			(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOLINK, wparam, lparam);
		}
		MyIP.iphdr->ih_saddr = MyTCP.tcphdr->th_saddr;
		MyIP.iphdr->ih_daddr = MyTCP.tcphdr->th_daddr;
		MyIP.iphdr->ih_flags = 0;
		MyIP.iphdr->ih_ident = ident;
		MyIP.iphdr->ih_len = MyTCP.tcphdr->th_len;
		MyIP.iphdr->ih_offset = offset;
		MyIP.iphdr->ih_protl = 1;
		MyIP.iphdr->ih_sum = MyTCP.tcphdr->th_sum;
		MyIP.iphdr->ih_TTL = MAXSIZE;
		MyIP.iphdr->ih_version = 4;
		MyIP.ih_sport = MyTCP.tcphdr->th_sport;
		MyIP.ih_dport = MyTCP.tcphdr->th_dport;
		MyIP.ih_num = MyTCP.tcphdr->th_num;
		MyIP.ih_ack = MyTCP.tcphdr->th_ack;
		strncpy(MyIP.data, MyTCP.data, MyTCP.tcphdr->th_len - offset * 8);
		(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOLINK, wparam, lparam);
	}
	else
	{
		int offset = 0, ident = 0;
		ident++;
		while (MyUDP.udphdr->uh_len - offset > MAXSIZE)
		{
			MyIP.iphdr->ih_saddr = MyUDP.udphdr->uh_saddr;
			MyIP.iphdr->ih_daddr = MyUDP.udphdr->uh_daddr;
			MyIP.iphdr->ih_flags = 1;
			MyIP.iphdr->ih_ident = ident;
			MyIP.iphdr->ih_len = MyUDP.udphdr->uh_len;
			MyIP.iphdr->ih_offset = offset;
			offset = offset + MAXSIZE / 8;
			MyIP.iphdr->ih_protl = 0;
			MyIP.iphdr->ih_sum = MyUDP.udphdr->uh_sum;
			MyIP.iphdr->ih_TTL = MAXSIZE;
			MyIP.iphdr->ih_version = 4;
			MyIP.ih_sport = MyUDP.udphdr->uh_sport;
			MyIP.ih_dport = MyUDP.udphdr->uh_dport;
			strncpy(MyIP.data, MyTCP.data, MAXSIZE);
			(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOLINK, wparam, lparam);
		}
		MyIP.iphdr->ih_saddr = MyUDP.udphdr->uh_saddr;
		MyIP.iphdr->ih_daddr = MyUDP.udphdr->uh_daddr;
		MyIP.iphdr->ih_flags = 0;
		MyIP.iphdr->ih_ident = ident;
		MyIP.iphdr->ih_len = MyUDP.udphdr->uh_len;
		MyIP.iphdr->ih_offset = offset;
		MyIP.iphdr->ih_protl = 0;
		MyIP.iphdr->ih_sum = MyUDP.udphdr->uh_sum;
		MyIP.iphdr->ih_TTL = MAXSIZE;
		MyIP.iphdr->ih_version = 4;
		MyIP.ih_sport = MyUDP.udphdr->uh_sport;
		MyIP.ih_dport = MyUDP.udphdr->uh_dport;
		strncpy(MyIP.data, MyTCP.data, MyUDP.udphdr->uh_len - offset * 8);
		(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOLINK, wparam, lparam);
	}
	return 0;
}

LRESULT CMainFrame::OnLinkSend(WPARAM wparam, LPARAM lparam) //链路层打包数据发送出去接口
{//
	return 0;
}






