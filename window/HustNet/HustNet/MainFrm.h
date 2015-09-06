/**@file
*@brief  MainFrm类
*@date 2015.4.20
*@version <0.1>
*@note
*框架类的定义
*/
#pragma once
#include "CmdView.h"
#include "LogView.h"
#include "RespondView.h"
/**
*@class  <CMainFrame>
*@brief  主要负责整个程序的界面框架控制
*@author ACM2012
*@note MainFrm是程序的框架类，主要负责整个界面的支撑和控制。MainFrm主要完成界面控制，窗口分割,
*消息中转等功能。在初始化MainFrm时，将客户区分成三块分别显示CmdView，LogView和RespondView,并
*动态加载这三个视图。为减少消息传递，框架负责消息的中转，将消息发送到指定的窗口，完成对应的操作。
*在窗口发生重绘时，动态调整窗口的大小以适应窗口显示。
*/

class CMainFrame : public CFrameWnd
{

protected:
	/**
	*仅从序列化创建
	*/
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	/**
	*特性
	*/
public:

	/**
	*操作
	*/
public:

	/**
	*重写
	*/
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	/**
	*实现
	*/
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	/**
	*控件条嵌入成员
	*/
protected:
	CToolBar          m_wndToolBar;
	CStatusBar       m_wndStatusBar;
	LogView          *plog;
	CmdView         *pcmd;
	RespondView   *prespond;
	BOOL               flag;
	BOOL              netflag;
public:
	CSplitterWnd    m_splitter, s_splitter;
protected:
	/**@brief 主窗体被创建时函数被触发*/
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	/**@brief CMainFrame 消息处理程序实现窗口分割*/
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	/**@brief 框体变化时动态调整窗格大小*/
	afx_msg void OnSize(UINT nType, int cx, int cy);
	/*@brief  接收到DISPATCH消息时函数触发*/
	afx_msg LRESULT Dispatch(WPARAM wparam, LPARAM lparam);
	/*@brief 关闭主窗体时被触发*/
	afx_msg void OnClose();
	/*@brief 进行网络设置*/
	afx_msg void OnNETSET();
	afx_msg void OnHttp();
};