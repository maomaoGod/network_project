#pragma once


// Phylayer 对话框

class Phylayer : public CDialogEx
{
	DECLARE_DYNAMIC(Phylayer)

public:
	Phylayer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Phylayer();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
