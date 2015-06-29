#pragma once
/**@file
*date 2015.4.20
*@version <0.1>
*@brief LogView ��ͼ
*/

/**
* @class <LogView>
* @brief   ��־�����࣬��ʾ��־��Ϣ
* @author ACM2012
* @note  LogViewλ������������½ǣ��̳���CEditView�࣬��Ҫ����״̬��Ϣ��������ô���ֻ�ܴӺ���
*�ڲ���ӡ�����������������룻�ڴ����ڲ����е�����˫�������������Ρ������ṩ�����ṩ����������
*���һ���ַ�������մ��ڡ����������������ⲿ��Ϣ��������ϢIDΪPRINT��CLEAN���ֱ𴥷�Print��
*Clean���������������Ҫ��ӡ״̬��Ϣ�������Logviewʱ��ֻ��Ҫ���Ͷ�Ӧ����Ϣ���ɡ���ӡ���ַ���
*ָ������Ϣ����wparam����ʽ���ݽ�����Logview��Դ�ӡ��״̬��Ϣ���м����������ʱ������ͬʱҲ
*�ᱻ����
*/

class LogView : public CEditView
{
	DECLARE_DYNCREATE(LogView)

protected:
	/**
	*��̬������ʹ�õ��ܱ����Ĺ��캯��
	*/
	LogView();
	virtual ~LogView();
	int logcount;
public:
	CString  Log;
public:
	/**
	*��д�Ի��Ƹ���ͼ
	*/
	virtual void OnDraw(CDC* pDC);
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
protected:
	DECLARE_MESSAGE_MAP()
	/**@brief ��LogView��ӡһ���������*/
	afx_msg LRESULT Print(WPARAM wparam, LPARAM lparam);
	/**@brief �����־����*/
	afx_msg LRESULT Clean(WPARAM wparam, LPARAM lparam);
public:
	/**@brief LogView ��ʼ��*/
	virtual void    OnInitialUpdate();
	/**@brief CmdView�������������*/
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	/**@brief ��Ӧ��굥��*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	/**@brief ��Ӧ���˫��*/
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	/**@brief ��Ӧ��������*/
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};



