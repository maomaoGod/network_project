#pragma once


// CDNSADDR �Ի���

class CDNSADDR : public CDialogEx
{
	DECLARE_DYNAMIC(CDNSADDR)

public:
	CDNSADDR(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDNSADDR();

// �Ի�������
	enum { IDD = IDD_DNSADDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
