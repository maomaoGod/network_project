// CmdView.cpp : 实现文件
#include "stdafx.h"
#include "HustNet.h"
#include "CmdView.h"
#include "MainFrm.h"
#include "UserCmd.h"
#include <map>

CMainFrame *pfame;
CString  command;
using namespace std;

typedef void (*CMDPROC)(CString);
map<CString, CMDPROC> my_map;
enum  STATE  { THREAD_EXIT, THREAD_WAIT, THREAD_RUN }  THREADFLAG;
enum  LINESTATE {OLDLINE,NEWLINE} LINEFLAG;

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
	ON_MESSAGE(DEALCMDOVER, OnDealCmdOver)
	ON_MESSAGE(ENDINPUT, OnEndInput)
END_MESSAGE_MAP()

// CmdView 诊断
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

// CmdView 消息处理程序
void PrintLog(CString log)
{
	CMD mycmd;
	mycmd.ID = PRINT;
	mycmd.agrs = &log;
	pfame->SendMessage(DISPATCH, LOGVIEW,(LPARAM)(&mycmd));
}

void PrintRp(CString rp)
{
	CMD mycmd;
	mycmd.ID = PRINT;
	mycmd.agrs = &rp;
	pfame->SendMessage(DISPATCH, RPVIEW, (LPARAM)(&mycmd));
}

void CleanRp(CString e)
{
	CMD mycmd;
	mycmd.ID = CLEAN;
	mycmd.agrs = NULL;
	pfame->SendMessage(DISPATCH, RPVIEW, LPARAM(&mycmd));
}

void CleanLog(CString e)
{
	CMD mycmd;
	mycmd.ID = CLEAN;
	mycmd.agrs = NULL;
	pfame->SendMessage(DISPATCH, LOGVIEW, LPARAM(&mycmd));
}

void SetRp(CString e)
{
	CMD mycmd;
	mycmd.ID = SETTEXT;
	mycmd.agrs = &e;
	pfame->SendMessage(DISPATCH, RPVIEW, LPARAM(&mycmd));
}

void SetHint(CString e)
{
	CMD mycmd;
	mycmd.ID = SETHINT;
	mycmd.agrs = &e;
	pfame->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
}

void TakeOverCmd(CString e)
{
	CMD mycmd;
	mycmd.ID = TAKEOVERCMD;
	mycmd.agrs = &e;
	pfame->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
}

void EndInput()
{
	CMD mycmd;
	mycmd.ID = ENDINPUT;
	mycmd.agrs = NULL;
	pfame->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
}

CString  GetLine()
{
	static CString  myline;
	while (LINEFLAG != NEWLINE)
		;
	myline = command;
	EndInput();
	LINEFLAG = OLDLINE;
	return myline;
}

void   MapTask(){   //任务分发
	int index;
	TCHAR c;
	CString Ins,args;
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
		PrintLog(_T("未知命令"));
	}
	else
    	(my_map[Ins])(args);
}

void DealCmdOver()
{
	CMD mycmd;
	mycmd.ID = DEALCMDOVER;
	mycmd.agrs = NULL;
	pfame->SendMessage(DISPATCH, CMDVIEW, (LPARAM)&mycmd);
}

DWORD WINAPI DEALCMD(LPVOID lpParameter){

	while (TRUE){
		if (THREADFLAG == THREAD_EXIT)
			break;
		else if (THREADFLAG == THREAD_WAIT)
			continue;
		else {
			MapTask();
			THREADFLAG = THREAD_WAIT;
			DealCmdOver();
		}
	}
	return 0;
}

void Initialcmd()
{
	THREADFLAG = THREAD_WAIT;
	if (CreateThread(NULL, NULL, DEALCMD, NULL, NULL, NULL)){
		PrintLog(_T("创建消息处理线程成功!\n"));
	}
	else PrintLog(_T("创建消息处理线程失败"));
	my_map.insert(pair<CString, CMDPROC>(_T("printrp"), PrintRp));
	my_map.insert(pair<CString, CMDPROC>(_T("compute"), Compute));
	my_map.insert(pair<CString, CMDPROC>(_T("cleanrp"), CleanRp));
	my_map.insert(pair<CString, CMDPROC>(_T("cleanlog"), CleanLog));
	my_map.insert(pair<CString, CMDPROC>(_T("setrp"), SetRp));
	my_map.insert(pair<CString, CMDPROC>(_T("test"), Test));
	my_map.insert(pair<CString, CMDPROC>(_T("conn"), Connect));
	my_map.insert(pair<CString, CMDPROC>(_T("setdll"), SetDll));
	my_map.insert(pair<CString, CMDPROC>(_T("gethtml"), GetHtml));
	//my_map.insert(pair<CString, CMDPROC>(_T("Mail"), Mail));
	my_map[_T("Mail")] = Mail;//equal to before
	my_map[_T("Chat")] = Chat;
}


