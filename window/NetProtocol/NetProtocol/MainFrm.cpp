// MainFrm.cpp : CMainFrame ���ʵ�� , ʵ�ִ����,�����,��·��Ĺ��á�
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
	psock = NULL;
	if (!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)packcap, (LPVOID) this, NULL, NULL))
		AfxMessageBox(_T("����ץ���߳�ʧ�ܣ�"));
	if (!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)connect, (LPVOID) this, NULL, NULL))
		AfxMessageBox(_T("���������߳�ʧ�ܣ�"));
	// TODO:  �ڴ���ӳ�Ա��ʼ������
	/*(if (!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)packcap, (LPVOID) this, NULL, NULL))
		AfxMessageBox(_T("ץ���̴߳���ʧ��"));
	numprocess = 0;*/
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
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
	// TODO:  �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}

// CMainFrame ���

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
			port2Rstruct[BindPort] = port2Rstruct[nPort]; //�ð󶨵Ķ˿ں�������ԭ���Ļ���
			Port2Sock[BindPort] = Port2Sock[nPort];   
			port2Wstruct[BindPort] = port2Wstruct[nPort];
			port2Rstruct.erase(nPort); //����ԭ���Ļ���
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
LRESULT CMainFrame::OnTrans2App(WPARAM wparam, LPARAM lparam) //��������������ݵ�Ӧ�ò�Ľӿ�
{  //ʹ��sendmessage��Ӧ�ó�������Ϣ
	//example ��˿ں�Ϊ0��Ӧ�ó�����pCopyDataStruct����  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//Ӧ�ò㷢��������������OnCopyData�л�ȡ
	struct Msg new_ip_msg = *((struct Msg *)wparam);

	//// ����UDP��TCP�Ŀ�ͷ����Դ�˿ڣ�ֱ�Ӽ�s����ΪUDP����ȡԴ�˿ں�
	//struct udp_message assume_udp_msg;
	//memcpy(&assume_udp_msg, new_ip_msg.data, strlen(new_ip_msg.data) + 1); // +1 for \0
	//struct tcplist *found_TCP = getNode(new_ip_msg.sip, assume_udp_msg.udp_src_port);

	// UDP
	if (new_ip_msg.protocol == PROTOCOL_UDP)
	{
		// ��ȡUDP���Ķ�
		struct udp_message new_udp_msg;
		memcpy(&new_udp_msg, new_ip_msg.data, new_ip_msg.datelen); // +1 for \0

		// �����
		if (!udpcheck(new_udp_msg.udp_msg_length - 8, new_udp_msg.udp_src_port, new_udp_msg.udp_dst_port, new_udp_msg.udp_msg_length % 2, (u16 *)&(new_udp_msg.udp_app_data), new_udp_msg.udp_checksum))
		{
			// ��������
			//return -1;
		}

		// ��������Ӧ�ò�Ľṹ��
		struct sockstruct new_sockstruct;
		new_sockstruct.dstport = new_udp_msg.udp_dst_port;
		new_sockstruct.srcport = new_udp_msg.udp_src_port;
		new_sockstruct.bindport = 0;
		new_sockstruct.datalength = new_udp_msg.udp_msg_length - 8;
		IP_uint2chars(new_sockstruct.srcip, new_ip_msg.sip);
		IP_uint2chars(new_sockstruct.dstip, new_ip_msg.dip);
		memcpy(new_sockstruct.data, new_udp_msg.udp_app_data, new_sockstruct.datalength); // +1 for \0

		// ����Ӧ�ò�
		SendMessage(APPSEND, (WPARAM)&new_sockstruct, (LPARAM)SOCKSEND);
	}
	// TCP
	else if (new_ip_msg.protocol == PROTOCOL_TCP)
	{
		//// ��ȡTCP���Ķ�
		//struct tcp_message new_tcp_msg;
		//memcpy(&new_tcp_msg, new_ip_msg.data, strlen(new_ip_msg.data) + 1); // +1 for \0

		//// opts��dataһͬ���м���
		//unsigned data_len = strlen(new_tcp_msg.tcp_opts_and_app_data) + 1; // +1 for \0

		//// �����
		//if (!udpcheck(data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, data_len % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
		//{
		//	// ��������
		//	return -1;
		//}

		// ���������¼�
		TCP_receive(new_ip_msg);

		//// ��������Ӧ�ò�Ľṹ��
		//struct sockstruct new_sockstruct;
		//new_sockstruct.dstport = new_tcp_msg.tcp_dst_port;
		//new_sockstruct.srcport = new_tcp_msg.tcp_src_port;
		//new_sockstruct.bindport = 0;
		//new_sockstruct.datalength = data_len;
		//IP_uint2chars(new_sockstruct.srcip, new_ip_msg.sip);
		//IP_uint2chars(new_sockstruct.dstip, new_ip_msg.dip);
		//memcpy(new_sockstruct.data, new_tcp_msg.tcp_opts_and_app_data, new_sockstruct.datalength + 1); // +1 for \0

		//// ����Ӧ�ò�
		//SendMessage(APPSEND, (WPARAM)&new_sockstruct, (LPARAM)SOCKSEND);
	}
	else
	{
		printf("What is this in OnTrans2App?!\n");
	}

	return 0;
}


