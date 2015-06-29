// CmdView.cpp : ʵ���ļ�
#pragma once
#include "stdafx.h"
#include "HustNet.h"
#include "CmdView.h"
#include "MainFrm.h"
#include "UserCmd.h"
#include "UICtrl.h"

IMPLEMENT_DYNCREATE(CmdView, CEditView)

CmdView::CmdView()
{

}

CmdView::~CmdView()
{
}

BEGIN_MESSAGE_MAP(CmdView, CEditView)
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(TAKEOVERCMD,OnTakeOverCmd)
	ON_MESSAGE(SETHINT, OnSetHint)
	ON_MESSAGE(ENDINPUT, OnEndInput)
	ON_MESSAGE(GETCOMMAND, OnGetCommand)
	ON_MESSAGE(GETLINESTATE, OnGetLineState)
	ON_MESSAGE(GETCMDSTATE, OnGetCmdState)
END_MESSAGE_MAP()

/** @brief CmdView ���*/
#ifdef _DEBUG
void CmdView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CmdView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG

/** @brief CmdView ��Ϣ�������*/

/**
* @author ACM2012
* @note  ʵ�������ַ������������֮���ӳ������ͨ����ģʽ��,��Enter������
* ʵ������ӳ�䲢��������Ĵ�����������ʵ�ּ򵥵�������,������Ϊ��,��ֱ�ӷ���;
* ������������Ӧ������������������Ӧ,����״̬����������"������Ч��ʾ"
*����ͨ������ָ�������غ���*/
void   CmdView::MapTask(){   //����ַ�
	int index;
	CString Ins,args;
	UserCmd usr;
	command.TrimLeft();
	command.TrimRight();
	if (command.IsEmpty())
		return;
	index = command.Find(_T(' '));
	if (index == -1){
		Ins = command;
		args.Empty();
	}
	else {
		Ins = command.Mid(0, index);
		args = (command.Mid(index)).TrimLeft();
	}
	if (my_map.find(Ins) == my_map.end()){
		PrintLog(_T("δ֪����"));
	}
	else
    	(usr.*(my_map[Ins]))(args);
}

/**
* @author ACM2012
* @note  ������InitalCmd�б�����,Ϊ������һ�����߳�,���߳̽���ѭ������,\n
*��������������������,\n
*��ʼִ������,��ִ����ɺ�,����ȴ�״̬,��֪ͨ���߳�����ִ�����
*/
DWORD WINAPI CmdView::DEALCMD(LPVOID lParam){
	CmdView *pThis = (CmdView*)lParam;
	if (pThis == NULL)
		return 0;
	while (TRUE){ ///<ѭ����ѯ��ǰ��Ϣ�����߳�״̬
		if (pThis->THREADFLAG == THREAD_EXIT)///<��Ϊ�˳������߳��˳�
			break;
		else if (pThis->THREADFLAG == THREAD_WAIT)///<��Ϊ�ȴ����������ѯ
			continue;
		else if (pThis->THREADFLAG == THREAD_RUN){
			pThis->MapTask();
			pThis->THREADFLAG = THREAD_WAIT;
			pThis->DealCmdOver( );
		}
	}
   return 0;
}

/**
* @author ACM2012
* @note  ��������Ҫʵ��������̵߳Ĵ���,�Լ�����ӳ����ƵĽ���
* ����ӳ��Ľ���ͨ��map��ʵ��,map��ʵ�ִ��ַ�����һ��UserCmd��������Ӧ������ָ��
*/
void CmdView::Initialcmd()
{
	THREADFLAG = THREAD_WAIT;
	if (CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DEALCMD, (LPVOID) this, NULL, NULL)){
		PrintLog(_T("������Ϣ�����̳߳ɹ�!\n"));
	}
	else PrintLog(_T("������Ϣ�����߳�ʧ��"));
	///< ���������ַ��������������ӳ���ϵ��map������
	my_map.insert(pair<CString, CMDPROC>(_T("cleanrp"), &UserCmd::cleanrp));
	my_map.insert(pair<CString, CMDPROC>(_T("cleanlog"), &UserCmd::cleanlog));
	my_map.insert(pair<CString, CMDPROC>(_T("compute"), &UserCmd::Compute));
	my_map.insert(pair<CString, CMDPROC>(_T("conn"), &UserCmd::Connect));
	my_map.insert(pair<CString, CMDPROC>(_T("setdll"), &UserCmd::SetDll));
	my_map.insert(pair<CString, CMDPROC>(_T("gethtml"), &UserCmd::GetHtml));
	my_map.insert(pair<CString, CMDPROC>(_T("mytest"), &UserCmd::mytest));
