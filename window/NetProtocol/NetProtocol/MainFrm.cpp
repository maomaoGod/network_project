// MainFrm.cpp : CMainFrame 类的实现 , 实现传输层,网络层,链路层的共用。
//
#include "stdafx.h"
#include "NetProtocol.h"
#include "MainFrm.h"
#include "CMyIP.h"
#include "string.h"
#include "Tools.h"
#include "pcap.h"

int sockcount = 0;

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
    ON_MESSAGE(APPSEND, OnAppSend)
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




LRESULT CMainFrame::OnTrans2App(WPARAM wparam, LPARAM lparam) //传输层解包传输数据到应用层的接口
{   //使用sendmessage向应用程序发送消息
	//example 向端口号为0的应用程序发送pCopyDataStruct数据  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//应用层发往传输层的数据在OnCopyData中获取
	struct Msg new_ip_msg = *((struct Msg *)wparam);

	//// 由于UDP和TCP的开头都是源端口，直接假s定其为UDP来获取源端口号
	//struct udp_message assume_udp_msg;
	//memcpy(&assume_udp_msg, new_ip_msg.data, strlen(new_ip_msg.data) + 1); // +1 for \0
	//struct tcplist *found_TCP = getNode(new_ip_msg.sip, assume_udp_msg.udp_src_port);

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
		memcpy(new_sockstruct.data, new_udp_msg.udp_app_data, new_sockstruct.datalength); // +1 for \0

		// 送往应用层
		SendMessage(APPSEND, (WPARAM)&new_sockstruct, (LPARAM)SOCKSEND);
	}
	// TCP
	else if (new_ip_msg.protocol == PROTOCOL_TCP)
	{
		//// 获取TCP报文段
		//struct tcp_message new_tcp_msg;
		//memcpy(&new_tcp_msg, new_ip_msg.data, strlen(new_ip_msg.data) + 1); // +1 for \0

		//// opts和data一同进行检验
		//unsigned data_len = strlen(new_tcp_msg.tcp_opts_and_app_data) + 1; // +1 for \0

		//// 检验和
		//if (!udpcheck(data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, data_len % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
		//{
		//	// 舍弃报文
		//	return -1;
		//}

		// 触发接收事件
		TCP_receive(new_ip_msg);

		//// 填入送往应用层的结构中
		//struct sockstruct new_sockstruct;
		//new_sockstruct.dstport = new_tcp_msg.tcp_dst_port;
		//new_sockstruct.srcport = new_tcp_msg.tcp_src_port;
		//new_sockstruct.funcID = SOCKSEND;
		//new_sockstruct.datalength = data_len;
		//IP_uint2chars(new_sockstruct.srcip, new_ip_msg.sip);
		//IP_uint2chars(new_sockstruct.dstip, new_ip_msg.dip);
		//memcpy(new_sockstruct.data, new_tcp_msg.tcp_opts_and_app_data, new_sockstruct.datalength + 1); // +1 for \0

		//// 送往应用层
		//SendMessage(APPSEND, (WPARAM)&new_sockstruct, (LPARAM)SOCKSEND);
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

	if (!ip.IP2Trans(wparam, lparam))
		return true;
	return false;
}

