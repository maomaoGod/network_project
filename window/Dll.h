#pragma once


// Dll 对话框
typedef int(*ADDPROC)(int a, int b);
class Dll : public CDialogEx
{
	DECLARE_DYNAMIC(Dll)

public:
	Dll(CWnd* pParent = NULL);   // 标准构造函数
	ADDPROC  Add;
	virtual ~Dll();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedLoad();
};