//	my_map.insert(pair<CString, CMDPROC>(_T("visit"), &UserCmd::Visit));
	my_map[_T("Http")] = &UserCmd::Http;//equal to before
	my_map[_T("Chat")] = &UserCmd::Chat;
	my_map[_T("Ftp")] = &UserCmd::Ftp;
}

/**
* @author ACM2012
* @note  �������ڰ���Enter��ʱ����,������ȡ��ǰ����������,��ȥ�����׿ո�,ͨ����־λ������ȡ��ǰCmdView
  *����ģʽ,������Ӧ������͵���Ӧ����������;����ͨ����ģʽ�������ַ����ᱻ���͵�����ӳ�亯��
  *MapTask�н��д���,���û�����ģʽ��,�û��������ں������ʱ��ȡ�������ַ���
*/
void CmdView::DealEnter()
{
		CString strText;
		int        num, len, nIndex,nline;
		///<�ƶ���굽��β
		len = myedit->GetWindowTextLength(); 
		myedit->SetSel(len, len, false);
		myedit->SetFocus();
		///<brief �����һ������
		num = myedit->GetLineCount();           
		nIndex = myedit->LineIndex(HintLine); 
		len = myedit->LineLength(nIndex); 
		myedit->GetLine(HintLine, strText.GetBuffer(len), len); 
		strText.ReleaseBuffer();
		command = (strText.Mid(HintSLen)).TrimLeft();  
		strText.Empty();
		///<�����������
        if (HintLine < num - 1){     
			num = myedit->GetLineCount();
			///< ��ÿһ��ƴ�ӵ��ַ���
			for (nline= HintLine + 1; nline< num; nline++){
				nIndex = myedit->LineIndex(nline);
				len = myedit->LineLength(nIndex);
				myedit->GetLine(nline, strText.GetBuffer(len), len);
				strText.ReleaseBuffer();
				command += strText;
				strText.Empty();
			}
		}
		///< �����û�����ģʽ��������������쳣
	   if (THREADFLAG == THREAD_RUN&&CMDFLAG == ROOT)
				PrintLog(_T("������Ч����ȴ���һ����ִ����"));	 
	   /**
	   *@brief ������ͨ����ģʽ����������
	   */
	   else if (THREADFLAG == THREAD_WAIT)  //δ�ӹ�ģʽ�·�������
		       THREADFLAG = THREAD_RUN;
	  ///< �����û�ģʽ����������
	   else  if (CMDFLAG == USER)  //�û�ģʽ��
		   LINEFLAG = NEWLINE;
}

/**
* @author ACM2012
* @note  �����ڰ��·�ϵͳ��ʱ����,��OnKeyDown֮��,OnKeyUp֮ǰ
*������Ҫ������������İ������س����ͻ��˼����������˼���ֹɾ��������ɾ����
*������ʾ,������������������⣻�����س���������Ӧ���������̽��������
*/

void CmdView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ	
	///<brief ������˼�
	if (VK_BACK == nChar){  ///<����ɾ����Χ
		CPoint point;
		int num;
		num = myedit->GetLineCount();
		point = GetCaretPos();
		if (point.x <= HintPLen&&num - 1 == HintLine)
			return;
	}
	///<brief ����س���
	if (VK_RETURN == nChar) ///<����س���Ϣ
		DealEnter();
	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

