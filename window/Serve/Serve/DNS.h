#pragma once


// CDNS �Ի���

class CDNS : public CDialogEx
{
	DECLARE_DYNAMIC(CDNS)

public:
	CDNS(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDNS();

// �Ի�������
	enum { IDD = IDD_DNS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
