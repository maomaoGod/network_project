// MainFrm.cpp : CMainFrame ���ʵ�� , ʵ�ִ����,�����,��·��Ĺ��á�
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CString UserName = _T("default");		// This is the User Name; TODO Xian
int ProtocolType = 3;	// This is the protocol type; TODO Xian
// 0 for stop and wait
// 1 for go back n
// 2 for selected resend
// 3 for TCP standard

void (*my_port_listen)(unsigned short);
void (*my_TCP_new)(unsigned int, unsigned short, unsigned int, unsigned short, int);
void (*my_TCP_send)(struct SockStruct_SocketLayer);
void (*my_TCP_receive)(struct Msg_NetLayer);
void (*my_TCP_close)(unsigned int, unsigned short, unsigned int, unsigned short);
void (*my_UDP_Send2IP)(struct SockStruct_SocketLayer, unsigned int, unsigned int, unsigned int);

// CMainFrame ����/����
DWORD WINAPI NewTcpControlThread(LPVOID)
{
	CString UserPath = _T("../../User/");
	CString TransPath = _T("/TransLayerDLL/");
	CString ProtocolPath;
	if (ProtocolType == 0)
	{
		ProtocolPath = _T("StopAndWait.dll");
	}
	else if (ProtocolType == 1)
	{
		ProtocolPath = _T("GoBackN.dll");
	}
	else if (ProtocolType == 2)
	{
		ProtocolPath = _T("SelectedResend.dll");
	}
	else if (ProtocolType == 3)
	{
		ProtocolPath = _T("TCP_Standard.dll");
	}
	else
	{
		printf("Protocol Error!\n");
	}
	CString DLLPath = UserPath + UserName + TransPath + ProtocolPath;

	void (*my_TCP_controller)();
	HINSTANCE hinst = LoadLibrary(DLLPath);
	if (hinst == NULL)
	{
		printf("Dll Not Found!\n");
		return -1;
	}

	my_TCP_controller = (void(*)())GetProcAddress(hinst, "TCP_controller");
	my_port_listen = (void(*)(unsigned short))GetProcAddress(hinst, "port_listen");
	my_TCP_new = (void(*)(unsigned int, unsigned short, unsigned int, unsigned short, int))GetProcAddress(hinst, "TCP_new");
	my_TCP_send = (void(*)(struct SockStruct_SocketLayer))GetProcAddress(hinst, "TCP_send");
	my_TCP_receive = (void(*)(struct Msg_NetLayer))GetProcAddress(hinst, "TCP_receive");
	my_TCP_close = (void(*)(unsigned int, unsigned short, unsigned int, unsigned short))GetProcAddress(hinst, "TCP_close");
	my_UDP_Send2IP = (void(*)(struct SockStruct_SocketLayer, unsigned int, unsigned int, unsigned int))GetProcAddress(hinst, "UDP_Send2IP");

	if (my_TCP_controller == NULL)
	{
		printf("Dll Error!\n");
		FreeLibrary(hinst);
		return -1;
	}
	
	my_TCP_controller();
	return 0;
}

