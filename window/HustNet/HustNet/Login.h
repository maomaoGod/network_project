#pragma once


// Login �Ի���

class Login : public CDialogEx
{
	DECLARE_DYNAMIC(Login)

public:
	CString A, B, C;
	Login(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Login();
	void GetInfo(CString &, CString &, CString&);
// �Ի�������
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
