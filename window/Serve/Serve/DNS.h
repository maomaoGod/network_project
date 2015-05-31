#pragma once


// CDNS 对话框

class CDNS : public CDialogEx
{
	DECLARE_DYNAMIC(CDNS)

public:
	CDNS(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDNS();

// 对话框数据
	enum { IDD = IDD_DNS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
