// CmdView.cpp : 实现文件
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

/** @brief CmdView 诊断*/
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

/** @brief CmdView 消息处理程序*/


/**
 *实现命令字符串到命令处理函数之间的映射\n
 *在普通命令模式下,由Enter键触发,实现命令映射并调用命令的处理函数;\n
 * 本函数实现简单的命令检查,若命令为空,则直接返回;若并不存在相应的命令\n
 *处理函数与命令相对应,则在状态输出栏中输出"命令无效提示"\n
 *否则通过命令指针调用相关函数*/
/** @brief 任务分发*/
void   CmdView::MapTask(){   //任务分发
	int index;
	CString Ins,args;
	UserCmd usr;
	/**
	*@brief 去除命令字符串的首部和尾部空格
	*/
	command.TrimLeft();
	command.TrimRight();
	if (command.IsEmpty())
		return;
	/**
	*@brief 获取命令关键字
	*/
	index = command.Find(_T(' '));
	if (index == -1){
		Ins = command;
		args.Empty();
	}
	else {
		Ins = command.Mid(0, index);
		args = (command.Mid(index)).TrimLeft();
	}
	/**
	*@brief 查询命令关键字，若未查询到，则返回，否则执行命令处理函数
	*/
	if (my_map.find(Ins) == my_map.end()){
		PrintLog(_T("未知命令"));
	}
	else
    	(usr.*(my_map[Ins]))(args);
}

/** @brief 监听命令处理状态*/
/** 
  *函数在InitalCmd中被调用,为创建的一个新线程,该线程进行循环测试,\n
  *若满足命令解析完成条件,\n
  *则开始执行命令,待执行完成后,进入等待状态,并通知主线程命令执行完成
  */
DWORD WINAPI CmdView::DEALCMD(LPVOID lParam){
	CmdView *pThis = (CmdView*)lParam;
	if (pThis == NULL)
		return 0;
	/**
	*@brief 循环查询当前消息处理线程状态
	*/
	while (TRUE){
		/**
		*@brief 若为退出，则线程退出
		*/
		if (pThis->THREADFLAG == THREAD_EXIT)
			break;
		/**
		*@brief 若为等待，则继续查询
		*/
		else if (pThis->THREADFLAG == THREAD_WAIT)
			continue;
		/**
		*@brief 若为执行，则进行消息处理
		*/
		else if (pThis->THREADFLAG == THREAD_RUN){
			pThis->MapTask();
			pThis->THREADFLAG = THREAD_WAIT;
			pThis->DealCmdOver( );
		}
	}
   return 0;
}
/** @brief 创建消息处理线程,注册消息映射*/
/**
  *本函数主要实现命令处理线程的创建,以及命令映射机制的建立\n
  *命令映射的建立通过map类实现,map类实现从字符串到一个UserCmd命令中相应的命令指针
  */
void CmdView::Initialcmd()
{
	/**
	*@brief 设置处理命令线程地址为等待状态
	*/
	THREADFLAG = THREAD_WAIT;
	/**
	*@brief 创建处理命令线程地址
	*/
	if (CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DEALCMD, (LPVOID) this, NULL, NULL)){
		PrintLog(_T("创建消息处理线程成功!\n"));
	}
	else PrintLog(_T("创建消息处理线程失败"));
	/**
	*@brief 插入命令字符串到命令处理函数的映射关系到map容器中
	*/
	my_map.insert(pair<CString, CMDPROC>(_T("cleanrp"), &UserCmd::cleanrp));
	my_map.insert(pair<CString, CMDPROC>(_T("cleanlog"), &UserCmd::cleanlog));
	my_map.insert(pair<CString, CMDPROC>(_T("compute"), &UserCmd::Compute));
	my_map.insert(pair<CString, CMDPROC>(_T("conn"), &UserCmd::Connect));
	my_map.insert(pair<CString, CMDPROC>(_T("setdll"), &UserCmd::SetDll));
	my_map.insert(pair<CString, CMDPROC>(_T("gethtml"), &UserCmd::GetHtml));
	//my_map.insert(pair<CString, CMDPROC>(_T("visit"), &UserCmd::Visit));
	my_map.insert(pair<CString, CMDPROC>(_T("mytest"), &UserCmd::mytest));
	my_map[_T("Http")] = &UserCmd::Http;//equal to before
	my_map[_T("Chat")] = &UserCmd::Chat;
	my_map[_T("Ftp")] = &UserCmd::Ftp;
	my_map[_T("smtp")] = &UserCmd::Smtp;
}