LRESULT CMainFrame::OnIP2Trans(WPARAM wparam, LPARAM lparam) //����������䵽�����Ľӿ�
{ //
	///< ������·�㷢�͵����ݽ��а���õ����ĳ����Լ�ƫ��, �Ƚ�ƫ�����Ƿ���ڱ��ĳ���
	///< �����ַ�Ƭȱʧ���߼���ͳ����� return FALSE;
	///< ���������ݳɹ����� ���������ļ���ͼ��, ��û�д���
	///< ��IP_msg�ṹ�����Msg�ṹ

	if (!ip.IP2Trans(wparam, lparam))
		return true;
	return false;
}

LRESULT CMainFrame::OnLink2IP(WPARAM wparam, LPARAM lparam) //��·�����������������Ľӿ�
{
	my_linker &receiver = (*(my_linker *)lparam);
	const u_char * packetData = (const u_char *)wparam;
	IP_Msg * ip_msg;
	ip_msg = receiver.combine(packetData);
	if (ip_msg != NULL)
	{
		AfxGetMainWnd()->SendMessage(IPTOTRANS, (WPARAM)ip_msg);
		delete ip_msg->iphdr;
	}
	return 0;
}

LRESULT CMainFrame::OnTrans2IP(WPARAM wparam, LPARAM lparam) //����������ݷ��͵������Ľӿ�
{ //ʹ��sendmessage��Ӧ�ó�������Ϣ
	//example ��˿ں�Ϊ0��Ӧ�ó�����pCopyDataStruct����  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//Ӧ�ò㷢��������������OnCopyData�л�ȡ
	struct sockstruct data_from_applayer = *((struct sockstruct *)wparam);
	unsigned int dst_ip = IP_chars2uint(data_from_applayer.dstip);
	unsigned short dst_port = data_from_applayer.dstport;
	unsigned short src_port = data_from_applayer.srcport;
	unsigned int src_ip = getIP();
	unsigned int data_len = data_from_applayer.datalength;

	// ��ȡFunction ID
	int funID = (int)lparam;

	// �ж���UDP����TCP
	struct tcplist *found_TCP = getNode(src_ip, src_port, dst_ip, dst_port);

	// UDP
	if (found_TCP == NULL)
	{
		struct udp_message new_udp_msg;
		// ����UDP���Ķνṹ
		new_udp_msg.udp_src_port = src_port;
		new_udp_msg.udp_dst_port = dst_port;
		new_udp_msg.udp_msg_length = 8 + data_len;
		memcpy(new_udp_msg.udp_app_data, data_from_applayer.data, data_len + 1); // +1 for \0
		new_udp_msg.udp_checksum = udpmakesum((u16)data_len, (u16)src_port, (u16)dst_port, data_len % 2, (u16 *)&(new_udp_msg.udp_app_data));

		// UDP��ӵ������
		struct Msg new_ip_msg;
		new_ip_msg.sip = src_ip;
		new_ip_msg.dip = dst_ip;
		new_ip_msg.ih_sport = src_port;
		new_ip_msg.ih_dport = dst_port;
		new_ip_msg.datelen = new_udp_msg.udp_msg_length;
		memcpy(new_ip_msg.data, &new_udp_msg, new_ip_msg.datelen);
		new_ip_msg.protocol = PROTOCOL_UDP;	// 17 for UDP

		// ���������
		SendMessage(IPTOLINK, (WPARAM)&new_ip_msg, lparam);
	}
	// TCP
	else
	{
		// �����ж�
		if (funID == SOCKCONNECT)
		{
			// ��������

			// �½�TCP���ӣ���ʼ��TCP��������
			TCP_new(src_ip, src_port, dst_ip, dst_port);

			// ��ȡ�½���TCP����ָ��
			struct tcplist *tcp = getNode(src_ip, src_port, dst_ip, dst_port);

			// ��һ�����֣�����SYN
			// ����SYN���Ķ�
			struct tcp_message new_tcp_msg;
			new_tcp_msg.tcp_src_port = src_port;
			new_tcp_msg.tcp_dst_port = dst_port;
			new_tcp_msg.tcp_seq_number = tcp->seq_number;
			new_tcp_msg.tcp_ack_number = 0;
			new_tcp_msg.tcp_hdr_length = 20;
			new_tcp_msg.tcp_reserved = 0;
			new_tcp_msg.tcp_urg = 0;
			new_tcp_msg.tcp_ack = 0;
			new_tcp_msg.tcp_psh = 0;
			new_tcp_msg.tcp_rst = 0;
			new_tcp_msg.tcp_syn = 1;
			new_tcp_msg.tcp_fin = 0;
			new_tcp_msg.tcp_rcv_window = tcp->rcvd_wind;
			new_tcp_msg.tcp_urg_ptr = NULL;
			new_tcp_msg.tcp_opts_and_app_data[0] = 21;	// whatever
			new_tcp_msg.tcp_checksum = tcpmakesum(1, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, 1, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data));
			
			// ����TCP_send()��������TCP���Ľṹ�ͱ��Ļ��壬��Ϊ����δ��¼SYN
			TCP_Send2IP(new_tcp_msg, src_ip, dst_ip, 1);

			// �ȴ����ԶԷ���syn��ack
			int waited_seq = wait_for_handshaking_ack(tcp);

			// ����TCP_send()��������TCP���Ľṹ�ͱ��Ļ��壬��Ϊ����δ��¼SYN
			new_tcp_msg.tcp_syn = 0;
			TCP_Send2IP(new_tcp_msg, src_ip, dst_ip, 1);
		}
		else if (funID == SOCKSEND)
		{
			// ����������Ӧ�ò�Ĺ������ݶ�
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

LRESULT CMainFrame::OnIP2Link(WPARAM wparam, LPARAM lparam) //����������ݷ��͵���·��ӿ�
{
	///< �������������Msg�ṹ��IP��ַ���뵽IP_msg�ṹ��,
	///< �����Ϣ���������ͽ��з�Ƭ����, 
	///< ������·��ķ��ͺ����������ʧ�� return FALSE;
	///< ���� return TRUE;
	if (!ip.IP2Link(wparam, lparam))
		return true;
	return false;
}


LRESULT CMainFrame::OnLinkSend(WPARAM wparam, LPARAM lparam) //��·�������ݷ��ͳ�ȥ�ӿ�
{
	static int seq = 0;
	if ((linker.send_by_frame((struct IP_Msg *)wparam, linker.get_adapter(), seq)) == 0)
	{
		seq += 1;
		seq = seq % 100;
	}
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
//		if (packetHeader->len != 176) continue;
		AfxGetApp()->m_pMainWnd->SendMessage(LINKTOIP, (WPARAM)packetData, (LPARAM)receiver);
	}
	return 0;
}

DWORD WINAPI CMainFrame::connect(LPVOID lParam)
{
	CMainFrame *pthis = (CMainFrame *)lParam;
	HANDLE Psemaphore = CreateSemaphore(NULL,0,100, _T("NetProtocolPsemaphore"));//�����ź���P
	HANDLE Csemaphore = CreateSemaphore(NULL,1,100, _T("NetProtocolCsemaphore"));//�����ź���C
	HANDLE Ssemaphore = CreateSemaphore(NULL,0, 100, _T("NetProtocolCreateSuccess"));//�����ź���S
	HANDLE MFile = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(regstruct), _T("NetProtocolListen"));//�������ӹ����ڴ�
	pthis->preg = (regstruct *)MapViewOfFile(MFile, FILE_MAP_WRITE, 0, 0, sizeof(regstruct));//ӳ�乲���ڴ�
	while (true){
		WaitForSingleObject(Psemaphore, INFINITE);//�ȴ�Ӧ�ó�����������
		parastruct *mypara =   new parastruct();
		ObjEvent   *myevent = new ObjEvent();
		pthis->Sock2Port[myevent] = sockcount; //����˿ں�
		pthis->Port2Sock[sockcount] = myevent; //���ݶ˿ں��ҵ���Ӧ�׽���
		pthis->port2Rstruct[sockcount]  = new prostruct();  //���䱾�ػ���
		pthis->port2Wstruct[sockcount] = new prostruct();  //���ػ���
		pthis->SockMark2Port[pthis->preg->SockMark] = sockcount;
		sockcount++;   //���������ļ���ͬ���ź���
		myevent->RFile = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(prostruct), pthis->preg->writefilename);
		myevent->WFile = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(prostruct), pthis->preg->readfilename);
		if (myevent->WFile == NULL || myevent->RFile == NULL){
			AfxMessageBox(_T("���������ļ�ʧ��"));
			continue;
		}
		myevent->Rpro  =  (prostruct *)MapViewOfFile(myevent->RFile, FILE_MAP_WRITE, 0, 0, sizeof(prostruct)); //����ͨ�Ż���
		myevent->Wpro =  (prostruct *)MapViewOfFile(myevent->WFile, FILE_MAP_WRITE, 0,0,sizeof(prostruct));
		myevent->PSsock =  CreateSemaphore(NULL, 1, 100, pthis->preg->PSname);
		myevent->PRsock=   CreateSemaphore(NULL, 0, 100, pthis->preg->PRname);
		myevent->PWsock = CreateSemaphore(NULL, 0, 100, pthis->preg->PWname);
		myevent->CSsock =  CreateSemaphore(NULL, 1, 100, pthis->preg->CSname);
		myevent->CRsock=   CreateSemaphore(NULL, 0, 100, pthis->preg->CRname);
		myevent->CWsock = CreateSemaphore(NULL, 0, 100, pthis->preg->CWname);
		if (!(myevent->PRsock || myevent->PSsock || myevent->PWsock || myevent->CSsock || myevent->CRsock || myevent->CWsock)){
			AfxMessageBox(_T("�����ź���ʧ��"));
			continue;
		}
		mypara->pEvent = myevent;
		mypara->pClass = pthis;
    	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReadFromApp, (LPVOID)mypara, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WriteToApp, (LPVOID)mypara, NULL, NULL);
		ReleaseSemaphore(Ssemaphore, 1, NULL);
		ReleaseSemaphore(Csemaphore,1,NULL); //�������
	}
	return 0;
}