/**
* @author ACM2012
* @note   *��������ͼ���ӵ��ĵ��󴥷�,��Ҫ��ɽ���ĳ�ʼ������
  *������Ҫʵ����������̵߳ĳ�ʼ��,��ʼ����ɺ󷽽������ַ��������������ӳ��
  *����ʵ�������������,����ͼ����������Ϊ��Times New Roman��������������������ڲ���
  *Command��ʾ�ַ�,������ʱ���������ַ�������
*/
void CmdView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	// TODO:  �ڴ����ר�ô����/����û���
	myedit = (CEdit *)this;
	Initialcmd();
	///<brief ��������
	myfont.CreatePointFont(120,	(LPCTSTR)_T("Times New Roman"));
	myedit->SetFont(&myfont);
	///<brief ��ӡ������ʾ��
	SetHint(_T("Command:"));
	myedit->ReplaceSel(Hint);
	HintLine = 0;
}

/**
* @author ACM2012
* @note   CmdView�������������
  *�������ڼ�����������ǰ����,������Ҫͨ�����ݽ����Ľṹ���ñ�����������Ӧ�Ĵ�����,\n
  *������ķ��Ϊӵ�д�ֱ������,��ʵ���Զ�����,�Զ��ֽⳤ�ַ���Ϊ����,���Ϊ������״̬
  */
BOOL CmdView::PreCreateWindow(CREATESTRUCT& cs)  //����CmdView����
{
	// TODO:  �ڴ����ר�ô����/����û���  
	/**
	*@brief ���ô�����ʽ
	*/
	m_dwDefaultStyle = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOVSCROLL |ES_MULTILINE | ES_NOHIDESEL;
	return CCtrlView::PreCreateWindow(cs);
}

/**
* @author ACM2012
* @note   ������굥������������굥��ʱ����,����ͨ������ѡ���ı������޸���Ӧ���ı�,
* ���������θ���Ϣ,��ֹ�޸�������ʷ
*/
void CmdView::OnLButtonDown(UINT nFlags, CPoint point) //���������
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/**
	*@brief ֱ�ӷ��أ��������������Ϣ
	*/
	return;
	//CEditView::OnLButtonDown(nFlags, point);
}
/**
* @author ACM2012
* @note   ������굥�������������˫��ʱ����,����ͨ������ѡ���ı������޸���Ӧ���ı�,
* ���������θ���Ϣ,��ֹ�޸�������ʷ
*/
void CmdView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/**
	*@brief ֱ�ӷ��أ��������˫����Ϣ
	*/
	return;
	//CEditView::OnLButtonDblClk(nFlags, point);
}

/**
* @author ACM2012
* @note  ���ƹ���ƶ��������ڰ��¼��̰���ʱ����,��Ҫ���ƹ���ƶ���
*���ι������������ƶ�,λ����ʾ�й�������ƶ�ʱ,���ó���Լ���ķ�Χ,�÷�Χ������
*����ʾ����ʾ���Ⱦ���
*/
void CmdView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPoint point;
	int nline;
	switch (nChar)
	{
	/**
	*@brief ֱ�ӷ��أ����ι�������ƶ�
	*/
	case  VK_UP:
	case  VK_DOWN:
	case   VK_HOME: return;
	/**
	*@brief �����������
	*/
	case  VK_LEFT:
		point = GetCaretPos();                        //��ֹ����ƶ���command����:
		nline = myedit->CharFromPos(point);
		/**
		*@brief �������������ƶ��ķ�Χ������
		*/
		if (point.x <= HintPLen&&HIWORD(nline)==HintLine) 
			return;
	default: break;
	}
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

