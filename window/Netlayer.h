#pragma once


// Netlayer 对话框

class Netlayer : public CDialogEx
{
	DECLARE_DYNAMIC(Netlayer)

public:
	Netlayer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Netlayer();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