DWORD WINAPI CMainFrame::ReadFromApp(LPVOID lParam)//��Ӧ�ó����ȡ����
{
	parastruct     *mypara = (parastruct *)lParam;
	CMainFrame *pthis =    (CMainFrame *)mypara->pClass;
	ObjEvent      *myevent = mypara->pEvent;
	while (true)
	{
		WaitForSingleObject(myevent->PRsock, INFINITE);   //�ȴ�Ӧ�ó���д���
		memcpy(pthis->port2Rstruct[pthis->Sock2Port[myevent]],myevent->Rpro,sizeof(prostruct));//���������ػ���
		AfxGetApp()->m_pMainWnd->SendMessage(APPTOTRANS, (WPARAM)pthis->Sock2Port[myevent]);//���ͱ��ر��
		ReleaseSemaphore(myevent->CSsock, 1, NULL);     //���������Ӧ�ó������д
	}
}

//�������ݵ�Ӧ�ò�
DWORD WINAPI CMainFrame::WriteToApp(LPVOID lParam)
{
	parastruct     *mypara  = (parastruct *)lParam;
	CMainFrame *pthis = (CMainFrame *)mypara->pClass;
	ObjEvent      *myevent = mypara->pEvent;
	while (true)
	{
		WaitForSingleObject(myevent->PWsock, INFINITE);  //�ȴ�д�ṹ
		memcpy(myevent->Wpro,pthis->port2Wstruct[pthis->Sock2Port[myevent]], sizeof(prostruct)); //�����ṹ
		ReleaseSemaphore(myevent->CRsock, 1, NULL);//Ӧ�ó���д
	}
}

