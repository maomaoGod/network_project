
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include <map>

using namespace std;

/**
*@class UDP报文段结构
*@brief 存放UDP报文段的结构体
*@author ACM1201
*@note
*UDP报文段基本结构
*/
struct udp_message
{
	unsigned udp_src_port : 16;	///< 源端口号
	unsigned udp_dst_port : 16;	///< 目的端口好
	unsigned udp_msg_length : 16;	///< 报文长度
	unsigned udp_checksum : 16;	///< 检验和
	CString udp_app_data;	///< 数据
};

/**
*@class TCP报文段结构
*@brief 存放TCP报文段的结构体
*@author ACM1201
*@note
*TCP报文段基本结构
*/
struct tcp_message
{
	unsigned tcp_src_port : 16;	///< 源端口号
	unsigned tcp_dst_port : 16;	///< 目的端口号
	unsigned tcp_seq_number : 32;	///< 序号
	unsigned tcp_ack_number : 32;	///< 确认号（所期待的下一个序号）
	unsigned tcp_hdr_length : 4;	///< 首部长度（通常为20Byte）
	unsigned tcp_reserved : 6;	///< 保留字段（并未使用）
	unsigned tcp_urg : 1;	///< 紧急标志（并未使用）
	unsigned tcp_ack : 1;	///< 确认标志
	unsigned tcp_psh : 1;	///< 立即上交标志（并未使用）
	unsigned tcp_rst : 1;	///< 用于连接建立与拆除
	unsigned tcp_syn : 1;	///< 用于连接建立与拆除
	unsigned tcp_fin : 1;	///< 用于连接建立与拆除
	unsigned tcp_rcv_window : 16;	///< 接收窗口
	unsigned tcp_checksum : 16;	///< 检验和
	unsigned tcp_urg_ptr : 16;	///< 紧急数据指针（并未使用）
	CString tcp_opts_and_app_data;	///< 选项字段（通常没有，故通常首部长度为20Byte）和数据
};

/**
*@class TCP报文发送状态结构体
*@brief 存放TCP报文段的发送状态的结构体
*@author ACM1201
*@note
*TCP报文段发送状态基本结构
*/
struct tcp_state
{
	unsigned int tcp_ack_counter;	///< TCP报文段发送后ack报文的计数
	unsigned int tcp_rcv_window;	///< TCP报文段的接受窗口
};

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	HWND PPwnd;
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CToolBar            m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	map <CWnd *,int>  pwnd2port;
	map <int ,HWND>  port2hwnd;
	int  numprocess;
// 生成的消息映射函数
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