/** @brief 处理回车键消息,触发命令处理函数*/
/**
  *本函数在按下Enter键时触发,函数获取当前输入命令行,并去除串首空格,通过标志位来将获取当前CmdView\n
  *工作模式,并将相应的命令发送到对应的命令处理过程;在普通命令模式下命令字符串会被发送到命令映射函数\n
  *MapTask中进行处理,在用户命令模式下,用户函数将在函数完成时获取子命令字符串
  */
void CmdView::DealEnter()
{
		CString strText;
		int        num, len, nIndex,nline;
		/**
		*@brief 移动光标到行尾
		*/
		len = myedit->GetWindowTextLength(); 
		myedit->SetSel(len, len, false);
		myedit->SetFocus();
		/**
		*@brief 处理第一行命令
		*/
		num = myedit->GetLineCount();           
		nIndex = myedit->LineIndex(HintLine); 
		len = myedit->LineLength(nIndex); 
		myedit->GetLine(HintLine, strText.GetBuffer(len), len); 
		strText.ReleaseBuffer();
		command = (strText.Mid(HintSLen)).TrimLeft();  
		strText.Empty();
		/**
		*@brief 处理多行命令
		*/
        if (HintLine < num - 1){     
			num = myedit->GetLineCount();
			/**
			*@brief 将每一行拼接到字符串
			*/
			for (nline= HintLine + 1; nline< num; nline++){
				nIndex = myedit->LineIndex(nline);
				len = myedit->LineLength(nIndex);
				myedit->GetLine(nline, strText.GetBuffer(len), len);
				strText.ReleaseBuffer();
				command += strText;
				strText.Empty();
			}
		}
		/**
		*@brief 处理用户命令模式下命令输入过快异常
		*/
	   if (THREADFLAG == THREAD_RUN&&CMDFLAG == ROOT)
				PrintLog(_T("命令无效：请等待上一命令执行完"));	 
	   /**
	   *@brief 处理普通命令模式下命令输入
	   */
	   else if (THREADFLAG == THREAD_WAIT)  //未接管模式下发送命令
		       THREADFLAG = THREAD_RUN;
	   /**
	   *@brief 处理用户模式下命令输入
	   */
	   else  if (CMDFLAG == USER)  //用户模式下
		   LINEFLAG = NEWLINE;
}
/** @brief CmdView监听每一次非系统键输入,监听到回车和回退按键时进行特殊处理*/
/**
  *函数在按下非系统键时触发,在OnKeyDown之后,OnKeyUp之前。\n
  *函数主要处理两个特殊的按键：回车键和回退键。监听回退键防止删除过程中删除了\n
  *输入提示,导致命令解析出现问题；监听回车键触发响应的命令处理过程进行命令处理
  */
void CmdView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值	
	/**
	*@brief 处理回退键
	*/
	if (VK_BACK == nChar){  //限制删除范围
		CPoint point;
		int num;
		num = myedit->GetLineCount();
		point = GetCaretPos();
		if (point.x <= HintPLen&&num - 1 == HintLine)
			return;
	}
	/**
	*@brief 处理回车键
	*/
	if (VK_RETURN == nChar) //处理回车消息
		DealEnter();
	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

/** 
  *函数在视图附加到文档后触发,主要完成界面的初始化工作\n
  *函数主要实现命令出现线程的初始化,初始化完成后方建立起字符串到命令处理函数的映射\n；
  *函数实现了字体的设置,本视图的字体设置为“Times New Roman”；函数在命令输入框内插入\n
  *Command提示字符,表明此时可以输入字符串命令
*/
void CmdView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	myedit = (CEdit *)this;
	Initialcmd();
	/**
	*@brief 设置字体
	*/
	myfont.CreatePointFont(120,	(LPCTSTR)_T("Times New Roman"));
	myedit->SetFont(&myfont);
	/**
	*@brief 打印命令提示语
	*/
	SetHint(_T("Command:"));
	myedit->ReplaceSel(Hint);
	HintLine = 0;
}


/**
  *CmdView框体的属性设置
  *本函数在即将创建窗体前触发,函数主要通过传递进来的结构引用变量来设置相应的窗体风格,\n
  *本窗体的风格为拥有垂直滚动条,并实现自动换行,自动分解长字符串为多行,光标为不隐藏状态
  */
BOOL CmdView::PreCreateWindow(CREATESTRUCT& cs)  //设置CmdView属性
{
	// TODO:  在此添加专用代码和/或调用基类  
	/**
	*@brief 设置窗口样式
	*/
	m_dwDefaultStyle = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOVSCROLL |ES_MULTILINE | ES_NOHIDESEL;
	return CCtrlView::PreCreateWindow(cs);
}
/**
  *屏蔽鼠标单击\n
  *本函数在鼠标单击时触发,函数通过单击选中文本可以修改相应的文本,本函数屏蔽该消息,防止修改命令历史
  */
