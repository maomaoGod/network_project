#pragma once

/**
*RespondViewλ����������Ҳ࣬�̳���CEditView�࣬��Ҫ������Ӧ��Ϣ��������ô���ֻ�ܴӺ����ڲ���ӡ�������������\n
*�����룻�ڴ����ڲ����е�����˫�������������Ρ������ṩ�����ṩ�������������һ���ַ���������RespondView���ݣ���\n
*����մ��ڡ����������������ⲿ��Ϣ��������ϢIDΪPRINT��SETTEXT��CLEAN���ֱ𴥷�Print��SetText��Clean����������\n
*�����Ҫ��ӡ״̬��Ϣ�������Logviewʱ��ֻ��Ҫ���Ͷ�Ӧ����Ϣ���ɡ���ӡ���ַ���ָ������Ϣ����wparam����ʽ���ݽ���,\n
*RespondView���Զ�ͷ�������ʾ��Ϣ����SETTEXT����ȫ�ɴ�ӡ�����ݾ������������ͷ����ʾ��Ϣ��
*/

class RespondView : public CEditView
{
	DECLARE_DYNCREATE(RespondView)

protected:
	/**
	*@brief ��̬������ʹ�õ��ܱ����Ĺ��캯��
	*/
	RespondView();
	virtual ~RespondView();
public:
	CString  Res;
public:
	/**
	*��д�Ի��Ƹ���ͼ
	*/
	virtual void OnDraw(CDC* pDC);
#ifdef _DEBUG
	/**
	*@brief RespondView ���
	*/
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	TEXTMETRIC tm;
protected:
	DECLARE_MESSAGE_MAP()
	/**
	*@brief ��RespondView��ӡһ���������\n
	* @param  wparam  ��Ҫ��ӡ���ַ���
	*/
	afx_msg LRESULT Print(WPARAM wparam, LPARAM lparam);
	/**
	*@brief �����־����
	*/
	afx_msg LRESULT Clean(WPARAM wparam, LPARAM lparam);
	/**
	*@brief ������Ӧ��������
	* @param  wparam  ���õĴ��������ַ���
	*/
	afx_msg LRESULT SetText(WPARAM wparam, LPARAM lparam);
public:
	/**
	*@brief RespondView ��Ϣ�������
	*/
	virtual void OnInitialUpdate();
	/**
	* @param cs  ���崴�����ṹ��
	* @brief RespondView�������������\n
	*/
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	/**
	*@brief �������˫��\n
	*/
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	/**
	*@brief ���ΰ�������\n
	*/
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	/**
	*@brief ������굥��
	*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


