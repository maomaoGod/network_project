// MainFrm.cpp : CMainFrame ���ʵ�� , ʵ�ִ����,�����,��·��Ĺ��á�
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
	Serve = _T("NetServe");
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
		LPCTSTR pszText = (LPCTSTR)(pCopyDataStruct->lpData);
		DWORD   dwLength = (DWORD)(pCopyDataStruct->cbData);
		CString mystr;
		memcpy(mystr.GetBuffer(dwLength / sizeof(TCHAR)), pszText, dwLength);
		mystr.ReleaseBuffer();
		PrintView(mystr);
		HWND swnd = ::FindWindow(NULL, _T("���пƼ���ѧ����ʵ��ƽ̨"));
		::SendMessage(port2hwnd[pwnd2port[pWnd]], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	}
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

LRESULT CMainFrame::OnCheck(WPARAM wparam, LPARAM lparam)
{
	HWND mywnd = *((HWND *)wparam);
	PPwnd = ::FindWindow(NULL, _T("���пƼ���ѧ����ʵ��ƽ̨"));
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

LRESULT CMainFrame::OnTrans2App(WPARAM wparam, LPARAM lparam) //��������������ݵ�Ӧ�ò�Ľӿ�
{ //ʹ��sendmessage��Ӧ�ó�������Ϣ
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
	return 0;
}

LRESULT CMainFrame::OnTrans2IP(WPARAM wparam, LPARAM lparam) //����������ݷ��͵������Ľӿ�
{ //ʹ��sendmessage��Ӧ�ó�������Ϣ
	//example ��˿ں�Ϊ0��Ӧ�ó�����pCopyDataStruct����  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//Ӧ�ò㷢��������������OnCopyData�л�ȡ
	CString mystr = *((CString *)wparam);
	unsigned int src_port = *((unsigned int *)lparam);
	
	// �зִ��ݵ����ݣ���ʽΪ��IP+Ŀ�Ķ˿ں�+DATA��
	int split_ip = mystr.Find(_T(':'), 0);
	int split_port = mystr.Find(_T(' '), split_ip);
	using namespace Tools;
	unsigned int dst_ip = IP_string2uint(Tstr::CS2S(mystr.Mid(0, split_ip)));
	unsigned int dst_port = port_string2uint(Tstr::CS2S(mystr.Mid(split_ip+1, split_port-split_ip-1)));
	unsigned int src_ip = getIP();

	// ��ת��Ϊ���ֽڣ��ټ��㳤�ȣ�������������ֽ���
	// ���������Ҫ�պ����ΪӦ�ò��Լ�ת�룬���������Ͽ��Լ��ٴ�����ֽ���
	CString temp_data = mystr.Mid(split_ip);
	unsigned int data_len = CStringA(temp_data).GetLength();

	// UDP
	if (true/* edited later */)
	{
		struct udp_message new_udp_msg;
		// ����UDP���Ķνṹ
		new_udp_msg.udp_src_port = src_port;
		new_udp_msg.udp_dst_port = dst_port;
		new_udp_msg.udp_msg_length = 8+data_len;
		new_udp_msg.udp_app_data = temp_data;
		new_udp_msg.udp_checksum = udpmakesum((u16)data_len, (u16)src_port, (u16)dst_port, data_len%2, (u16 *)&temp_data);

		// UDP��ӵ������
		OnIP2Link((WPARAM)&new_udp_msg, lparam);
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

#define MSS 1024
#define RTT 200

struct tcpmsg
{
	int ACK;
	int seq;
	int time;
};

struct tcplist
{
	tcplist* next;
	int MSG_num;    //�Ѿ����͵ı�����
	int cwnd;       //���ڴ�С
	unsigned int IP;  //IP
	int Threshold;   //��ֵ
	int count;      //��ǰ�Ѿ��ж��ٱ��ĵõ���ȷACK
	struct tcpmsg tcp_msg[100];
}tcp_list;

tcplist* head = NULL;

bool createNodeList()
{
	head = (tcplist*)malloc(sizeof(tcp_list));
	if (NULL == head)
	{
		return false;
	}
	else
	{
		head->MSG_num = 0;
		head->cwnd = MSS;
		head->IP = 0;
		head->Threshold = 65 * 1024;
		head->count = 0;
		head->tcp_msg[0].ACK = 0;
		head->tcp_msg[0].seq = 0;
		head->tcp_msg[0].time = 0;
		head->next = NULL;
		return true;
	}
}

bool addNode(tcplist* tcp_list)
{
	if (NULL == head)
	{
		return false;
	}
	tcplist* p = head->next;
	tcplist* q = head;
	while (NULL != p)
	{
		q = p;
		p = p->next;
	}
	q->next = tcp_list;
	tcp_list->next = NULL;
	return true;
}

bool deletenode(tcplist* p)
{
	tcplist* s1;
	tcplist* s2;
	s1 = head;
	while (s1 != p && s1 != NULL)
	{
		s2 = s1;
		s1 = s1->next;
	}
	if (s1 == NULL)
	{
		printf("the node you want to delete doesn't exist��");
		return false;
	}
	else
	{
		if (s1 == head)
		{
			head = s1->next;
		}
		else if (s1->next == NULL)
		{
			s2->next = NULL;
		}
		else
		{
			s2->next = s1->next;
		}
		return true;
	}
}

tcplist* GetNode(tcplist* head_, unsigned int ip)
{//�ڴ�ͷ���ĵ�����head�в��ҵ�i����㣬���ҵ���0��i��n����
	//�򷵻ظý��Ĵ洢λ�ã����򷵻�NULL��
	int j;
	tcplist *p;
	p = head_;//��ͷ��㿪ʼɨ��
	while (p->next){//˳ָ�����ɨ�裬ֱ��p->nextΪNULLΪֹ
		if (p->IP = ip)  //���ҵ�Ŀ��IP���򷵻�p
		{
			return p;
		}
		p = p->next;
		j++;
	}
	return NULL;
}

int ACK_global;




void TCP_controller()
{
//	// ���߳��ܿص�����
//	for (;;)
//	{
//		if (New_TCP_Link_Created)
//		{
//			// ���ڳ�ʼ��
//            createNodeList();
//		}
//		foreach (TCP_Entity in TCP_Active_List)
//		{
//			Update(TCP_Entity.State);
//			// ӵ�����Ƹ��´��ڴ�С
            //��Ҫ���͵ı������no_����ip
	    unsigned int ip;
		int no_;
		tcplist* temp1;
		temp1 = GetNode(head, ip);
		if (temp1 == NULL)
            {
				    tcplist* node1 = (tcplist*)malloc(sizeof(tcp_list)); 
					head->MSG_num = 1;
					head->cwnd = MSS;
					head->IP = ip;
					head->count = 0;
					head->Threshold = 65 * 1024;
					head->tcp_msg[0].ACK = 0;
					head->tcp_msg[0].seq = no_;
					head->tcp_msg[0].time = GetTickCount();
					head->next = NULL;
					addNode(node1);
             }
		  else
		  {
			  if (head->MSG_num - head->count <= head->cwnd / MSS)
			  {
				  temp1->MSG_num++;
				  head->tcp_msg[temp1->MSG_num].ACK = 0;
				  head->tcp_msg[temp1->MSG_num].seq = no_;
				  head->tcp_msg[temp1->MSG_num].time = GetTickCount();
			  }
			  else //wait();
				  ;
		  }
//			Update(TCP_Entity.Window);
//            
//			// ͳ�Ƹ������Ƿ�ack
//			foreach (Msg_Entity in TCP_Entity.Msg_List)
//			{
//				Update(Msg_Entity.ACK_Cnt);
//				Update(Msg_Entity.State);
//				// ����3��ack��������
//				Update(TCP_Entity.Window);
		//ÿ��һ��ʱ��û�յ������ı��ģ��ط�ACK
//		        ACK_global = Msg_Entity.confirm_no;
//			}
//		}
		if (ACK_global != 0)
		{
			//�õ���Ӧ����ip
			tcplist* temp2;
			temp2 = GetNode(head, ip);
			if (temp2->tcp_msg[temp2->count].seq <= ACK_global)   //����ACK����
			{
				temp2->tcp_msg[temp2->count].ACK++;
			}
			else
			{
				temp2->count++;
			}
			if (temp2->cwnd <= temp2->Threshold) //������
			{
				temp2->cwnd += MSS;
			}
			else
			{
				if (GetTickCount() - temp2->tcp_msg[temp2->count].time > RTT)  //��ʱ������Ϊ������
				{
					temp2->Threshold = temp2->cwnd / 2;
					temp2->cwnd = MSS;
				}
				else if (temp2->tcp_msg[temp2->count].ACK >= 3)    //�յ�3������ACK������Ϊӵ������
				{
					temp2->Threshold = temp2->cwnd / 2;
					temp2->cwnd = temp2->Threshold;
				}
				else      //�յ�ǰ��δȷ�����ݵ�ACK
				{
					temp2->cwnd = temp2->cwnd + MSS*(MSS / temp2->cwnd);
				}
			}
			ACK_global = 0;
		}


//		if (TCP_Link_Destroyed)
//		{
//			// �������а���
		    //������Ҫ�����TCP��ip
		    deletenode( GetNode(head, ip) );
//		}
//	}
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
	if(send((struct IP_Msg *)wparam,(unsigned short)lparam)!=0)
	{
		printf("error sending datagram!\n");
	}
	return 0;
}

DWORD WINAPI CMainFrame::packcap(LPVOID lParam)
{
	return 0;

}