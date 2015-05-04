
// MainFrm.cpp : CMainFrame ���ʵ�� , ʵ�ִ����,�����,��·��Ĺ��á�
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

int flag = 0; ///< ��־��TCP��UDP

/**
*@class <_iphdr>
*@brief ���IP�ײ��Ľṹ��
*@author ACM2012
*@note
*�Լ������IP�����ײ�
*/
typedef struct _iphdr //����IP�ײ� 
{
	unsigned short ih_version;  ///< ��ʶ�����ݰ���IP�汾��,һ��4λ, 0100��ʾIPv4, 0110��ʾIPv6
	unsigned short ih_len;	    ///< 16λ���ݱ�����
	unsigned short ih_ident;	///< ���ݱ���16λ��ʶ
	unsigned short ih_flags;    ///< ���ݱ��ı�־
	unsigned short ih_offset;	///< ���ݱ���Ƭƫ��
	unsigned short ih_TTL;		///< ���ݱ�������
	unsigned short ih_protl;	///< ���ݱ���Э��
	unsigned short ih_sum;		///< 16λ�ײ������
	unsigned int ih_saddr;		///< 32λԴIP
	unsigned int ih_daddr;		///< 32λĿ��IP
} IP_HEADER;

/**
*@class <_udphdr>
*@brief ���UDP�ײ��Ľṹ��
*@author ACM1201
*@note
*�Լ������UDP�ײ��Ľṹ
*/
typedef struct _udphdr //����UDP�ײ� 
{
	unsigned short uh_saddr;	///<16λԴIP
	unsigned short uh_daddr;	///<16λĿ��IP
	unsigned short uh_sport;	///<16λԴ�˿�
	unsigned short uh_dport;	///<16λĿ�Ķ˿�
	unsigned short uh_len;		///<16λ����
	unsigned short uh_sum;		///<16λУ���
} UDP_HEADER;

/**
*@class <_tcphdr>
*@brief ���TCP�ײ��Ľṹ��
*@author ACM1201
*@note
*�Լ������TCP�ײ��Ľṹ
*/
typedef struct _tcphdr //����TCP�ײ� 
{
	unsigned short th_saddr;	///<16λԴIP
	unsigned short th_daddr;	///<16λĿ��IP
	unsigned short th_sport;	///<16λԴ�˿�
	unsigned short th_dport;	///<16λĿ�Ķ˿�
	unsigned short th_len;		///<16λ����
	unsigned short th_sum;		///<16λУ���
	unsigned short th_num;      ///<���
	unsigned short th_ack;		///<ȷ�Ϻ�
} TCP_HEADER;

struct UDP_Msg{
	UDP_HEADER *udphdr;				///<udpͷ��
	char *data;					///<����
};
struct UDP_Msg MyUDP;

struct TCP_Msg{
	TCP_HEADER *tcphdr;				///<tcpͷ��
	char *data;					///<����
};
struct TCP_Msg MyTCP;

struct IP_Msg{
	IP_HEADER *iphdr;			///<ָ��ip�ײ���ָ��
	unsigned short ih_sport;	///<16λԴ�˿�
	unsigned short ih_dport;	///<16λĿ�Ķ˿�
	unsigned short ih_num;      ///<���
	unsigned short ih_ack;		///<ȷ�Ϻ�
	char *data;					///<����
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
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
           LPCTSTR pszText =    (LPCTSTR)(pCopyDataStruct->lpData);
           DWORD   dwLength = (DWORD)(pCopyDataStruct->cbData);
           CString mystr;
           memcpy(mystr.GetBuffer(dwLength/sizeof(TCHAR)), pszText, dwLength);
           mystr.ReleaseBuffer();
		   SendMessage(APPTOTRANS, (WPARAM)&mystr,(LPARAM)&pwnd2port[pWnd]);
		   //HWND swnd = ::FindWindow(NULL,_T("���пƼ���ѧ����ʵ��ƽ̨"));
           //::SendMessage(port2hwnd[pwnd2port[pWnd]], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
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
//�������ɸú���������returnǰ���һ��Ϊ
LRESULT CMainFrame::OnApp2Trans(WPARAM wparam, LPARAM lparam) //��������������ݵ�Ӧ�ò�Ľӿ�
{   //ʹ��sendmessage��Ӧ�ó�������Ϣ
	//example ��˿ں�Ϊ0��Ӧ�ó�����pCopyDataStruct����  ::SendMessage(port2hwnd[port], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//Ӧ�ò㷢��������������OnCopyData�л�ȡ
	SendMessage(TRANSTOIP, wparam, lparam);
	return 0;
}




LRESULT CMainFrame::OnTrans2App(WPARAM wparam, LPARAM lparam) //��������������ݵ�Ӧ�ò�Ľӿ�
{ //ʹ��sendmessage��Ӧ�ó�������Ϣ
	//example ��˿ں�Ϊ0��Ӧ�ó�����pCopyDataStruct����  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//Ӧ�ò㷢��������������OnCopyData�л�ȡ
	//CString mystr = *((CString *)wparam);
	//int port = *((int *)lparam);



	// UDP
	if (true/* edited later */)
	{
		
	}

	// TCP

	return 0;
}

LRESULT CMainFrame::OnIP2Trans(WPARAM wparam, LPARAM lparam) //����������䵽�����Ľӿ�
{ //
	///< ������·�㷢�͵����ݽ��а���õ����ĳ����Լ�ƫ��, �Ƚ�ƫ�����Ƿ���ڱ��ĳ���
	///< �����ַ�Ƭȱʧ���߼���ͳ����� return FALSE;
	///< ���������ݳɹ����� ���������ļ���ͼ��, ��û�д���
	///< ��IP_msg�ṹ�����Msg�ṹ
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
	}

	return 0;
}

LRESULT CMainFrame::OnIP2Link(WPARAM wparam, LPARAM lparam) //����������ݷ��͵���·��ӿ�
{
	///< �������������Msg�ṹ��IP��ַ���뵽IP_msg�ṹ��,
	///< �����Ϣ���������ͽ��з�Ƭ����, 
	///< ������·��ķ��ͺ����������ʧ�� return FALSE;
	///< ���� return TRUE;
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

LRESULT CMainFrame::OnLinkSend(WPARAM wparam, LPARAM lparam) //��·�������ݷ��ͳ�ȥ�ӿ�
{//
	return 0;
}