LRESULT CMainFrame::OnLink2IP(WPARAM wparam, LPARAM lparam) //链路层解包传输数据网络层的接口
{
	my_linker &receiver = (*(my_linker *)lparam);
	const u_char * packetData = (const u_char *)wparam;
	IP_Msg * ip_msg;
	ip_msg = receiver.combine(packetData);
	if (ip_msg != NULL)
	{
		AfxGetApp()->m_pMainWnd->SendMessage(IPTOTRANS, (WPARAM)ip_msg);
		delete ip_msg->iphdr;
	}
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

	// 获取Function ID
	int funcID = data_from_applayer.funcID;

	// 判断是UDP还是TCP
	// UDP
	if (funcID == SOCKSENDTO)
	{
		struct udp_message new_udp_msg;
		// 填入UDP报文段结构
		new_udp_msg.udp_src_port = src_port;
		new_udp_msg.udp_dst_port = dst_port;
		new_udp_msg.udp_msg_length = 8 + data_len;
		memcpy(new_udp_msg.udp_app_data, data_from_applayer.data, data_len + 1); // +1 for \0
		new_udp_msg.udp_checksum = udpmakesum((u16)data_len, (u16)src_port, (u16)dst_port, data_len % 2, (u16 *)&(new_udp_msg.udp_app_data));

		// UDP无拥塞控制
		struct Msg new_ip_msg;
		new_ip_msg.sip = src_ip;
		new_ip_msg.dip = dst_ip;
		new_ip_msg.ih_sport = src_port;
		new_ip_msg.ih_dport = dst_port;
		new_ip_msg.datelen = new_udp_msg.udp_msg_length;
		memcpy(new_ip_msg.data, &new_udp_msg, new_ip_msg.datelen);
		new_ip_msg.protocol = PROTOCOL_UDP;	// 17 for UDP

		// 发往网络层
		SendMessage(IPTOLINK, (WPARAM)&new_ip_msg, lparam);
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

			//// 第一次握手，发送SYN
			//// 构造SYN报文段
			//struct tcp_message new_tcp_msg;
			//new_tcp_msg.tcp_src_port = src_port;
			//new_tcp_msg.tcp_dst_port = dst_port;
			//new_tcp_msg.tcp_seq_number = tcp->seq_number;
			//new_tcp_msg.tcp_ack_number = 0;
			//new_tcp_msg.tcp_hdr_length = 20;
			//new_tcp_msg.tcp_reserved = 0;
			//new_tcp_msg.tcp_urg = 0;
			//new_tcp_msg.tcp_ack = 0;
			//new_tcp_msg.tcp_psh = 0;
			//new_tcp_msg.tcp_rst = 0;
			//new_tcp_msg.tcp_syn = 1;
			//new_tcp_msg.tcp_fin = 0;
			//new_tcp_msg.tcp_rcv_window = tcp->rcvd_wind;
			//new_tcp_msg.tcp_urg_ptr = NULL;
			//new_tcp_msg.tcp_opts_and_app_data[0] = 33;	// whatever
			//new_tcp_msg.tcp_checksum = tcpmakesum(1, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, 1, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data));
			//
			//// 不走TCP_send()，不加入TCP报文结构和报文缓冲，因为其中未记录SYN
			//TCP_Send2IP(new_tcp_msg, src_ip, dst_ip, 1);

			//// 等待来自对方的syn和ack
			//int waited_seq = wait_for_handshaking_ack(tcp);

			// 不走TCP_send()，不加入TCP报文结构和报文缓冲，因为其中未记录SYN
			//new_tcp_msg.tcp_syn = 0;
			//TCP_Send2IP(new_tcp_msg, src_ip, dst_ip, 1);
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

			//// 构造FIN报文段
			//struct tcp_message new_tcp_msg;
			//new_tcp_msg.tcp_src_port = src_port;
			//new_tcp_msg.tcp_dst_port = dst_port;
			//new_tcp_msg.tcp_seq_number = found_TCP->seq_number;
			//new_tcp_msg.tcp_ack_number = 0;
			//new_tcp_msg.tcp_hdr_length = 20;
			//new_tcp_msg.tcp_reserved = 0;
			//new_tcp_msg.tcp_urg = 0;
			//new_tcp_msg.tcp_ack = 0;
			//new_tcp_msg.tcp_psh = 0;
			//new_tcp_msg.tcp_rst = 0;
			//new_tcp_msg.tcp_syn = 0;
			//new_tcp_msg.tcp_fin = 1;
			//new_tcp_msg.tcp_rcv_window = found_TCP->rcvd_wind;
			//new_tcp_msg.tcp_urg_ptr = NULL;
			//new_tcp_msg.tcp_opts_and_app_data[0] = 21;	// whatever
			//new_tcp_msg.tcp_checksum = tcpmakesum(1, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, 1, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data));
			//
			//// 不走TCP_send()，不加入TCP报文结构和报文缓冲，因为其中未记录FIN
			//// 也就是说，TCP的连接请求从不重发
			//TCP_Send2IP(new_tcp_msg, src_ip, dst_ip, 1);

			//// 等待来自对方的syn和ack
			//int waited_seq = wait_for_handshaking_ack(found_TCP);

			//// 不走TCP_send()，不加入TCP报文结构和报文缓冲，因为其中未记录FIN
			//new_tcp_msg.tcp_syn = 0;
			//TCP_Send2IP(new_tcp_msg, src_ip, dst_ip, 1);

			//TCP_destroy;
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
	if (!ip.IP2Link(wparam, lparam))
		return true;
	return false;
}


LRESULT CMainFrame::OnLinkSend(WPARAM wparam, LPARAM lparam) //链路层打包数据发送出去接口
{
	static int seq = 0;
	struct IP_Msg *datagram = (struct IP_Msg *)wparam;
	pcap_t *adapter = linker.get_adapter();
	unsigned int Src_IP = (*datagram).iphdr->ih_saddr;
	unsigned int Des_IP = (*datagram).iphdr->ih_daddr;
	if (!linker.transtable(Des_IP))
	{
		linker.send_broadcast(adapter, Src_IP, Des_IP);
		linker.get_mac(adapter);
	}
	if ((linker.send_by_frame(datagram, adapter, seq)) == 0)
	{
		seq += 1;
		seq = seq % 100;
	}
	//告诉网络层发送失败
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
		if (sizeof(Broadcast_frame) > packetHeader->len) continue;

		if (receiver->check(packetData))
		{
			Broadcast_frame r_frame = *((Broadcast_frame *)packetData), s_frame;
			for (int i = 0; i < 3; ++i)
			{
				receiver->mac_des[i] = s_frame.MAC_des[i] = r_frame.MAC_src[i];
				s_frame.MAC_src[i] = receiver->mac_src[i];
			}
			s_frame.type = 0x0806;
			s_frame.IP_dst = r_frame.IP_src;
			s_frame.IP_src = r_frame.IP_dst;
			pcap_sendpacket(adapterHandle,
				(const u_char *)&s_frame,
				sizeof(s_frame));
		}
		else AfxGetApp()->m_pMainWnd->SendMessage(LINKTOIP, (WPARAM)packetData, (LPARAM)receiver);
	}
	return 0;
}

LRESULT CMainFrame::OnAppSend(WPARAM wparam, LPARAM lparam)
{
	m_sockpool.SendToApp((void *)wparam);
	return 0;
}

LRESULT CMainFrame::SockStateUpdate(WPARAM wparam, LPARAM lparam)
{
	GetActiveView()->SendMessage(SOCKSTATEUPDATE, wparam);
	return 0;
}