//�����ݿ���׼���������ݵ�Ӧ�ò�
LRESULT CMainFrame::OnAppSend(WPARAM wparam, LPARAM lparam)
{
	sockstruct  *pmysock= (sockstruct *)wparam;
	UINT           FuncID = lparam;
	portsrc        tempsrc;
	unsigned short nPort;
	memcpy(tempsrc.srcip, pmysock->srcip, 20); //����Դ�˿�Դ��ַĿ�Ķ˿��ҵ�ͨ�Ŷ˿�
	tempsrc.srcport = pmysock->srcport;
	tempsrc.dstport = pmysock->dstport;
	nPort = (src2port.find(tempsrc) == src2port.end()) ? pmysock->dstport : src2port[tempsrc];   //�ҵ�Ŀ��˿�
	if (Port2Sock.find(nPort) == Port2Sock.end())
		return 0;//��δ�ҵ�ָ���˿ڣ�����
	WaitForSingleObject(Port2Sock[nPort]->PSsock, INFINITE); //�ȴ�
	memcpy(&port2Wstruct[nPort]->mysock, pmysock, sizeof(sockstruct));  //��������
	port2Wstruct[nPort]->FuncID = FuncID; //����Ŀ������
	ReleaseSemaphore(Port2Sock[nPort]->PWsock, 1, NULL);
	return 0;
}
