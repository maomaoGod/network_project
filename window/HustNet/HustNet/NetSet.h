#pragma once


// NetSet 对话框

class NetSet : public CDialogEx
{
	DECLARE_DYNAMIC(NetSet)

public:
	NetSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NetSet();

// 对话框数据
	enum { IDD = IDD_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLocalBrowse();
	afx_msg void OnBnClickedServerBrowse();
};
