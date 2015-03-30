#pragma once
//#include  "ShowWebpage.h"
#include "CWebBrowser2.h"
#include "NetHtml.h"
// HtmlDlg 对话框

class HtmlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(HtmlDlg)

public:
	HtmlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~HtmlDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CWebBrowser2  myweb;
	NetHtml   Html;
	afx_msg void OnBnClickedButton1();
	CString getURLContext(LPCTSTR pstrURL);
	virtual BOOL OnInitDialog();
};
