#pragma once


// Ipconfig 对话框

class Ipconfig : public CDialogEx
{
	DECLARE_DYNAMIC(Ipconfig)

public:
	Ipconfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Ipconfig();

// 对话框数据
	enum { IDD = IDD_IPCONFIG };
	CString IP1, IP2, IP3, IP4, IP5;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void GetIP(CString, CString, CString, CString, CString);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
