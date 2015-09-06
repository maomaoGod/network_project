/**@file
*@brief
*@date 2015.4.20
*@version <0.1>
*@note
*�����������õ�ͷ�ļ�
*/
#pragma once

/**
*@class <NetSet>
*@brief  NetSet�ṩ����Э������ã�ʵ������Э����ϴ������ù���
*@author ACM2012
*@note  NetSet �Ի����ࡣNetSet�ṩ����Э������ã�ʵ������Э����ϴ������ù��ܡ�ͨ��NetSet�Ի���
*���Զ�̬���ر��صĶ�̬���ӿ��ļ���ʹ���û���ʵ�ֵ�����Э�顣�ṩ���ּ������ӿ���ƣ��ȿ���
*ͨ�����ļ��Ի�����ʵ�֣�Ҳ����ͨ��·��ֱ�ӽ������á�
*/

class NetSet : public CDialogEx
{
	DECLARE_DYNAMIC(NetSet)

public:
	/**
	*��׼���캯��
	*/
	NetSet(CWnd* pParent = NULL);   
	virtual ~NetSet();

/**
*�Ի�������
*/
	enum { IDD = IDD_SET };

protected:
	/**
	*DDX/DDV ֧��
	*/
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLocalBrowse();
	afx_msg void OnBnClickedServerBrowse();
	afx_msg void OnBnClickedUpload();
};
extern CString ServeIP, User, Password;