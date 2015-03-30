#pragma once


// Trslayer 对话框

class Trslayer : public CDialogEx
{
	DECLARE_DYNAMIC(Trslayer)

public:
	Trslayer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Trslayer();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
