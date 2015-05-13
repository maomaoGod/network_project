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
//    ON_WM_COPYDATA()
//ON_MESSAGE(CHECKHWND, OnCheck)
    ON_MESSAGE(APPSEND, OnAppSend)
	ON_MESSAGE(TRANSTOAPP, OnTrans2App)
	ON_MESSAGE(APPTOTRANS, OnApp2Trans)
	ON_MESSAGE(IPTOTRANS, OnIP2Trans)
	ON_MESSAGE(LINKTOIP, OnLink2IP)
	ON_MESSAGE(TRANSTOIP, OnTrans2IP)
	ON_MESSAGE(IPTOLINK, OnIP2Link)
	ON_MESSAGE(LINKSEND, OnLinkSend)
//	ON_COMMAND(ID_32783, &CMainFrame::OnSENDHUST)
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
	psock = NULL;
	if (!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)packcap, (LPVOID) this, NULL, NULL))
		AfxMessageBox(_T("创建抓包线程失败！"));
	if (!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)connect, (LPVOID) this, NULL, NULL))
		AfxMessageBox(_T("创建连接线程失败！"));
	// TODO:  在此添加成员初始化代码
	if (!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)packcap, (LPVOID) this, NULL, NULL))
		AfxMessageBox(_T("抓包线程创建失败"));
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


