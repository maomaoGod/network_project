// ACMNetDlg.h : 头文件
//
#pragma once
#include "afxwin.h"

#include "Tools.h"
using namespace Tools;
//#include "ACMBrow.h"
// CACMNetDlg 对话框
class CACMNetDlg : public CDialogEx
{
// 构造
public:
	CACMNetDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ACMNET_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit  mycmd;
	Dlluser dlltest;
	afx_msg void OnClose();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSetting();
	afx_msg void OnShowset();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
