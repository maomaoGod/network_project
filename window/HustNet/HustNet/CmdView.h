
 /**
 * @class <CmdView>
 * @brief   CmdViewλ�����������Ͻ�,��Ҫ������������(�������),�������������ӳ��(����ӳ��)
 * @author ACM2012
 * @note �ò��ֹ����������̹߳�ͬ��ɣ����߳���ɽ���ά�����ƹ���,������Ʒ����������cmdview��
 *������Ӧ,���ƹ���Լ������Ӧ�Է�ֹ�����޸���ʾ������������ʷ,���ṩ�ӹ������еĹ���,��UserCmd
 *������е�������Ի�ȡ��Ӧ������������(�����Http�Ựģʽ���ȡGet��������);���߳��ڽ��յ�����
 *�̵������������źź�,�������������Ӧ���������,�ڱ�������,ʹ���˼򵥵�mapӳ�����,����
 *�����ַ���ӳ�䵽����Ĵ�����ָ��,ͨ��ָ�������Ӧ���������,��������ɺ�,֪ͨ���߳�,���߳�
 *�����Ӧ�Ĵ����������ʾ��Ϣ�ַ�����CmdView�Ĺ���ģʽ��2��,һ��Ϊ��ͨ����ģʽ����һ�����һ��
 *�������Ľ�����ӳ���ɿ����ɣ���һ��Ϊ�û��ӹ�ģʽ,UserCmd�е�����ӹ�CmdView��,���Ի�ȡ��
 *����,�Ӷ�����������,������Ľ����������û������������
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
	/** @brief ʵ�ִ������ַ��������������ӳ��ṹ*/
	map<CString, CMDPROC> my_map; 
	/** @brief CmdView������Ϣ�ṹ*/
	TEXTMETRIC tm; 
	/** @brief CmdView��ʾ���������*/
	CFont   myfont; 
	/** @brief CmdView���ı���*/
	CEdit  *myedit;
	/** @brief ������ʾ��*/
	CString Hint;
	/** @brief ������ʾ������ʾ����*/
	int HintPLen;
	/** @brief ������ʾ��洢����*/
	int HintSLen; 
	/** @brief ������ʾ������к�*/
	int HintLine;     
protected:
	DECLARE_MESSAGE_MAP()
public:
	/**@brief �����ڰ��·�ϵͳ��ʱ����,��OnKeyDown֮��,OnKeyUp֮ǰ*/
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	/**@brief ��������ͼ���ӵ��ĵ��󴥷�,��Ҫ��ɽ���ĳ�ʼ������*/
	virtual void OnInitialUpdate();
	/**@brief CmdView�������������*/
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	/**@brief ������굥��*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	/**@������굥��*/
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	/**@brief ���ƹ���ƶ�*/
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	/**@brief ���ڴ�С�ı�ʱ����ȷ����ʾ���к�*/
	afx_msg void OnSize(UINT nType, int cx, int cy);
	/**@brief ����ӵ�����뽹��*/
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	/**@brief ����������ʾ��*/
	afx_msg  LRESULT  OnSetHint(WPARAM wparam, LPARAM lparam);
	/**@brief �ӹ����������봰��*/
	afx_msg  LRESULT  OnTakeOverCmd(WPARAM wparam, LPARAM lparam);
	/**@brief �û�ģʽ���������Ѵ������*/
	afx_msg  LRESULT  OnEndInput(WPARAM wparam, LPARAM lparam);
	/**@brief ��ȡ�����ַ���*/
	afx_msg  LRESULT  OnGetCommand(WPARAM wparam, LPARAM lparam);
	/**@brief ��ȡ����״̬*/
	afx_msg  LRESULT  OnGetLineState(WPARAM wparam, LPARAM lparam);
	/**@brief ��ȡ����������״̬*/
	afx_msg  LRESULT  OnGetCmdState(WPARAM wparam, LPARAM lparam);
	/**@brief ʵ�������ַ������������֮���ӳ��	*/
	void  MapTask();
	/**@brief ���������״̬*/
	static DWORD WINAPI DEALCMD(LPVOID lParam);
protected:
	/**@brief ����س�����Ϣ,�����������*/
	void DealEnter();
	/**@brief ������Ϣ�����߳�,ע����Ϣӳ��*/
	void Initialcmd();
	/**@brief �����Ѵ������\n*/
	void DealCmdOver();
};