/**
* @author ACM2012
* @note   ���ڴ�С�ı�ʱ����ȷ����ʾ���к�\n
*   �����ڴ��ڴ�С�ı�ʱ����,�����Զ����з��,�ڴ��ڵĿ�ȸı�ʱ,���кſ��ܷ����仯,\n
*   ������ȷ����ʾ�е��к�,��ֹ�����ȡ������
*/
void CmdView::OnSize(UINT nType, int cx, int cy)
{
	CEditView::OnSize(nType, cx, cy);
	// TODO:  �ڴ˴������Ϣ����������
	int num = ((CEdit *)this)->GetLineCount();   //���»�ȡHintLine�б��
	int nIndex,nline;
	CString strText,temp;
	/**
	*@brief ����������ϲ���
	*/
	for (nline = num - 1; nline >= 0; nline--){
		nIndex = ((CEdit*)this)->LineIndex(nline);
		int len = ((CEdit *)this)->LineLength(nIndex);
		((CEdit *)this)->GetLine(nline, strText.GetBuffer(len), len);
		strText.ReleaseBuffer();
		if (len >= HintSLen){
			temp = strText.Mid(0, HintSLen);
			/**
			*@brief ����ҵ�������ʾ�����
			*/
			if (temp.Compare(Hint) == 0){
				HintLine = nline;
				break;
			}
		}
	}
}

/**
* @author ACM2012
* @note   �����ڼ���ʧȥ���뽹��ʱ����,Ϊ��֤CmdViewһֱӵ�����뽹��,���伴��ʧȥ���뽹��ʱ\n
 *���»�ȡ���뽹��
*/
void CmdView::OnKillFocus(CWnd* pNewWnd)
{
	CEditView::OnKillFocus(pNewWnd);

	// TODO:  �ڴ˴������Ϣ����������   
	/**
	*@brief �����ı�����
	*/
	int len = myedit->GetWindowTextLength();  //��ȡ���뽹��
	/**
	*@brief ����ƶ����ı�ĩβ
	*/
	myedit->SetSel(len,len,false);
	/**
	*@brief �������ù��Ϊ���뽹��
	*/
	myedit->SetFocus();
}

/**
* @author ACM2012
* @param wparam �ӹܺ��������ʾ��\n
* @note   �����ڴ��ڽ��յ�SETHINT��Ϣʱ����,����ʵ������������ʾ��Ĺ���,\n 
  *�����¼���������ʾ�����ʾ���Ⱥʹ洢���ȡ���ʾ���ȿ��ƹ����ƶ��ͻ��˼���ɾ��,\n
  *��֤����ɾ��������ʷ�����������ȡʧ��
*/
LRESULT CmdView::OnSetHint(WPARAM wparam, LPARAM lparam)
{
	CClientDC dc(this);
	CSize  sz;
	///<��ȡ�������õ�������ʾ������
	Hint = *((CString *)wparam);
	///<��ȡ������ʾ�����ʾ���ȣ�������������ʾ����
	dc.GetTextMetrics(&tm);
	sz = dc.GetTextExtent(Hint);
	///<��ȡ������ʾ��Ĵ洢���ȣ����������ô洢����
	HintSLen = _tcslen(Hint);
	HintPLen = sz.cx + tm.tmWeight / 200;
	return 0;
}

/**
* @author ACM2012
* @param   wparam  �ӹܺ��������ʾ��\n
* @note   �����ڽ��յ�TAKEOVERCMD��Ϣʱ����,������ɴ���ͨ����ģʽ���û�����ģʽ���л�,\n
*��������������ʾ�ﲢ����������ʾ�����ʾ���Ⱥʹ洢����,�ӹ����������봰�ں�,������\n
*����һ�����������ʾ��,�˺���������뽫��ֱ�ӱ��û�����ػ�����ᱻ����ӳ����ƻ��
*/
LRESULT CmdView::OnTakeOverCmd(WPARAM wparam, LPARAM lparam)
{	
	///<brief �л�CmdView��״̬ģʽ
	if (CMDFLAG==ROOT)
    	CMDFLAG = USER;
	///< �����û�����ģʽ�µ�����״̬
	LINEFLAG = OLDLINE;
	///<brief ��������������ʾ��
	Hint = *((CString *)wparam);
	SetHint(Hint);
	myedit->ReplaceSel(Hint);
	HintLine = myedit->GetLineCount() - 1;
	return 0;
}