CMainFrame::CMainFrame()
{
	connsocknum = 0;
	mac_flag = 0;
	CurIP = 0;
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




LRESULT CMainFrame::OnTrans2App(WPARAM wparam, LPARAM lparam) //��������������ݵ�Ӧ�ò�Ľӿ�
{   //ʹ��sendmessage��Ӧ�ó�������Ϣ
	//example ��˿ں�Ϊ0��Ӧ�ó�����pCopyDataStruct����  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//Ӧ�ò㷢��������������OnCopyData�л�ȡ
	struct Msg_NetLayer new_ip_msg = *((struct Msg_NetLayer *)wparam);

	// UDP
	if (new_ip_msg.protocol == PROTOCOL_UDP)
	{
		// ��ȡUDP���Ķ�
		struct udp_message new_udp_msg;
		memcpy(&new_udp_msg, new_ip_msg.data, new_ip_msg.datelen); // +1 for \0

		// �����
		if (!CTransLayerTools::udpcheck(new_udp_msg.udp_msg_length - 8, new_udp_msg.udp_src_port, new_udp_msg.udp_dst_port, new_udp_msg.udp_msg_length % 2, (u16 *)&(new_udp_msg.udp_app_data), new_udp_msg.udp_checksum))
		{
			// ��������
			//return -1;
		}

		// ��������Ӧ�ò�Ľṹ��
		struct SockStruct_SocketLayer new_sockstruct;
		new_sockstruct.dstport = new_udp_msg.udp_dst_port;
		new_sockstruct.srcport = new_udp_msg.udp_src_port;
		new_sockstruct.funcID = SOCKSENDTO;
		new_sockstruct.datalength = new_udp_msg.udp_msg_length - 8;
		CTransLayerTools::IP_uint2chars(new_sockstruct.srcip, new_ip_msg.sip);
		CTransLayerTools::IP_uint2chars(new_sockstruct.dstip, new_ip_msg.dip);
		memcpy(new_sockstruct.data, new_udp_msg.udp_app_data, new_sockstruct.datalength);

		// ����Ӧ�ò�
		SendMessage(APPSEND, (WPARAM)&new_sockstruct);
	}
	// TCP
	else if (new_ip_msg.protocol == PROTOCOL_TCP)
	{
		// ���������¼�
		my_TCP_receive(new_ip_msg);
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

	ip_message *Routest = (struct ip_message*)wparam;
	if (Routest->ih_ident == data_info)
	{
		if (!IP.IP2Trans(wparam, lparam))
			return true;
		else
		{
			AfxMessageBox(_T("����㴫�����������ݰ���������.\n"));
			return false;
		}
	}
	else
	{
		if (!IP.RecvMsg(wparam, lparam))
			return true;
		else
		{
			AfxMessageBox(_T("��������·����Ϣ��������.\n"));
			return false;
		}
	}

}

LRESULT CMainFrame::OnLink2IP(WPARAM wparam, LPARAM lparam) //��·�����������������Ľӿ�
{
	linker.Link2IP(wparam);
	return 0;
}

LRESULT CMainFrame::OnTrans2IP(WPARAM wparam, LPARAM lparam) //����������ݷ��͵������Ľӿ�
{ //ʹ��sendmessage��Ӧ�ó�������Ϣ
	//example ��˿ں�Ϊ0��Ӧ�ó�����pCopyDataStruct����  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//Ӧ�ò㷢��������������OnCopyData�л�ȡ
	struct SockStruct_SocketLayer data_from_applayer = *((struct SockStruct_SocketLayer *)wparam);
	unsigned int dst_ip = CTransLayerTools::IP_chars2uint(data_from_applayer.dstip);
	unsigned short dst_port = data_from_applayer.dstport;
	unsigned short src_port = data_from_applayer.srcport;
	unsigned int src_ip = CTransLayerTools::getIP();
	unsigned int data_len = data_from_applayer.datalength;

	CTransLayerTools::IP_uint2chars(data_from_applayer.srcip, src_ip);
	// ת��IP
	if (dst_ip == CTransLayerTools::IP_chars2uint("127.0.0.1"))
	{
		dst_ip = src_ip;
		memcpy(data_from_applayer.dstip, data_from_applayer.srcip, 20);
	}
	// ��ȡFunction ID
	int funcID = data_from_applayer.funcID;
	

	// �ж���UDP����TCP
	// UDP
	if (funcID == SOCKSENDTO)
	{
		// ֱ�ӷ��Ϳ�
		my_UDP_Send2IP(data_from_applayer, src_ip, dst_ip, data_len);
	}
	// TCP
	else
	{
		// �����ж�
		if (funcID == SOCKCONNECT)
		{
			// ��ʽ���������֣������������Ĺ���

			// �½�TCP���ӣ���ʼ��TCP��������
			my_TCP_new(src_ip, src_port, dst_ip, dst_port, LINK_CONNECTING);
		}
		else if (funcID == SOCKSEND)
		{
			// ����������Ӧ�ò�Ĺ������ݶ�
			my_TCP_send(data_from_applayer);
		}
		else if (funcID == SOCKCLOSE)
		{
			// �����Ͽ����ӣ���Ϊ�뿪״̬�����Խ������ݵ��ǲ�����
			my_TCP_close(src_ip, src_port, dst_ip, dst_port);
		}
		else if (funcID == SOCKLISTEN)
		{
			// �����˿�
			my_port_listen(src_port);
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
	clock_t t1 = clock(), t2;
	if (end_connect == 1)
	{
		if (!IP.IP2Link(wparam, lparam))
			return true;
		else
		{
			AfxMessageBox(_T("����㴫�����·���ݰ���������.\n"));
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
				AfxMessageBox(_T("����㴫�����·���ݰ���������.\n"));
				return false;
			}
		}
		else{
			if (!IP.SendMsg(wparam, lparam))
				return true;
			else
			{
				AfxMessageBox(_T("����㷢��·����Ϣ��������.\n"));
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
			AfxMessageBox(_T("����㷢��·����Ϣ��������.\n"));
			return false;
		}
	}
	t2 = t1;
	///< �����·�������Ӧ��ÿ��15s����һ��·����Ϣ
}


LRESULT CMainFrame::OnLinkSend(WPARAM wparam, LPARAM lparam) //��·�������ݷ��ͳ�ȥ�ӿ�
{
	static int seq = 0;
	struct IP_Msg *datagram = (struct IP_Msg *)wparam;
	unsigned int Src_IP = datagram->ih_saddr;
	unsigned int Des_IP = datagram->ih_daddr;
	unsigned short len = datagram->ih_len;
	if (Des_IP != CTransLayerTools::getIP())
	{
		if (CurIP == Des_IP)
			goto next;
		while (true)
		{
			linker.send_broadcast(linker.adapterHandle, Src_IP, Des_IP);
			if (linker.get_mac(linker.adapterHandle)){
				mac_flag = 1; 
				CurIP = Des_IP;
				break;
			}
			Sleep(200);
		}
	}
	else
	{
		for (int i = 0; i < 3; ++i) linker.mac_des[i] = linker.mac_src[i];
	}
	//}
next:
	//for (int i = 0; i < 3; ++i) linker.mac_des[i] = linker.mac_src[i];
	if ((linker.send_by_frame(datagram, linker.adapterHandle, seq, len)) == 0)
	{
		seq += 1;
		seq = seq % 100;
	}
	//��������㷢��ʧ��
	return 0;
}


LRESULT CMainFrame::OnAppSend(WPARAM wparam, LPARAM lparam)
{
	m_sockpool.SendToApp((void *)wparam);
	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (connsocknum != m_sockpool.sockconnum){
		connsocknum = m_sockpool.sockconnum;
		GetActiveView()->SendMessage(SOCKSTATEUPDATE, connsocknum);
	}
	CFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnClient()
{
	// TODO:  �ڴ���������������
	end_connect = 1;
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_CLIENT, MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_ROUTE, MF_BYCOMMAND | MF_UNCHECKED);
}


void CMainFrame::OnRoute()
{
	// TODO:  �ڴ���������������
	end_connect = 0;
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_ROUTE, MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_CLIENT, MF_BYCOMMAND | MF_UNCHECKED);
}


void CMainFrame::OnOspf()
{
	// TODO:  �ڴ���������������
	Routing_select = 1;
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_OSPF, MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_RIP, MF_BYCOMMAND | MF_UNCHECKED);
}


void CMainFrame::OnRip()
{
	// TODO:  �ڴ���������������
	Routing_select = 0;
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_RIP, MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_OSPF, MF_BYCOMMAND | MF_UNCHECKED);
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO:  �ڴ����ר�ô����/����û���
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_CLIENT, MF_BYCOMMAND | MF_CHECKED);
	GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_OSPF, MF_BYCOMMAND | MF_CHECKED);
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
