#pragma once
//#include  "ShowWebpage.h"
#include "CWebBrowser2.h"
// GetHtml 对话框

class GetHtml : public CDialogEx
{
	DECLARE_DYNAMIC(GetHtml)

public:
	GetHtml(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GetHtml();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CWebBrowser2  myweb;
	afx_msg void OnBnClickedButton1();
	CString getURLContext(LPCTSTR pstrURL);
	virtual BOOL OnInitDialog();
};