LRESULT CMainFrame::OnApp2Trans(WPARAM wparam,LPARAM lparam)
{	
	int nPort = wparam;
	int BindPort = Port2Sock[nPort]->Rpro->mysock.bindport;
	UINT FuncID = port2Rstruct[nPort]->FuncID;
	portsrc tempsrc;
	switch (FuncID)
	{
	case SOCKBIND:
		SockMark2Port[Port2Sock[nPort]->Rpro->SockMark] = BindPort;
		Sock2Port[Port2Sock[nPort]] = BindPort;
		if (BindPort != nPort){
			port2Rstruct[BindPort] = port2Rstruct[nPort]; //用绑定的端口号来接收原来的环境
			Port2Sock[BindPort] = Port2Sock[nPort];   
			port2Wstruct[BindPort] = port2Wstruct[nPort];
			port2Rstruct.erase(nPort); //擦出原来的环境
			Port2Sock.erase(nPort);
			port2Wstruct.erase(nPort);
		}
		break;
	case SOCKLISTEN:
		break;
	case SOCKSEND:
	case SOCKSENDTO:
		port2Rstruct[nPort]->mysock.srcport = nPort;
		SendMessage(TRANSTOIP, (WPARAM)&port2Rstruct[nPort]->mysock, (LPARAM)SOCKSEND);
		break;
	case SOCKACCEPT:
		memcpy(tempsrc.srcip, port2Rstruct[nPort]->mysock.dstip, 20);
		tempsrc.srcport = port2Rstruct[nPort]->mysock.dstport;
		tempsrc.dstport = port2Rstruct[nPort]->mysock.srcport;
		src2port[tempsrc] = SockMark2Port[port2Rstruct[nPort]->AcceptSockMark];
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
	memcpy(&assume_udp_msg, new_ip_msg.data, strlen(new_ip_msg.data) + 1); // +1 for \0
	struct tcplist *found_TCP = getNode(new_ip_msg.sip, assume_udp_msg.udp_src_port);

	// UDP
	if (found_TCP == NULL)
	{
		// 获取UDP报文段
		struct udp_message new_udp_msg;
		memcpy(&new_udp_msg, new_ip_msg.data, strlen(new_ip_msg.data) + 1); // +1 for \0

		// 检验和
		if (!udpcheck(new_udp_msg.udp_msg_length - 8, new_udp_msg.udp_src_port, new_udp_msg.udp_dst_port, new_udp_msg.udp_msg_length % 2, (u16 *)&(new_udp_msg.udp_app_data), new_udp_msg.udp_checksum))
		{
			// 舍弃报文
			return -1;
		}

		// 填入送往应用层的结构中
		struct sockstruct new_sockstruct;
		new_sockstruct.dstport = new_udp_msg.udp_dst_port;
		new_sockstruct.srcport = new_udp_msg.udp_src_port;
		new_sockstruct.bindport = 0;
		new_sockstruct.datalength = new_udp_msg.udp_msg_length - 8;
		IP_uint2chars(new_sockstruct.srcip, new_ip_msg.sip);
		IP_uint2chars(new_sockstruct.dstip, new_ip_msg.dip);
		memcpy(new_sockstruct.data, new_udp_msg.udp_app_data, new_sockstruct.datalength + 1); // +1 for \0

		COPYDATASTRUCT CopyDataStruct;
		// 字节数
		CopyDataStruct.cbData = sizeof(new_sockstruct);
		// 发送内容
		CopyDataStruct.lpData = &new_sockstruct;
		// 设置为Send，应用层套接字Receive响应
		CopyDataStruct.dwData = SOCKSEND;
		// 进程间通信
		//::SendMessage(port2hwnd[new_udp_msg.udp_dst_port], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&CopyDataStruct);
	}
	// TCP
	else
	{
		// 获取TCP报文段
		struct tcp_message new_tcp_msg;
		memcpy(&new_tcp_msg, new_ip_msg.data, strlen(new_ip_msg.data) + 1); // +1 for \0

		// opts和data一同进行检验
		unsigned data_len = strlen(new_tcp_msg.tcp_opts_and_app_data) + 1; // +1 for \0

		// 检验和
		if (!udpcheck(data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, data_len % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
		{
			// 舍弃报文
			return -1;
		}

		//TCP_receive();

		// 填入送往应用层的结构中
		struct sockstruct new_sockstruct;
		new_sockstruct.dstport = new_tcp_msg.tcp_dst_port;
		new_sockstruct.srcport = new_tcp_msg.tcp_src_port;
		new_sockstruct.bindport = 0;
		new_sockstruct.datalength = data_len;
		IP_uint2chars(new_sockstruct.srcip, new_ip_msg.sip);
		IP_uint2chars(new_sockstruct.dstip, new_ip_msg.dip);
		memcpy(new_sockstruct.data, new_tcp_msg.tcp_opts_and_app_data, new_sockstruct.datalength + 1); // +1 for \0

		COPYDATASTRUCT CopyDataStruct;
		// 字节数
		CopyDataStruct.cbData = sizeof(new_sockstruct);
		// 发送内容，暂时不分开opts和data
		CopyDataStruct.lpData = &new_sockstruct;
		// 设置为Send，应用层套接字Receive响应
		CopyDataStruct.dwData = SOCKSEND;
		// 进程间通信
		//::SendMessage(port2hwnd[new_tcp_msg.tcp_dst_port], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&CopyDataStruct);
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

//struct tcp_message global_new_tcp_msg;

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
	int funID = (int)lparam;

	// 判断是UDP还是TCP
	struct tcplist *found_TCP = getNode(dst_ip, dst_port);

	// UDP
	if (found_TCP == NULL)
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
		new_ip_msg.datelen = new_udp_msg.udp_msg_length;
		memcpy(new_ip_msg.data, &new_udp_msg, new_ip_msg.datelen);
		new_ip_msg.protocol = 17;	// 17 for UDP
		OnIP2Link((WPARAM)&new_ip_msg, lparam);
	}
	// TCP
	else
	{
		// 下面可能需要放在别的地方
		// 填入TCP报文段结构
		struct tcp_message new_tcp_msg;
		// 目前的TCP实现结构有问题，感觉面对每一个连接，需要有一个线程跑TCP负责应答ACK，调整窗口
		// 当TCP连接断开时线程消逝
		// 或者只开一个线程，用于TCP总控，负责维护TCP状态链表，并应答ACK等
		// 然后连接时创建TCP连接到TCP链表，断开时从链表剥离
		// 可以考虑进程间发消息，也可以考虑直接用全局变量做标志位
		new_tcp_msg.tcp_src_port = src_port;
		new_tcp_msg.tcp_dst_port = dst_port;

		// 方法判断
		if (funID == SOCKCONNECT)
		{
			// 三次握手
			//TCP_new();
			//TCP_send();
			for (;;)
			{
				// wait for ack
			}
			//TCP_send();
		}
		else if (funID == SOCKSEND)
		{
		//	TCP_send();
		}
		else if (funID == SOCKCLOSE)
		{
		//	TCP_send();
			for (;;)
			{
				// wait for ack
			}
			for (;;)
			{
				// wait for FIN
			}
		//	TCP_send();
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

DWORD WINAPI CMainFrame::connect(LPVOID lParam)
{
	CMainFrame *pthis = (CMainFrame *)lParam;
	HANDLE Psemaphore = CreateSemaphore(NULL,0,100, _T("NetProtocolPsemaphore"));//创建信号量P
	HANDLE Csemaphore = CreateSemaphore(NULL,1,100, _T("NetProtocolCsemaphore"));//创建信号量C
	HANDLE Ssemaphore = CreateSemaphore(NULL,0, 100, _T("NetProtocolCreateSuccess"));//创建信号量S
	HANDLE MFile = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(regstruct), _T("NetProtocolListen"));//请求连接共享内存
	pthis->preg = (regstruct *)MapViewOfFile(MFile, FILE_MAP_WRITE, 0, 0, sizeof(regstruct));//映射共享内存
	while (true){
		WaitForSingleObject(Psemaphore, INFINITE);//等待应用程序请求连接
		parastruct *mypara =   new parastruct();
		ObjEvent   *myevent = new ObjEvent();
		pthis->Sock2Port[myevent] = sockcount; //分配端口号
		pthis->Port2Sock[sockcount] = myevent; //根据端口号找到对应套接字
		pthis->port2Rstruct[sockcount]  = new prostruct();  //分配本地缓存
		pthis->port2Wstruct[sockcount] = new prostruct();  //本地缓存
		pthis->SockMark2Port[pthis->preg->SockMark] = sockcount;
		sockcount++;   //创建共享文件和同步信号量
		myevent->RFile  = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(prostruct), pthis->preg->readfilename);
		myevent->WFile = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(prostruct), pthis->preg->writefilename);
		if (myevent->WFile == NULL || myevent->RFile == NULL){
			AfxMessageBox(_T("创建共享文件失败"));
			continue;
		}
		myevent->Rpro  =  (prostruct *)MapViewOfFile(myevent->RFile, FILE_MAP_WRITE, 0, 0, sizeof(prostruct)); //创建通信环境
		myevent->Wpro =  (prostruct *)MapViewOfFile(myevent->WFile, FILE_MAP_WRITE, 0,0,sizeof(prostruct));
		myevent->PSsock =  CreateSemaphore(NULL, 1, 100, pthis->preg->PSname);
		myevent->PRsock=   CreateSemaphore(NULL, 0, 100, pthis->preg->PRname);
		myevent->PWsock = CreateSemaphore(NULL, 0, 100, pthis->preg->PWname);
		myevent->CSsock =  CreateSemaphore(NULL, 1, 100, pthis->preg->CSname);
		myevent->CRsock=   CreateSemaphore(NULL, 0, 100, pthis->preg->CRname);
		myevent->CWsock = CreateSemaphore(NULL, 0, 100, pthis->preg->CWname);
		if (!(myevent->PRsock || myevent->PSsock || myevent->PWsock || myevent->CSsock || myevent->CRsock || myevent->CWsock)){
			AfxMessageBox(_T("创建信号量失败"));
			continue;
		}
		mypara->pEvent = myevent;
		mypara->pClass = pthis;
    	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReadFromApp, (LPVOID)mypara, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WriteToApp, (LPVOID)mypara, NULL, NULL);
		ReleaseSemaphore(Ssemaphore, 1, NULL);
		ReleaseSemaphore(Csemaphore,1,NULL); //创建完成
	}
	return 0;
}

DWORD WINAPI CMainFrame::ReadFromApp(LPVOID lParam)//从应用程序读取数据
{
	parastruct     *mypara = (parastruct *)lParam;
	CMainFrame *pthis =    (CMainFrame *)mypara->pClass;
	ObjEvent      *myevent = mypara->pEvent;
	while (true)
	{
		WaitForSingleObject(myevent->PRsock, INFINITE);   //等待应用程序写完成
		memcpy(pthis->port2Rstruct[pthis->Sock2Port[myevent]],myevent->Wpro,sizeof(prostruct));//拷贝到本地缓存
		AfxGetApp()->m_pMainWnd->SendMessage(APPTOTRANS, (WPARAM)pthis->Sock2Port[myevent]);//发送本地标号
		ReleaseSemaphore(myevent->CSsock, 1, NULL);     //读数据完成应用程序可再写
	}
}

//发送数据到应用层
DWORD WINAPI CMainFrame::WriteToApp(LPVOID lParam)
{
	parastruct     *mypara  = (parastruct *)lParam;
	CMainFrame *pthis = (CMainFrame *)mypara->pClass;
	ObjEvent      *myevent = mypara->pEvent;
	while (true)
	{
		WaitForSingleObject(myevent->PWsock, INFINITE);  //等待写结构
		memcpy(myevent->Rpro,pthis->port2Wstruct[pthis->Sock2Port[myevent]], sizeof(prostruct)); //拷贝结构
		ReleaseSemaphore(myevent->CRsock, 1, NULL);//应用程序写
	}
}

//将数据拷贝准备发送数据到应用层
LRESULT CMainFrame::OnAppSend(WPARAM wparam, LPARAM lparam)
{
	sockstruct  *pmysock= (sockstruct *)wparam;
	UINT           FuncID = lparam;
	portsrc       tempsrc;
	unsigned short nPort;
	memcpy(tempsrc.srcip, pmysock->srcip, 20); //根据源端口源地址目的端口找到通信端口
	tempsrc.srcport = pmysock->srcport;
	tempsrc.dstport = pmysock->dstport;
	nPort = (src2port.find(tempsrc) == src2port.end()) ? pmysock->dstport : src2port[tempsrc];   //找到目标端口
	WaitForSingleObject(Port2Sock[nPort]->PSsock, INFINITE); //等待
	if (Port2Sock.find(nPort) == Port2Sock.end()){     //若未找到指定端口，丢包
		ReleaseSemaphore(Port2Sock[nPort]->PSsock, 1, NULL);
		return 0;
	}
	memcpy(&port2Wstruct[nPort]->mysock, pmysock, sizeof(sockstruct));  //拷贝数据
	port2Wstruct[nPort]->FuncID = FuncID; //拷贝目的数据
	ReleaseSemaphore(Port2Sock[nPort]->PWsock, 1, NULL);
	return 0;
}
