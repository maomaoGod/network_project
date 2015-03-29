#pragma once


// Applayer 对话框

class Applayer : public CDialogEx
{
	DECLARE_DYNAMIC(Applayer)

public:
	Applayer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Applayer();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
