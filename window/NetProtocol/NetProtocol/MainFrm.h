
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include <map>

using namespace std;

/**
*@class UDP���Ķνṹ
*@brief ���UDP���ĶεĽṹ��
*@author ACM1201
*@note
*UDP���Ķλ����ṹ
*/
struct udp_message
{
	unsigned udp_src_port : 16;	///< Դ�˿ں�
	unsigned udp_dst_port : 16;	///< Ŀ�Ķ˿ں�
	unsigned udp_msg_length : 16;	///< ���ĳ���
	unsigned udp_checksum : 16;	///< �����
	CString udp_app_data;	///< ����
};

/**
*@class TCP���Ķνṹ
*@brief ���TCP���ĶεĽṹ��
*@author ACM1201
*@note
*TCP���Ķλ����ṹ
*/
struct tcp_message
{
	unsigned tcp_src_port : 16;	///< Դ�˿ں�
	unsigned tcp_dst_port : 16;	///< Ŀ�Ķ˿ں�
	unsigned tcp_seq_number : 32;	///< ���
	unsigned tcp_ack_number : 32;	///< ȷ�Ϻţ����ڴ�����һ����ţ�
	unsigned tcp_hdr_length : 4;	///< �ײ����ȣ�ͨ��Ϊ20Byte��
	unsigned tcp_reserved : 6;	///< �����ֶΣ���δʹ�ã�
	unsigned tcp_urg : 1;	///< ������־����δʹ�ã�
	unsigned tcp_ack : 1;	///< ȷ�ϱ�־
	unsigned tcp_psh : 1;	///< �����Ͻ���־����δʹ�ã�
	unsigned tcp_rst : 1;	///< �������ӽ�������
	unsigned tcp_syn : 1;	///< �������ӽ�������
	unsigned tcp_fin : 1;	///< �������ӽ�������
	unsigned tcp_rcv_window : 16;	///< ���մ���
	unsigned tcp_checksum : 16;	///< �����
	unsigned tcp_urg_ptr : 16;	///< ��������ָ�루��δʹ�ã�
	CString tcp_opts_and_app_data;	///< ѡ���ֶΣ�ͨ��û�У���ͨ���ײ�����Ϊ20Byte��������
};

/**
*@class TCP���ķ���״̬�ṹ��
*@brief ���TCP���Ķεķ���״̬�Ľṹ��
*@author ACM1201
*@note
*TCP���Ķη���״̬�����ṹ
*/
struct tcp_state
{
	unsigned int tcp_ack_counter;	///< TCP���Ķη��ͺ�ack���ĵļ���
	unsigned int tcp_rcv_window;	///< TCP���ĶεĽ��ܴ���
};

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	HWND PPwnd;
// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CToolBar            m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	map <CWnd *,int>  pwnd2port;
	map <int ,HWND>  port2hwnd;
	int  numprocess;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg LRESULT OnCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnTrans2App(WPARAM, LPARAM);
	afx_msg LRESULT OnIP2Trans(WPARAM, LPARAM);
	afx_msg LRESULT OnLink2IP(WPARAM, LPARAM);
	afx_msg LRESULT OnTrans2IP(WPARAM, LPARAM);
	afx_msg LRESULT OnIP2Link(WPARAM, LPARAM);
	afx_msg LRESULT OnLinkSend(WPARAM, LPARAM);
};


