#pragma once


// Login 对话框

class Login : public CDialogEx
{
	DECLARE_DYNAMIC(Login)

public:
	CString A, B, C;
	Login(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Login();
	void GetInfo(CString &, CString &, CString&);
// 对话框数据
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
