
// NetProtocolView.h : CNetProtocolView 类的接口
//

#pragma once

#include "resource.h"

/**
*@class  <CNetProtocolView>
*@brief   程序界面窗口类，主要负责显示用户界面，更新界面数据
*@author ACM2012
*@note  程序主要完成界面的更新操作。本程序中主界面主要显示当前服务的套接字和应用程序数量，这些
*处理都由CNetProtocolView类完成
*/


class CNetProtocolView : public CFormView
{
protected: // 仅从序列化创建
	CNetProtocolView();
	DECLARE_DYNCREATE(CNetProtocolView)

public:
	enum{ IDD = IDD_NETPROTOCOL_FORM };

	// 特性
public:
	CNetProtocolDoc* GetDocument() const;

	// 操作
public:

	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

	// 实现
public:
	virtual ~CNetProtocolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	/** @brief 套接字数量改变，激活界面更新窗口数据 */
	afx_msg LRESULT SockStateUpdate(WPARAM, LPARAM);
	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
private:
	unsigned appnum=0; ///<应用程序数量
	unsigned uploadrate=0;///<上传速度
	unsigned downloadrate=0;///<下载速度
};

#ifndef _DEBUG  // NetProtocolView.cpp 中的调试版本
inline CNetProtocolDoc* CNetProtocolView::GetDocument() const
{
	return reinterpret_cast<CNetProtocolDoc*>(m_pDocument);
}
#endif

