// MainFrm.cpp : CMainFrame 类的实现 , 实现传输层,网络层,链路层的共用。
//
#include "stdafx.h"
#include "NetProtocol.h"
#include "MainFrm.h"
#include "CMyIP.h"
#include "string.h"
#include "Tools.h"
#include "pcap.h"
#include "windows.h"
#include "time.h"

int sockcount = 0;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define SERVE 1000
#define CLIENT 2000
extern void PrintView(CString e);
// CMainFrame

int Routing_select = 1;
int end_connect = 1;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
    ON_MESSAGE(APPSEND, OnAppSend)
	ON_MESSAGE(TRANSTOAPP, OnTrans2App)
	ON_MESSAGE(IPTOTRANS, OnIP2Trans)
	ON_MESSAGE(LINKTOIP, OnLink2IP)
	ON_MESSAGE(TRANSTOIP, OnTrans2IP)
	ON_MESSAGE(IPTOLINK, OnIP2Link)
	ON_MESSAGE(LINKSEND, OnLinkSend)
	ON_WM_TIMER()
	ON_COMMAND(ID_CLIENT, &CMainFrame::OnClient)
	ON_COMMAND(ID_ROUTE, &CMainFrame::OnRoute)
	ON_COMMAND(ID_OSPF, &CMainFrame::OnOspf)
	ON_COMMAND(ID_RIP, &CMainFrame::OnRip)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构
DWORD WINAPI NewTcpControlThread(LPVOID)
{
	TCP_controller();
	return 0;
}

CMainFrame::CMainFrame()
{
	connsocknum = 0;
	 CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewTcpControlThread, NULL, NULL, NULL);
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
	SetTimer(1000, 1000, NULL);
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




LRESULT CMainFrame::OnTrans2App(WPARAM wparam, LPARAM lparam) //传输层解包传输数据到应用层的接口
{   //使用sendmessage向应用程序发送消息
	//example 向端口号为0的应用程序发送pCopyDataStruct数据  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//应用层发往传输层的数据在OnCopyData中获取
	struct Msg new_ip_msg = *((struct Msg *)wparam);

	// UDP
	if (new_ip_msg.protocol == PROTOCOL_UDP)
	{
		// 获取UDP报文段
		struct udp_message new_udp_msg;
		memcpy(&new_udp_msg, new_ip_msg.data, new_ip_msg.datelen); // +1 for \0

		// 检验和
		if (!udpcheck(new_udp_msg.udp_msg_length - 8, new_udp_msg.udp_src_port, new_udp_msg.udp_dst_port, new_udp_msg.udp_msg_length % 2, (u16 *)&(new_udp_msg.udp_app_data), new_udp_msg.udp_checksum))
		{
			// 舍弃报文
			//return -1;
		}

		// 填入送往应用层的结构中
		struct sockstruct new_sockstruct;
		new_sockstruct.dstport = new_udp_msg.udp_dst_port;
		new_sockstruct.srcport = new_udp_msg.udp_src_port;
		new_sockstruct.funcID = SOCKSENDTO;
		new_sockstruct.datalength = new_udp_msg.udp_msg_length - 8;
		IP_uint2chars(new_sockstruct.srcip, new_ip_msg.sip);
		IP_uint2chars(new_sockstruct.dstip, new_ip_msg.dip);
		memcpy(new_sockstruct.data, new_udp_msg.udp_app_data, new_sockstruct.datalength);

		// 送往应用层
		SendMessage(APPSEND, (WPARAM)&new_sockstruct);
	}
	// TCP
	else if (new_ip_msg.protocol == PROTOCOL_TCP)
	{
		// 触发接收事件
		TCP_receive(new_ip_msg);
	}
	else
	{
		printf("What is this in OnTrans2App?!\n");
	}

	return 0;
}

LRESULT CMainFrame::OnIP2Trans(WPARAM wparam, LPARAM lparam) //网络层解包传输到传输层的接口
{ //
	///< 根据链路层发送的数据进行剥离得到报文长度以及偏移, 比较偏移量是否等于报文长度
	///< 若发现分片缺失或者检验和出错则 return FALSE;
	///< 若是则数据成功接收 进行少量的检验和检查, 若没有错误
	///< 则将IP_msg结构剥离出Msg结构

	ip_message *Routest = (struct ip_message*)wparam;
	if (Routest->ih_ident == data_info)
	{
		if (!IP.IP2Trans(wparam, lparam))
			return true;
		else
		{
			AfxMessageBox(_T("网络层传输给传输层数据包出现问题.\n"));
			return false;
		}
	}
	else
	{
		if (!IP.RecvMsg(wparam, lparam))
			return true;
		else
		{
			AfxMessageBox(_T("网络层接收路由信息出现问题.\n"));
			return false;
		}
	}

}

