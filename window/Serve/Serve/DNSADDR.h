#pragma once


// CDNSADDR 对话框

class CDNSADDR : public CDialogEx
{
	DECLARE_DYNAMIC(CDNSADDR)

public:
	CDNSADDR(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDNSADDR();

// 对话框数据
	enum { IDD = IDD_DNSADDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
