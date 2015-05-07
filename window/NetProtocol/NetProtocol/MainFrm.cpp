// MainFrm.cpp : CMainFrame ���ʵ�� , ʵ�ִ����,�����,��·��Ĺ��á�
//
#include "stdafx.h"
#include "NetProtocol.h"
#include "MainFrm.h"
#include "CMyIP.h"
#include "string.h"
#include "Tools.h"
#include "pcap.h"

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
	ON_MESSAGE(APPTOTRANS, OnApp2Trans)
	ON_MESSAGE(IPTOTRANS, OnIP2Trans)
	ON_MESSAGE(LINKTOIP, OnLink2IP)
	ON_MESSAGE(TRANSTOIP, OnTrans2IP)
	ON_MESSAGE(IPTOLINK, OnIP2Link)
	ON_MESSAGE(LINKSEND, OnLinkSend)
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
	if (!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)packcap, (LPVOID) this, NULL, NULL))
		AfxMessageBox(_T("����ץ���߳�ʧ�ܣ�"));
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

BOOL CALLBACK lpEnumHwnd(HWND hwnd, LPARAM lParam)//�������д��ڣ�Ѱ�ҿͻ��˺ͷ���������
{
	CString Client, Serve;
	Client = _T("���пƼ���ѧ����ʵ��ƽ̨");
	Serve = _T("����ʵ�������");
	TCHAR str[100];
	::GetWindowText(hwnd, str, 100);
	if (Client.Compare(str) == 0 || Serve.Compare(str) == 0)
		(AfxGetApp()->m_pMainWnd)->SendMessage(CHECKHWND, (WPARAM)&hwnd, lParam);
	return 1;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pwnd2port.find(pWnd) == pwnd2port.end()){  //Ӧ�ó���ע��
		EnumWindows(lpEnumHwnd, (LPARAM)pWnd);
		return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
	}
	if (pCopyDataStruct != NULL){//��������Ӧ�ó������Ϣ
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
	if (index == numprocess&&pwnd2port.find((CWnd *)lparam) == pwnd2port.end()){ //һ������ֻ��ע��һ��
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
	case SOCKBIND:   //��ʱ�����Ƕ˿ڳ�ͻ
		port2hwnd.erase(pwnd2port[pWnd]);
		pwnd2port.erase(pWnd);
		pwnd2port[pWnd] = mysock.bindport;
		port2hwnd[mysock.bindport] = temp;
		PrintView(_T("�󶨶˿ڵ�6500!"));
		break;
	case SOCKLISTEN:
		break;
	case SOCKSEND:
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
LRESULT CMainFrame::OnTrans2App(WPARAM wparam, LPARAM lparam) //��������������ݵ�Ӧ�ò�Ľӿ�
{  //ʹ��sendmessage��Ӧ�ó�������Ϣ
	//example ��˿ں�Ϊ0��Ӧ�ó�����pCopyDataStruct����  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//Ӧ�ò㷢��������������OnCopyData�л�ȡ

	// UDP
	if (true/* edited later */)
	{
		struct udp_message new_udp_msg = *((struct udp_message *)wparam);

		// �����
		if (!udpcheck(new_udp_msg.udp_msg_length-8, new_udp_msg.udp_src_port, new_udp_msg.udp_dst_port, new_udp_msg.udp_msg_length%2, (u16 *)&(new_udp_msg.udp_app_data), new_udp_msg.udp_checksum))
		{
			// ��������
			return -1;
		}

		COPYDATASTRUCT CopyDataStruct;
		// �ֽ���
		CopyDataStruct.cbData = new_udp_msg.udp_msg_length-8;
		// ��������
		CopyDataStruct.lpData = &(new_udp_msg.udp_app_data);
		// ���̼�ͨ��
		::SendMessage(port2hwnd[new_udp_msg.udp_dst_port], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&CopyDataStruct);
	}
	// TCP
	else
	{
		struct tcp_message new_tcp_msg = *((struct tcp_message *)wparam);
		// opts��dataһͬ���м���
		unsigned data_len = CStringA(new_tcp_msg.tcp_opts_and_app_data).GetLength();

		// �����
		if (!udpcheck(data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, data_len%2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
		{
			// ��������
			return -1;
		}

		COPYDATASTRUCT CopyDataStruct;
		// �ֽ���
		CopyDataStruct.cbData = data_len;
		// �������ݣ���ʱ���ֿ�opts��data
		CopyDataStruct.lpData = &(new_tcp_msg.tcp_opts_and_app_data);
		// ���̼�ͨ��
		::SendMessage(port2hwnd[new_tcp_msg.tcp_dst_port], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&CopyDataStruct);
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
{//
	my_linker &receiver = (my_linker)(*(my_linker *)lparam);
	const u_char * packetData = (const u_char *)wparam;
	IP_Msg * ip_msg;
	ip_msg = receiver.combine(packetData);
	if (ip_msg != NULL) AfxGetMainWnd()->SendMessage(IPTOTRANS, (WPARAM)ip_msg);
	return 0;

}

struct tcp_message global_new_tcp_msg;

LRESULT CMainFrame::OnTrans2IP(WPARAM wparam, LPARAM lparam) //����������ݷ��͵������Ľӿ�
{ //ʹ��sendmessage��Ӧ�ó�������Ϣ
	//example ��˿ں�Ϊ0��Ӧ�ó�����pCopyDataStruct����  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//Ӧ�ò㷢��������������OnCopyData�л�ȡ
	struct sockstruct data_from_applayer = *((struct sockstruct *)wparam);
	unsigned int dst_ip = IP_chars2uint(data_from_applayer.dstip);
	unsigned short dst_port = data_from_applayer.dstport;
	unsigned short src_port = data_from_applayer.srcport;
	unsigned int src_ip = IP_chars2uint(data_from_applayer.srcip);
	unsigned int data_len = data_from_applayer.datalength;

	//// ��ת��Ϊ���ֽڣ��ټ��㳤�ȣ�������������ֽ���
	//// ���������Ҫ�պ����ΪӦ�ò��Լ�ת�룬���������Ͽ��Լ��ٴ�����ֽ���
	//CString temp_data = mystr.Mid(split_ip);
	//unsigned int data_len = CStringA(temp_data).GetLength();

	// UDP
	if (true/* edited later */)
	{
		struct udp_message new_udp_msg;
		// ����UDP���Ķνṹ
		new_udp_msg.udp_src_port = src_port;
		new_udp_msg.udp_dst_port = dst_port;
		new_udp_msg.udp_msg_length = 8+data_len;
		memcpy(new_udp_msg.udp_app_data, data_from_applayer.data, data_len+1); // +1 for \0
		//new_udp_msg.udp_checksum = udpmakesum((u16)data_len, (u16)src_port, (u16)dst_port, data_len%2, (u16 *)&temp_data);

		// UDP��ӵ������
		struct Msg new_ip_msg;
		new_ip_msg.sip = src_ip;
		new_ip_msg.dip = dst_ip;
		memcpy(new_ip_msg.data, &new_udp_msg, new_udp_msg.udp_msg_length+1); // +1 for \0
		OnIP2Link((WPARAM)&new_ip_msg, lparam);
	}
	// TCP
	else
	{
		struct tcp_message new_tcp_msg;
		// Ŀǰ��TCPʵ�ֽṹ�����⣬�о����ÿһ�����ӣ���Ҫ��һ���߳���TCP����Ӧ��ACK����������
		// ��TCP���ӶϿ�ʱ�߳�����
		// ����ֻ��һ���̣߳�����TCP�ܿأ�����ά��TCP״̬������Ӧ��ACK��
		// Ȼ������ʱ����TCP���ӵ�TCP�����Ͽ�ʱ���������
		// ���Կ��ǽ��̼䷢��Ϣ��Ҳ���Կ���ֱ����ȫ�ֱ�������־λ
		new_tcp_msg.tcp_src_port = src_port;
		new_tcp_msg.tcp_dst_port = dst_port;

		// �����ж�
		if (true/* method == SK_CONNECT */)
		{
			// ����MainFrmTransTools��ʵ�ֵ���������
			//ShakeHands();
		}
		else if (true/* method == SK_SEND */)
		{
			// ��ά����TCP״̬������в�ѯ���Ƿ��ѽ�������
			// ��δ���������򱨴�
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

