
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "CmdView.h"
#include "LogView.h"
#include "RespondView.h"

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

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CToolBar          m_wndToolBar;
	CStatusBar       m_wndStatusBar;
	LogView          *plog;
	CmdView         *pcmd;
	RespondView   *prespond;
	BOOL               flag;
public:
	CSplitterWnd    m_splitter, s_splitter;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT Dispatch(WPARAM wparam, LPARAM lparam);
	afx_msg void OnClose();
};