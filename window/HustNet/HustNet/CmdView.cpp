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
// CmdView
#define TIPLEN 8
typedef void (*CMDPROC)(CString);
map<CString, CMDPROC> my_map;

enum  STATE  { THREAD_EXIT, THREAD_WAIT, THREAD_RUN }  THREADFLAG;
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
	mycmd.ID = SET;
	mycmd.agrs = &e;
	pfame->SendMessage(DISPATCH, RPVIEW, LPARAM(&mycmd));
}

void   MapTask(){   //任务分发
	int len, i;
	TCHAR c;
	CString Ins,args;
	args.Empty();
	len = command.GetLength();
	for (i = 0; i < command.GetLength(); i++){
		if ((c = command.GetAt(i)) != _T(' ')){
			Ins += c;
		}
		else break;
	}
	while (i < command.GetLength() && (c = command.GetAt(i++)) != _T(' '));
	args = command.Mid(i);
	if (my_map.find(Ins) == my_map.end()){
		PrintLog(_T("未知命令"));
	}
	else
    	(my_map[Ins])(args);
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
}

void CmdView::DealEnter()
{
	CString strText;
	int        num, len, index, nIndex;
    len = myedit->GetWindowTextLength(); //移动光标到行尾
	myedit->SetSel(len, len, false);
	myedit->SetFocus();
	num = myedit->GetLineCount(); //处理command行
	nIndex = myedit->LineIndex(maskline); //获取行号
	len = myedit->LineLength(nIndex);  //获取行长度
	myedit->GetLine(maskline, strText.GetBuffer(len), len); //获取行内容
	strText.ReleaseBuffer();
	index = TIPLEN;
	while (index < len&&_T(' ') == strText.GetAt(index))
		index++;  //去除空格
	if (index == len){
		command.Empty();
		return;
	}
	else
	    command = strText.Mid(index);
	strText.Empty();
	if (maskline == num - 1 && !command.IsEmpty())  //只有一行命令
		;
	else if (maskline < num - 1){   //多行命令
		num = myedit->GetLineCount();
		for (int i = maskline + 1; i < num; i++){
			nIndex = myedit->LineIndex(i);
			len = myedit->LineLength(nIndex);
			myedit->GetLine(i, strText.GetBuffer(len), len);
			strText.ReleaseBuffer();
			command += strText;
			strText.Empty();
		}
	}
	if (THREADFLAG == THREAD_RUN)
		PrintLog(_T("命令无效：请等待上一命令执行完"));
	else
    	THREADFLAG = THREAD_RUN;
}

void CmdView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值	
	if (VK_BACK == nChar){  //限制删除范围
		CPoint point;
		int num;
		num = myedit->GetLineCount();
		point = GetCaretPos();
		if (point.x <= masklength&&num - 1 == maskline)
			return;
	}
	if (VK_RETURN == nChar) //处理回车消息
		DealEnter();
	CEditView::OnChar(nChar, nRepCnt, nFlags);
	if (VK_RETURN == nChar){
		myedit->ReplaceSel(_T("Command:"));//添加Command提示
		maskline = myedit->GetLineCount()-1;
	}
}

void CmdView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	pfame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	myedit = (CEdit *)this;
	Initialcmd();

	CClientDC dc(this);

	myfont.CreatePointFont(120,	(LPCTSTR)_T("Times New Roman"));
	myedit->SetFont(&myfont);
	
	dc.GetTextMetrics(&tm);
	CSize  sz;
	sz = dc.GetTextExtent(_T("Command:"));
	masklength = sz.cx+tm.tmWeight/200;
	myedit->ReplaceSel(_T("Command:"));
	maskline = 0;
}


BOOL CmdView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此添加专用代码和/或调用基类    //设置CmdView属性
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
		if (point.x <= masklength&&HIWORD(nline)==maskline) 
			return;
	default: break;
	}
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CmdView::OnSize(UINT nType, int cx, int cy)
{
	CEditView::OnSize(nType, cx, cy);
	// TODO:  在此处添加消息处理程序代码
	int num = myedit->GetLineCount();   //重新获取maskline行标号
	int nIndex;
	CString strText,temp;
	for (int i = num - 1; i >= 0; i--){
		nIndex = ((CEdit*)this)->LineIndex(i);
		int len = myedit->LineLength(nIndex);
		myedit->GetLine(i, strText.GetBuffer(len), len);
		strText.ReleaseBuffer();
		if (len >= TIPLEN){
			temp = strText.Mid(0, TIPLEN);
			if (temp.Compare(_T("Command:")) == 0){
				maskline = i;
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