LRESULT CMainFrame::OnLink2IP(WPARAM wparam, LPARAM lparam) //链路层解包传输数据网络层的接口
{
	linker.Link2IP(wparam);
	return 0;
}

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

	IP_uint2chars(data_from_applayer.srcip, src_ip);
	// 转换IP
	if (dst_ip == IP_chars2uint("127.0.0.1"))
	{
		dst_ip = src_ip;
		memcpy(data_from_applayer.dstip, data_from_applayer.srcip, 20);
	}

	// 获取Function ID
	int funcID = data_from_applayer.funcID;

	// 判断是UDP还是TCP
	// UDP
	if (funcID == SOCKSENDTO)
	{
		// 直接发送咯
		UDP_Send2IP(data_from_applayer, src_ip, dst_ip, data_len);
	}
	// TCP
	else
	{
		// 方法判断
		if (funcID == SOCKCONNECT)
		{
			// 隐式的三次握手，并不是阻塞的过程

			// 新建TCP连接，初始化TCP连接链表
			TCP_new(src_ip, src_port, dst_ip, dst_port, LINK_CONNECTING);
		}
		else if (funcID == SOCKSEND)
		{
			// 不需拆分来自应用层的过大数据段
			TCP_send(data_from_applayer);
		}
		else if (funcID == SOCKCLOSE)
		{
			// 主动断开连接，成为半开状态，可以接收数据但是不发送
			TCP_close(src_ip, src_port, dst_ip, dst_port);
		}
		else if (funcID == SOCKLISTEN)
		{
			// 监听端口
			port_listen(src_port);
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
	clock_t t1 = clock(), t2;
	if (end_connect == 1)
	{
		if (!IP.IP2Link(wparam, lparam))
			return true;
		else
		{
			AfxMessageBox(_T("网络层传输给链路数据包出现问题.\n"));
			return false;
		}
	}
	else
	{
		ip_message *Routest = new ip_message;
		Routest = (struct ip_message *)wparam;

		if (Routest->ih_ident == data_info)                             
		{
			if (!IP.IP2Link(wparam, lparam))
				return true;
			else
			{
				AfxMessageBox(_T("网络层传输给链路数据包出现问题.\n"));
				return false;
			}
		}
		else{
			if (!IP.SendMsg(wparam, lparam))
				return true;
			else
			{
				AfxMessageBox(_T("网络层发送路由信息出现问题.\n"));
				return false;
			}
		}
	}

	if (t1 - t2 ==15)
	{
		if (!IP.SendMsg(wparam, lparam))
			return true;
		else
		{
			AfxMessageBox(_T("网络层发送路由信息出现问题.\n"));
			return false;
		}
	}
	t2 = t1;
	///< 如果在路由里面就应该每隔15s发送一次路由信息
}


LRESULT CMainFrame::OnLinkSend(WPARAM wparam, LPARAM lparam) //链路层打包数据发送出去接口
{
	static int seq = 0;
	struct IP_Msg *datagram = (struct IP_Msg *)wparam;
	unsigned int Src_IP = datagram->ih_saddr;
	unsigned int Des_IP = datagram->ih_daddr;
	unsigned short len = datagram->ih_len;
	if (Des_IP != getIP())
	{
		while (true)
		{
			linker.send_broadcast(linker.adapterHandle, Src_IP, Des_IP);
			if (linker.get_mac(linker.adapterHandle)) break;
			Sleep(200);
		}
	}
	else
	{
		for (int i = 0; i < 3; ++i) linker.mac_des[i] = linker.mac_src[i];
	}
	//}
	//for (int i = 0; i < 3; ++i) linker.mac_des[i] = linker.mac_src[i];
	if ((linker.send_by_frame(datagram, linker.adapterHandle, seq, len)) == 0)
	{
		seq += 1;
		seq = seq % 100;
	}
	//告诉网络层发送失败
	return 0;
}


LRESULT CMainFrame::OnAppSend(WPARAM wparam, LPARAM lparam)
{
	m_sockpool.SendToApp((void *)wparam);
	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (connsocknum != m_sockpool.sockconnum){
		connsocknum = m_sockpool.sockconnum;
		GetActiveView()->SendMessage(SOCKSTATEUPDATE, connsocknum);
	}
	CFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnClient()
{
	// TODO:  在此添加命令处理程序代码
	end_connect = 1;
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_CLIENT, MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_ROUTE, MF_BYCOMMAND | MF_UNCHECKED);
}


void CMainFrame::OnRoute()
{
	// TODO:  在此添加命令处理程序代码
	end_connect = 0;
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_ROUTE, MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_CLIENT, MF_BYCOMMAND | MF_UNCHECKED);
}


void CMainFrame::OnOspf()
{
	// TODO:  在此添加命令处理程序代码
	Routing_select = 1;
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_OSPF, MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_RIP, MF_BYCOMMAND | MF_UNCHECKED);
}


void CMainFrame::OnRip()
{
	// TODO:  在此添加命令处理程序代码
	Routing_select = 0;
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_RIP, MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_OSPF, MF_BYCOMMAND | MF_UNCHECKED);
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO:  在此添加专用代码和/或调用基类
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_CLIENT, MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_OSPF, MF_BYCOMMAND | MF_CHECKED);
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
