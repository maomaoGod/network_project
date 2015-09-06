/**@file
*@brief
*@date 2015.4.20
*@version <0.1>
*@note
*声明网络设置的头文件
*/
#pragma once

/**
*@class <NetSet>
*@brief  NetSet提供网络协议的设置，实现网络协议的上传和设置功能
*@author ACM2012
*@note  NetSet 对话框类。NetSet提供网络协议的设置，实现网络协议的上传和设置功能。通过NetSet对话框，
*可以动态加载本地的动态链接库文件，使用用户自实现的网络协议。提供两种加载链接库机制，既可以
*通过打开文件对话框来实现，也可以通过路径直接进行设置。
*/

class NetSet : public CDialogEx
{
	DECLARE_DYNAMIC(NetSet)

public:
	/**
	*标准构造函数
	*/
	NetSet(CWnd* pParent = NULL);   
	virtual ~NetSet();

/**
*对话框数据
*/
	enum { IDD = IDD_SET };

protected:
	/**
	*DDX/DDV 支持
	*/
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLocalBrowse();
	afx_msg void OnBnClickedServerBrowse();
	afx_msg void OnBnClickedUpload();
};
extern CString ServeIP, User, Password;