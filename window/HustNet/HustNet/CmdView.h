/**
 *CmdViewλ�����������Ͻ�,��Ҫ������������(�������),�������������ӳ��(����ӳ��)��\n
 *�ò��ֹ����������̹߳�ͬ��ɣ�\n
 *���߳���ɽ���ά�����ƹ���,������Ʒ����������cmdview��������Ӧ,���ƹ���Լ������Ӧ�Է�ֹ�����޸���ʾ����������\n
 *��ʷ,���ṩ�ӹ������еĹ���,��UserCmd������е�������Ի�ȡ��Ӧ������������(�����Http�Ựģʽ���ȡGet��������);\n
 *���߳��ڽ��յ����̵߳������������źź�,�������������Ӧ���������,�ڱ�������,ʹ���˼򵥵�mapӳ�����,����\n
 *�����ַ���ӳ�䵽����Ĵ�����ָ��,ͨ��ָ�������Ӧ���������,��������ɺ�,֪ͨ���߳�,���߳������Ӧ�Ĵ�����\n
 *�����ʾ��Ϣ�ַ�����CmdView�Ĺ���ģʽ��2��,һ��Ϊ��ͨ����ģʽ����һ�����һ���������Ľ�����ӳ���ɿ����ɣ�\n
 *��һ��Ϊ�û��ӹ�ģʽ,UserCmd�е�����ӹ�CmdView��,���Ի�ȡ������,�Ӷ�����������,������Ľ����������û������������
 */

#pragma once
/**
 *CmdView ��ͼ*/
#include "UserCmd.h"
#include "UICtrl.h"
#include <map>
using namespace std;

typedef void (UserCmd:: *CMDPROC)(CString);

class CmdView : public CEditView
{
	DECLARE_DYNCREATE(CmdView)
protected:
	/**
	*��̬������ʹ�õ��ܱ����Ĺ��캯��
	*/
	CmdView();           
	virtual ~CmdView();

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
protected:	
	STATE        THREADFLAG;
	LINESTATE  LINEFLAG;
	CMDMODE CMDFLAG;
	CString command;
	/**
	*ʵ�ִ������ַ��������������ӳ��ṹ
	*/
	map<CString, CMDPROC> my_map; 
	/**
	 *CmdView������Ϣ�ṹ
	 */
	TEXTMETRIC tm; 
	/**
	*CmdView��ʾ���������
	*/
	CFont   myfont; 
	/**
	*CmdView���ı���
	*/
	CEdit  *myedit;
	/**
	*������ʾ��
	*/
	CString Hint;
	/** 
	*������ʾ������ʾ����
	*/
	int HintPLen;
	/**
	*������ʾ��洢����*/
	int HintSLen; 
	/**
	*������ʾ������к�
	*/
	int HintLine;     
protected:
	DECLARE_MESSAGE_MAP()
public:
	/**
	*@brief �����ڰ��·�ϵͳ��ʱ����,��OnKeyDown֮��,OnKeyUp֮ǰ��
	*/
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	/**
	*@brief ��������ͼ���ӵ��ĵ��󴥷�,��Ҫ��ɽ���ĳ�ʼ������
	*/
	virtual void OnInitialUpdate();
	//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	/**
	*@brief CmdView�������������
	*/
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	/**
	*@brief ������굥��\n
	*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	/**
	*������굥��\n
	*/
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	/**
	*@brief ���ƹ���ƶ�\n
	*/
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	/**
	*@brief ���ڴ�С�ı�ʱ����ȷ����ʾ���к�\n
	*/
	afx_msg void OnSize(UINT nType, int cx, int cy);
	/**
	*@brief ����ӵ�����뽹��
	*/
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	/**
	*@brief ����������ʾ��\n
	* @param wparam �ӹܺ��������ʾ��\n
	*/
	afx_msg  LRESULT  OnSetHint(WPARAM wparam, LPARAM lparam);
	/**
	*@brief �ӹ����������봰��\n
	*@param   wparam  �ӹܺ��������ʾ��\n
	*/
	afx_msg  LRESULT  OnTakeOverCmd(WPARAM wparam, LPARAM lparam);
	/**
	*@brief �û�ģʽ���������Ѵ������\n
	*/
	afx_msg  LRESULT  OnEndInput(WPARAM wparam, LPARAM lparam);
	/**
	*@brief ��ȡ�����ַ���\n
	*@param   wparam  ����command�Ľ��յ�ַ\n
	*/
	afx_msg  LRESULT  OnGetCommand(WPARAM wparam, LPARAM lparam);
	/**
	*@brief ��ȡ����״̬
	*@param   wparam  ����LINEFLAG�Ľ��յ�ַ\n
	*/
	afx_msg  LRESULT  OnGetLineState(WPARAM wparam, LPARAM lparam);
	/**
	*@brief ��ȡ����������״̬
	*@param   wparam  ����CMDSTATE�Ľ��յ�ַ\n
	*/
	afx_msg  LRESULT  OnGetCmdState(WPARAM wparam, LPARAM lparam);
	/**
	*@brief ʵ�������ַ������������֮���ӳ��
	*/
	void  MapTask();
	/**
	*@brief ���������״̬
	*/
	static DWORD WINAPI DEALCMD(LPVOID lParam);
protected:
	/**
	*@brief ����س�����Ϣ,�����������
	*/
	void DealEnter();
	/**
	*@brief ������Ϣ�����߳�,ע����Ϣӳ��
	*/
	void Initialcmd();
	/**
	*@brief �����Ѵ������\n
	*/
	void DealCmdOver();
};