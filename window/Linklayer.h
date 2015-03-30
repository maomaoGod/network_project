#pragma once


// Linklayer 对话框

class Linklayer : public CDialogEx
{
	DECLARE_DYNAMIC(Linklayer)

public:
	Linklayer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Linklayer();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