// CmdView
void CmdView::DealEnter()
{
		CString strText;
		int        num, len, nIndex,nline;
		len = myedit->GetWindowTextLength(); //移动光标到行尾
		myedit->SetSel(len, len, false);
		myedit->SetFocus();
		num = myedit->GetLineCount();           //处理command行
		nIndex = myedit->LineIndex(HintLine); //获取行号
		len = myedit->LineLength(nIndex);  //获取行长度
		myedit->GetLine(HintLine, strText.GetBuffer(len), len); //获取行内容
		strText.ReleaseBuffer();
		command = (strText.Mid(HintSLen)).TrimLeft();  
		strText.Empty();
        if (HintLine < num - 1){      //多行命令
			num = myedit->GetLineCount();
			for (nline= HintLine + 1; nline< num; nline++){
				nIndex = myedit->LineIndex(nline);
				len = myedit->LineLength(nIndex);
				myedit->GetLine(nline, strText.GetBuffer(len), len);
				strText.ReleaseBuffer();
				command += strText;
				strText.Empty();
			}
		}
	   if (THREADFLAG == THREAD_RUN&&CMDFLAG == ROOT)//未接管模式下命令输入过快
				PrintLog(_T("命令无效：请等待上一命令执行完"));	 
	   else if (THREADFLAG == THREAD_WAIT)  //未接管模式下发送命令
		       THREADFLAG = THREAD_RUN;
	   else  if (CMDFLAG == USER)  //用户模式下
		   LINEFLAG = NEWLINE;
}

void CmdView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值	
	if (VK_BACK == nChar){  //限制删除范围
		CPoint point;
		int num;
		num = myedit->GetLineCount();
		point = GetCaretPos();
		if (point.x <= HintPLen&&num - 1 == HintLine)
			return;
	}
	if (VK_RETURN == nChar) //处理回车消息
		DealEnter();
	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

void CmdView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	pfame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	myedit = (CEdit *)this;
	Initialcmd();

	myfont.CreatePointFont(120,	(LPCTSTR)_T("Times New Roman"));
	myedit->SetFont(&myfont);
	SetHint(_T("Command:"));
	myedit->ReplaceSel(Hint);
	HintLine = 0;
}


BOOL CmdView::PreCreateWindow(CREATESTRUCT& cs)  //设置CmdView属性
{
	// TODO:  在此添加专用代码和/或调用基类  
	m_dwDefaultStyle = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOVSCROLL |ES_MULTILINE | ES_NOHIDESEL;
	return CCtrlView::PreCreateWindow(cs);
}

void CmdView::OnLButtonDown(UINT nFlags, CPoint point) //屏蔽鼠标点击
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return;
	//CEditView::OnLButtonDown(nFlags, point);
}
void CmdView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return;
	//CEditView::OnLButtonDblClk(nFlags, point);
}


void CmdView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CPoint point;
	int nline;
	switch (nChar)
	{
	case  VK_UP:
	case  VK_DOWN:
	case   VK_HOME: return;
	case  VK_LEFT:
		point = GetCaretPos();                        //禁止光标移动到command区域:
		nline = myedit->CharFromPos(point);
		if (point.x <= HintPLen&&HIWORD(nline)==HintLine) 
			return;
	default: break;
	}
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CmdView::OnSize(UINT nType, int cx, int cy)
{
	CEditView::OnSize(nType, cx, cy);
	// TODO:  在此处添加消息处理程序代码
	int num = ((CEdit *)this)->GetLineCount();   //重新获取HintLine行标号
	int nIndex,nline;
	CString strText,temp;
	for (nline = num - 1; nline >= 0; nline--){
		nIndex = ((CEdit*)this)->LineIndex(nline);
		int len = ((CEdit *)this)->LineLength(nIndex);
		((CEdit *)this)->GetLine(nline, strText.GetBuffer(len), len);
		strText.ReleaseBuffer();
		if (len >= HintSLen){
			temp = strText.Mid(0, HintSLen);
			if (temp.Compare(Hint) == 0){
				HintLine = nline;
				break;
			}
		}
	}
}


void CmdView::OnKillFocus(CWnd* pNewWnd)
{
	CEditView::OnKillFocus(pNewWnd);

	// TODO:  在此处添加消息处理程序代码   
	int len = myedit->GetWindowTextLength();  //获取输入焦点
	myedit->SetSel(len,len,false);
	myedit->SetFocus();
}

LRESULT CmdView::OnSetHint(WPARAM wparam, LPARAM lparam)
{
	CClientDC dc(this);
	CSize  sz;
	Hint = *((CString *)wparam);
	dc.GetTextMetrics(&tm);
	sz = dc.GetTextExtent(Hint);
	HintSLen = _tcslen(Hint);
	HintPLen = sz.cx + tm.tmWeight / 200;
	return 0;
}

LRESULT CmdView::OnTakeOverCmd(WPARAM wparam, LPARAM lparam)
{	

	if (CMDFLAG==ROOT)
    	CMDFLAG = USER;
	LINEFLAG = OLDLINE;
	Hint = *((CString *)wparam);
	SetHint(Hint);
	myedit->ReplaceSel(Hint);
	HintLine = myedit->GetLineCount() - 1;
	return 0;
}

LRESULT  CmdView::OnDealCmdOver(WPARAM wparam, LPARAM lparam)
{
	if (CMDFLAG == USER){
		myedit->ReplaceSel(_T("退出成功\r\n"));
		CMDFLAG = ROOT;
		SetHint(_T("Command:"));
	}
	HintLine = myedit->GetLineCount() - 1;
	POINT cur;
	cur.x = 0;
	cur.y = HintLine*tm.tmHeight;
	SetCaretPos(cur);
	SetFocus();
	myedit->ReplaceSel(Hint);
	return 0;
}

LRESULT  CmdView::OnEndInput(WPARAM wparam, LPARAM lparam)
{
	if (CMDFLAG == USER){
		myedit->ReplaceSel(Hint);
		HintLine = myedit->GetLineCount() - 1;
	}
	return 0;
}