void CmdView::OnLButtonDown(UINT nFlags, CPoint point) //屏蔽鼠标点击
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	/**
	*@brief 直接返回，屏蔽左键单击消息
	*/
	return;
	//CEditView::OnLButtonDown(nFlags, point);
}
/**
  *屏蔽鼠标单击\n
  *本函数在鼠标双击时触发,函数通过单击选中文本可以修改相应的文本,本函数屏蔽该消息,防止修改命令历史
  */
void CmdView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	/**
	*@brief 直接返回，屏蔽左键双击消息
	*/
	return;
	//CEditView::OnLButtonDblClk(nFlags, point);
}

/**
  *限制光标移动\n
  *本函数在按下键盘按键时触发,主要限制光标移动；屏蔽光标的向上向下移动,位于提示行光标向左移动时,\n
  *不得超过约定的范围,该范围由输入的提示符显示长度决定
  */
void CmdView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CPoint point;
	int nline;
	switch (nChar)
	{
	/**
	*@brief 直接返回，屏蔽光标上下移动
	*/
	case  VK_UP:
	case  VK_DOWN:
	case   VK_HOME: return;
	/**
	*@brief 监听光标左移
	*/
	case  VK_LEFT:
		point = GetCaretPos();                        //禁止光标移动到command区域:
		nline = myedit->CharFromPos(point);
		/**
		*@brief 如果超过光标能移动的范围，返回
		*/
		if (point.x <= HintPLen&&HIWORD(nline)==HintLine) 
			return;
	default: break;
	}
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}
/**
  *窗口大小改变时重新确定提示行行号\n
  *函数在窗口大小改变时触发,窗口自动换行风格,在窗口的宽度改变时,其行号可能发生变化,\n
  *需重新确定提示行的行号,防止错误获取命令行
  */
void CmdView::OnSize(UINT nType, int cx, int cy)
{
	CEditView::OnSize(nType, cx, cy);
	// TODO:  在此处添加消息处理程序代码
	int num = ((CEdit *)this)->GetLineCount();   //重新获取HintLine行标号
	int nIndex,nline;
	CString strText,temp;
	/**
	*@brief 从最底行往上查找
	*/
	for (nline = num - 1; nline >= 0; nline--){
		nIndex = ((CEdit*)this)->LineIndex(nline);
		int len = ((CEdit *)this)->LineLength(nIndex);
		((CEdit *)this)->GetLine(nline, strText.GetBuffer(len), len);
		strText.ReleaseBuffer();
		if (len >= HintSLen){
			temp = strText.Mid(0, HintSLen);
			/**
			*@brief 如果找到命令提示语，返回
			*/
			if (temp.Compare(Hint) == 0){
				HintLine = nline;
				break;
			}
		}
	}
}

/**
  *保持拥有输入焦点
  *函数在即将失去输入焦点时触发,为保证CmdView一直拥有输入焦点,在其即将失去输入焦点时\n
  *重新获取输入焦点
  */
void CmdView::OnKillFocus(CWnd* pNewWnd)
{
	CEditView::OnKillFocus(pNewWnd);

	// TODO:  在此处添加消息处理程序代码   
	/**
	*@brief 计算文本长度
	*/
	int len = myedit->GetWindowTextLength();  //获取输入焦点
	/**
	*@brief 光标移动至文本末尾
	*/
	myedit->SetSel(len,len,false);
	/**
	*@brief 重新设置光标为输入焦点
	*/
	myedit->SetFocus();
}

/**
  *设置命令提示语\n
  * @param wparam 接管后的命令提示语\n
  *函数在窗口接收到SETHINT消息时触发,函数实现设置命令提示语的功能,\n 
  *并重新计算命令提示语的显示长度和存储长度。显示长度控制光标的移动和回退键的删除,\n
  *保证不会删除命令历史而导致命令获取失败
  */
LRESULT CmdView::OnSetHint(WPARAM wparam, LPARAM lparam)
{
	CClientDC dc(this);
	CSize  sz;
	/**
	*@brief 获取重新设置的命令提示语内容
	*/
	Hint = *((CString *)wparam);
	/**
	*@brief 获取命令提示语的显示长度，并重新设置显示长度
	*/
	dc.GetTextMetrics(&tm);
	sz = dc.GetTextExtent(Hint);
	/**
	*@brief 获取命令提示语的存储长度，并重新设置存储长度
	*/
	HintSLen = _tcslen(Hint);
	HintPLen = sz.cx + tm.tmWeight / 200;
	return 0;
}

