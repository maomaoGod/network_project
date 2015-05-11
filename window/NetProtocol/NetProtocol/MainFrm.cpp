// MainFrm.cpp : CMainFrame 类的实现 , 实现传输层,网络层,链路层的共用。
//
#include "stdafx.h"
#include "NetProtocol.h"
#include "MainFrm.h"
#include "CMyIP.h"
#include "string.h"
#include "Tools.h"
#include "pcap.h"


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
	Serve = _T("网络实验服务器");
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
		SendMessage(APPTOTRANS, (WPARAM)pCopyDataStruct, (LPARAM)pWnd);
	}
  	 return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

LRESULT CMainFrame::OnCheck(WPARAM wparam, LPARAM lparam)
{
	HWND mywnd = *((HWND *)wparam);
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

LRESULT CMainFrame::OnApp2Trans(WPARAM wparam, LPARAM lparam)
{
	sockstruct mysock = *(sockstruct *)(((COPYDATASTRUCT *)wparam)->lpData);
	int FuncID = ((COPYDATASTRUCT *)wparam)->dwData;
	CWnd *pWnd = (CWnd *)lparam;
	HWND temp = port2hwnd[pwnd2port[pWnd]];
	COPYDATASTRUCT  mycp;
	switch (FuncID)
	{
	case SOCKBIND:   //暂时不考虑端口冲突
		port2hwnd.erase(pwnd2port[pWnd]);
		pwnd2port.erase(pWnd);
		pwnd2port[pWnd] = mysock.bindport;
		port2hwnd[mysock.bindport] = temp;
		PrintView(_T("绑定端口到6500!"));
		break;
	case SOCKLISTEN:
		break;
	case SOCKSEND:
		mysock.srcport = pwnd2port[pWnd];
		mycp.dwData = SOCKSEND;
		mycp.lpData = (void *)&mysock;
		mycp.cbData = sizeof(sockstruct);
		::SendMessage(port2hwnd[mysock.dstport], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&mycp);
		break;
	case SOCKSENDTO:
		break;
	case SOCKCONNECT:
		break;
	default:
		break;
	}
	return 0;
}
LRESULT CMainFrame::OnTrans2App(WPARAM wparam, LPARAM lparam) //传输层解包传输数据到应用层的接口
{  //使用sendmessage向应用程序发送消息
	//example 向端口号为0的应用程序发送pCopyDataStruct数据  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//应用层发往传输层的数据在OnCopyData中获取
	struct Msg new_ip_msg = *((struct Msg *)wparam);

	// 由于UDP和TCP的开头都是源端口，直接假s定其为UDP来获取源端口号
	struct udp_message assume_udp_msg;
	memcpy(&assume_udp_msg, new_ip_msg.data, strlen(new_ip_msg.data)+1); // +1 for \0
	struct tcplist *found_TCP = getNode(new_ip_msg.sip, assume_udp_msg.udp_src_port);

	// UDP
	if (found_TCP == NULL)
	{
		// 获取UDP报文段
		struct udp_message new_udp_msg;
		memcpy(&new_udp_msg, new_ip_msg.data, strlen(new_ip_msg.data)+1); // +1 for \0

		// 检验和
		if (!udpcheck(new_udp_msg.udp_msg_length-8, new_udp_msg.udp_src_port, new_udp_msg.udp_dst_port, new_udp_msg.udp_msg_length%2, (u16 *)&(new_udp_msg.udp_app_data), new_udp_msg.udp_checksum))
		{
			// 舍弃报文
			return -1;
		}

		// 填入送往应用层的结构中
		struct sockstruct new_sockstruct;
		new_sockstruct.dstport = new_udp_msg.udp_dst_port;
		new_sockstruct.srcport = new_udp_msg.udp_src_port;
		new_sockstruct.bindport = 0;
		new_sockstruct.datalength = new_udp_msg.udp_msg_length-8;
		IP_uint2chars(new_sockstruct.srcip, new_ip_msg.sip);
		IP_uint2chars(new_sockstruct.dstip, new_ip_msg.dip);
		memcpy(new_sockstruct.data, new_udp_msg.udp_app_data, new_sockstruct.datalength+1); // +1 for \0
		
		COPYDATASTRUCT CopyDataStruct;
		// 字节数
		CopyDataStruct.cbData = sizeof(new_sockstruct);
		// 发送内容
		CopyDataStruct.lpData = &new_sockstruct;
		// 设置为Send，应用层套接字Receive响应
		CopyDataStruct.dwData = SOCKSEND;
		// 进程间通信
		::SendMessage(port2hwnd[new_udp_msg.udp_dst_port], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&CopyDataStruct);
	}
	// TCP
	else
	{
		// 获取TCP报文段
		struct tcp_message new_tcp_msg;
		memcpy(&new_tcp_msg, new_ip_msg.data, strlen(new_ip_msg.data)+1); // +1 for \0

		// opts和data一同进行检验
		unsigned data_len = strlen(new_tcp_msg.tcp_opts_and_app_data)+1; // +1 for \0

		// 检验和
		if (!udpcheck(data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, data_len%2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
		{
			// 舍弃报文
			return -1;
		}

		TCP_receive();

		// 填入送往应用层的结构中
		struct sockstruct new_sockstruct;
		new_sockstruct.dstport = new_tcp_msg.tcp_dst_port;
		new_sockstruct.srcport = new_tcp_msg.tcp_src_port;
		new_sockstruct.bindport = 0;
		new_sockstruct.datalength = data_len;
		IP_uint2chars(new_sockstruct.srcip, new_ip_msg.sip);
		IP_uint2chars(new_sockstruct.dstip, new_ip_msg.dip);
		memcpy(new_sockstruct.data, new_tcp_msg.tcp_opts_and_app_data, new_sockstruct.datalength+1); // +1 for \0

		COPYDATASTRUCT CopyDataStruct;
		// 字节数
		CopyDataStruct.cbData = sizeof(new_sockstruct);
		// 发送内容，暂时不分开opts和data
		CopyDataStruct.lpData = &new_sockstruct;
		// 设置为Send，应用层套接字Receive响应
		CopyDataStruct.dwData = SOCKSEND;
		// 进程间通信
		::SendMessage(port2hwnd[new_tcp_msg.tcp_dst_port], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&CopyDataStruct);
	}

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
	my_linker &receiver = (my_linker)(*(my_linker *)lparam);
	const u_char * packetData = (const u_char *)wparam;
	IP_Msg * ip_msg;
	ip_msg = receiver.combine(packetData);
	if (ip_msg != NULL) AfxGetMainWnd()->SendMessage(IPTOTRANS, (WPARAM)ip_msg);
	return 0;
}

struct tcp_message global_new_tcp_msg;

LRESULT CMainFrame::OnTrans2IP(WPARAM wparam, LPARAM lparam) //传输层打包数据发送到网络层的接口
{ //使用sendmessage向应用程序发送消息
	//example 向端口号为0的应用程序发送pCopyDataStruct数据  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//应用层发往传输层的数据在OnCopyData中获取
	struct sockstruct data_from_applayer = *((struct sockstruct *)wparam);
	unsigned int dst_ip = IP_chars2uint(data_from_applayer.dstip);
	unsigned short dst_port = data_from_applayer.dstport;
	unsigned short src_port = data_from_applayer.srcport;
	unsigned int src_ip = getIP();
	unsigned int data_len = data_from_applayer.datalength;

	// 判断是UDP还是TCP
	struct tcplist *found_TCP = getNode(dst_ip, dst_port);

	// UDP
	if (found_TCP == NULL)
	{
		struct udp_message new_udp_msg;
		// 填入UDP报文段结构
		new_udp_msg.udp_src_port = src_port;
		new_udp_msg.udp_dst_port = dst_port;
		new_udp_msg.udp_msg_length = 8+data_len;
		memcpy(new_udp_msg.udp_app_data, data_from_applayer.data, data_len+1); // +1 for \0
		new_udp_msg.udp_checksum = udpmakesum((u16)data_len, (u16)src_port, (u16)dst_port, data_len%2, (u16 *)&(new_udp_msg.udp_app_data));

		// UDP无拥塞控制
		struct Msg new_ip_msg;
		new_ip_msg.sip = src_ip;
		new_ip_msg.dip = dst_ip;
		new_ip_msg.datelen = new_udp_msg.udp_msg_length;
		memcpy(new_ip_msg.data, &new_udp_msg, new_udp_msg.udp_msg_length+1); // +1 for \0
		OnIP2Link((WPARAM)&new_ip_msg, lparam);
	}
	// TCP
	else
	{
		struct tcp_message new_tcp_msg;
		// 目前的TCP实现结构有问题，感觉面对每一个连接，需要有一个线程跑TCP负责应答ACK，调整窗口
		// 当TCP连接断开时线程消逝
		// 或者只开一个线程，用于TCP总控，负责维护TCP状态链表，并应答ACK等
		// 然后连接时创建TCP连接到TCP链表，断开时从链表剥离
		// 可以考虑进程间发消息，也可以考虑直接用全局变量做标志位
		new_tcp_msg.tcp_src_port = src_port;
		new_tcp_msg.tcp_dst_port = dst_port;

		// 获取Function ID
		int funID = (int)lparam;

		// 方法判断
		if (funID == SOCKCONNECT)
		{
			// 三次握手
			TCP_new();
			TCP_send();
			for (;;)
			{
				// wait for ack
			}
			TCP_send();
		}
		else if (funID == SOCKSEND)
		{
			TCP_send();
		}
		else if (funID == SOCKCLOSE)
		{
			TCP_send();
			for (;;)
			{
				// wait for ack
			}
			for (;;)
			{
				// wait for FIN
			}
			TCP_send();
			TCP_destroy;
		}
		else
		{
			printf("OnTrans2IP: What is this?\n");
		}
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
	static int seq = 0;
	while ((linker.send_by_frame((struct IP_Msg *)wparam, linker.get_adapter(), seq)) != 0)
		seq += 1;
	return 0;
}

DWORD WINAPI CMainFrame::packcap(LPVOID lParam)
{
	my_linker *receiver = new my_linker();
	pcap_t * adapterHandle = receiver->get_adapter();
	struct pcap_pkthdr * packetHeader;
	const u_char       * packetData;
	int retValue;
	receiver->initialize();

	while ((retValue = pcap_next_ex(adapterHandle,

		&packetHeader,

		&packetData)) >= 0)

	{
		if (retValue == 0) continue;
		if (packetHeader->len != 168) continue;
		AfxGetMainWnd()->SendMessage(LINKTOIP, (WPARAM)packetData, (LPARAM)receiver);
	}
	return 0;
}

