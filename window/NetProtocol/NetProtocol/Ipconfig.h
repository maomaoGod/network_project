#pragma once


// Ipconfig �Ի���

class Ipconfig : public CDialogEx
{
	DECLARE_DYNAMIC(Ipconfig)

public:
	Ipconfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Ipconfig();

// �Ի�������
	enum { IDD = IDD_IPCONFIG };
	CString IP1, IP2, IP3, IP4, IP5;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void GetIP(CString, CString, CString, CString, CString);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