/**
* @author ACM2012
* @note �����Ѵ������ �ú���������ӳ�亯������,���������ɺ�,����ǰģʽ�����û�����ģʽ,
* ���˳��û�����ģʽ,������ͨ����ģʽ,����������������ʾ��ΪĬ�ϵġ�Command:������ɺ�������
* ����һ��,��ȡ���뽹�㲢���������ʾ��
*/
void CmdView::DealCmdOver()
{
	///< �л�CmdView��״̬ģʽ
	if (CMDFLAG == USER){
		myedit->ReplaceSel(_T("�˳��ɹ�\r\n"));
		CMDFLAG = ROOT;
	///< ��������������ʾ��ΪĬ��
		SetHint(_T("Command:"));
	}
	HintLine = myedit->GetLineCount() - 1;
	///<���Ĭ�ϵ�������ʾ��
	POINT cur;
	cur.x = 0;
	cur.y = HintLine*tm.tmHeight;
	SetCaretPos(cur);
	SetFocus();
	myedit->ReplaceSel(Hint);
}
/**
* @author ACM2012
* @note   *�û�ģʽ���������Ѵ������\n
*�����ڴ�����յ�ENDINPUT����ʱ����,����Ϣ��GetLine���ͱ����Ѿ���ɴ�����������л�ȡ��һ������,\n
*����ִ��һ���򵥵��ж�,ȷ�����û�ģʽ��,�ڹ�괦���������ʾ��
*/
LRESULT  CmdView::OnEndInput(WPARAM wparam, LPARAM lparam)
{
	/**
	*@brief ����û�����ģʽ�µ�������ʾ��
	*/
	if (CMDFLAG == USER){
		myedit->ReplaceSel(Hint);
		HintLine = myedit->GetLineCount() - 1;
		LINEFLAG = OLDLINE;
	}
	return 0;
}

/**
* @author ACM2012
* @note   �û�ģʽ���ӻ�ȡ�����ַ���
*�����ڴ�����յ�GETCOMMAND����ʱ����,����Ϣ��GetLine���ͣ�д���ַ��װ��wparam��,
*������command������copy��Ŀ���ַ���
*/
LRESULT  CmdView::OnGetCommand(WPARAM wparam, LPARAM lparam)
{
	///<brief ��������command�Ĳ���Ŀ���ַ
	CString *pmyline = (CString *)wparam;
	///<brief ����command��Ŀ���ַ
	*pmyline = command;
	return 0;
}

/**
* @author ACM2012
* @note   *�����ڴ�����յ�GETLINESTATE����ʱ����,����Ϣ��GetLine���ͣ�д���ַ��װ��wparam��,\n
*����������״̬д�뵽ָ���ĵ�ַ����
*/

LRESULT  CmdView::OnGetLineState(WPARAM wparam, LPARAM lparam){
	///<brief ������������״̬�Ĳ���Ŀ���ַ
	LINESTATE *mylinestate = (LINESTATE *)wparam;
	///<brief ��������״̬��Ŀ���ַ
	*mylinestate = LINEFLAG;
	return 0;
}
/**
* @author ACM2012
* @note   �û�ģʽ���ӻ�ȡCmdView����״̬״̬\n
*�����ڴ�����յ�GETCMDSTATE����ʱ����, ����Ϣ��GetLine���ͣ�д���ַ��װ��wparam��, \n
*������CmdView״̬д�뵽ָ���ĵ�ַ���ء�GetLineͨ���ú����жϻ�ȡ����״̬�Ƿ�Ϸ�
*/
LRESULT  CmdView::OnGetCmdState(WPARAM wparam, LPARAM lparam){
	///<brief ��������CmdView״̬�Ĳ���Ŀ���ַ
    CMDMODE *mymode = (CMDMODE *)wparam;
	///<brief ����CmdView״̬��Ŀ���ַ
	*mymode = CMDFLAG;
	return 0;
}