/**
  *接管命令行输入窗口\n
  *@param   wparam  接管后的命令提示语\n
  *函数在接收到TAKEOVERCMD消息时触发,函数完成从普通命令模式到用户命令模式的切换,\n
  *重新设置命令提示语并计算命令提示语的显示长度和存储长度,接管命令行输入窗口后,函数将\n
  *另起一行输出命令提示语,此后的命令输入将被直接被用户命令截获而不会被命令映射机制获得*/
LRESULT CmdView::OnTakeOverCmd(WPARAM wparam, LPARAM lparam)
{	
	/**
	*@brief 切换CmdView的状态模式
	*/
	if (CMDFLAG==ROOT)
    	CMDFLAG = USER;
	/**
	*@brief 设置用户命令模式下的输入状态
	*/
	LINEFLAG = OLDLINE;
	/**
	*@brief 重新设置命令提示语
	*/
	Hint = *((CString *)wparam);
	SetHint(Hint);
	myedit->ReplaceSel(Hint);
	HintLine = myedit->GetLineCount() - 1;
	return 0;
}

/**
 *命令已处理完成\n
 *该函数由命令映射函数调用,当命令处理完成后,若当前模式处于用户命令模式,则退出用户命令模式,进入普通命令模式,\n
 *并重新设置命令提示语为默认的“Command:”。完成后命令行另起一行,获取输入焦点并输出命令提示语
 */
void CmdView::DealCmdOver()
{
	/**
	*@brief 切换CmdView的状态模式
	*/
	if (CMDFLAG == USER){
		myedit->ReplaceSel(_T("退出成功\r\n"));
		CMDFLAG = ROOT;
		/**
		*@brief 重新设置命令提示语为默认
		*/
		SetHint(_T("Command:"));
	}
	HintLine = myedit->GetLineCount() - 1;
	/**
	*@brief 输出默认的命令提示语
	*/
	POINT cur;
	cur.x = 0;
	cur.y = HintLine*tm.tmHeight;
	SetCaretPos(cur);
	SetFocus();
	myedit->ReplaceSel(Hint);
}
/**
  *用户模式下子命令已处理完成\n
  *函数在窗体接收到ENDINPUT命令时触发,该消息由GetLine发送表明已经完成从命令输入框中获取了一行输入,\n
  *函数执行一个简单的判断,确认在用户模式下,在光标处输出命令提示语*/
LRESULT  CmdView::OnEndInput(WPARAM wparam, LPARAM lparam)
{
	/**
	*@brief 输出用户命令模式下的命令提示语
	*/
	if (CMDFLAG == USER){
		myedit->ReplaceSel(Hint);
		HintLine = myedit->GetLineCount() - 1;
		LINEFLAG = OLDLINE;
	}
	return 0;
}

/**
*用户模式下子获取命令字符串\n
*函数在窗体接收到GETCOMMAND命令时触发,该消息由GetLine发送，写入地址封装在wparam中,\n
*函数将command的内容copy到目的字符串*/
LRESULT  CmdView::OnGetCommand(WPARAM wparam, LPARAM lparam)
{
	/**
	*@brief 解析拷贝command的参数目标地址
	*/
	CString *pmyline = (CString *)wparam;
	/**
	*@brief 拷贝command到目标地址
	*/
	*pmyline = command;
	return 0;
}
/**
*用户模式下子获取输入状态\n
*函数在窗体接收到GETLINESTATE命令时触发,该消息由GetLine发送，写入地址封装在wparam中,\n
*函数将输入状态写入到指定的地址返回*/
LRESULT  CmdView::OnGetLineState(WPARAM wparam, LPARAM lparam){
	/**
	*@brief 解析拷贝输入状态的参数目标地址
	*/
	LINESTATE *mylinestate = (LINESTATE *)wparam;
	/**
	*@brief 拷贝输入状态到目标地址
	*/
	*mylinestate = LINEFLAG;
	return 0;
}
/**
*用户模式下子获取CmdView命令状态状态\n
*函数在窗体接收到GETCMDSTATE命令时触发, 该消息由GetLine发送，写入地址封装在wparam中, \n
*函数将CmdView状态写入到指定的地址返回。GetLine通过该函数判断获取输入状态是否合法*/
LRESULT  CmdView::OnGetCmdState(WPARAM wparam, LPARAM lparam){
	/**
	*@brief 解析拷贝CmdView状态的参数目标地址
	*/
    CMDMODE *mymode = (CMDMODE *)wparam;
	/**
	*@brief 拷贝CmdView状态到目标地址
	*/
	*mymode = CMDFLAG;
	return 0;
}
