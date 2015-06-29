// MainFrm.h : CMainFrame 类的接口
//
#pragma once
#include <map>
#include "CMyIP.h"
#include "TransLayer.h"
#include "my_linker.h"
#include "sockpool.h"
using namespace std;


/**
*@class  <CMainFrame>
*@brief   程序框架窗口类,主要完成界面的显示控制,协议栈之间消息的传递
*@author ACM2012
*@note 程序框架窗口类，主要负责界面的现实。在本程序中，程序主框架是协议栈中各个层通信的接口，
*协议栈中各个层通过窗口的消息机制进行通信，从而实现各个层之间的信息交互，选择消息的原因在于窗口消息
*可以使类之间的关系更加清晰，数据的传送更加明确。
*/

class CMainFrame : public CFrameWnd
{

protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// 特性
public:

	// 操作
public:

public:
	int connsocknum;
	CMyIP IP;
	my_linker  linker;
	SockPool   m_sockpool;
	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	HWND PPwnd;
	int mac_flag;
	unsigned int CurIP;
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
	CWnd   *MyWnd;
	int  numprocess;
	// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	/** @brief 传输层处理网络层数据，发往应用层 */
	afx_msg LRESULT OnTrans2App(WPARAM, LPARAM);
	/** @brief 网络层处理链路层数据，发往传输层 */
	afx_msg LRESULT OnIP2Trans(WPARAM, LPARAM);
	/** @brief  链路层抓取数据，发往网络层 */
	afx_msg LRESULT OnLink2IP(WPARAM, LPARAM);
	/** @brief 传输层接收应用层数据，发往网络层 */
	afx_msg LRESULT OnTrans2IP(WPARAM, LPARAM);
	/** @brief 网络层接收传输层数据，发往 */
	afx_msg LRESULT OnIP2Link(WPARAM, LPARAM);
	/** @brief 链路层接受网络层数据发往 */
	afx_msg LRESULT OnLinkSend(WPARAM, LPARAM);
	/** @brief 数据发往应用层*/
	afx_msg LRESULT OnAppSend(WPARAM, LPARAM);
	/** @brief 定时器消息处理函数 */
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	/** @brief  选择程序作为端系统*/
	afx_msg void OnClient();
	/** @brief  选择程序作为路由器 */
	afx_msg void OnRoute();
	/** @brief  使用OSPF选路协议*/
	afx_msg void OnOspf();
	/** @brief  使用RIP选路协议 */
	afx_msg void OnRip();
	/** @brief   MainFrame创建完成进行初始化*/
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};
