/**
*LogView.cpp : 实现文件
*/
#include "stdafx.h"
#include "HustNet.h"
#include "LogView.h"

// LogView
IMPLEMENT_DYNCREATE(LogView, CEditView)


/**
*LogView构造函数\n
*构造函数创建类对象，并进行简单的初始化工作。函数初始化窗体记录字符串为空，并将记录标号设为0，\n
*表示记录从0开始计数
*/
LogView::LogView()
{
	Log.Empty();  
	logcount = 0;
}

LogView::~LogView()
{
}

/**
*实现消息和窗口类的映射关系
*/
BEGIN_MESSAGE_MAP(LogView, CEditView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_MESSAGE(PRINT, Print)
	ON_MESSAGE(CLEAN, Clean)
END_MESSAGE_MAP()


/**
*LogView 绘图
*/

void LogView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	/**
	*TODO:  在此添加绘制代码
	*/
}


/**
*LogView 诊断
*/

#ifdef _DEBUG
void LogView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void LogView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


/**
*LogView 初始化\n
*函数在视图附加到文档后触发，主要完成界面的初始化工作\n
*函数实现了字体的设置，本视图的字体设置为“Times New Roman”；函数在命令输入框内插入
*/
void LogView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	/**
	*TODO:  在此添加专用代码和/或调用基类
	*/
	CClientDC dc(this);
	static CFont  myfont;
	myfont.CreatePointFont(120, (LPCTSTR)_T("Times New Roman"));
	((CEdit *)this)->SetFont(&myfont);
}

/**
*param cs  窗体创建风格结构体\n
*CmdView框体的属性设置\n
*本函数在即将创建窗体前触发，函数主要通过传递进来的结构引用变量来设置相应的窗体风格,\n
*本窗体的风格为拥有垂直滚动条，并实现自动换行，自动分解长字符串为多行，光标为不隐藏状态
*/
BOOL LogView::PreCreateWindow(CREATESTRUCT& cs)
{
	/**
	*TODO:  在此添加专用代码和/或调用基类
	*/
	m_dwDefaultStyle = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL;
	return CCtrlView::PreCreateWindow(cs);
}

/**
*屏蔽鼠标单击\n
*本函数在鼠标单击时触发，函数通过单击选中文本可以修改相应的文本，本函数屏蔽该消息，防止修改日志历史
*/
void LogView::OnLButtonDown(UINT nFlags, CPoint point)
{
	/**
	*TODO:  在此添加消息处理程序代码和/或调用默认值\n
	*return;
	*/
	CEditView::OnLButtonDown(nFlags, point);
}

/**
*屏蔽鼠标双击\n
*本函数在鼠标双击时触发，函数通过双击选中文本可以修改相应的文本，本函数屏蔽该消息，防止修改日志历史
*/
void LogView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	/**
	*TODO:  在此添加消息处理程序代码和/或调用默认值
	*/
	return;
	//CEditView::OnLButtonDblClk(nFlags, point);
}

/**
*屏蔽按键输入\n
*本函数在非系统键被按下时触发。LogView仅显示日志历史，通过屏蔽按键消息使得不允许用户直接输入改变日志历史
*/
void LogView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	/**
	*TODO:  在此添加消息处理程序代码和/或调用默认值
	*/
	return;
	//CEditView::OnChar(nChar, nRepCnt, nFlags);
}

/**
*@param  wparam  需要打印的字符串  \n
*在LogView打印一行文字输出\n
*函数在窗口接收到PRINT消息时被触发。需要打印的字符串参数为wparam，函数自动添加记录标号
*自动添加换行符，并将输入焦点设置为尾部，实现滚动条随着字体的增加而向下移动
*/
LRESULT LogView::Print(WPARAM wparam, LPARAM lparam)
{
	CString mystr = *((CString *)wparam);
	CString temp;
	/**
	*@brief 添加提示语和记录标号，添加换行符到尾部
	*/
	temp.Format(_T("log%d: %s \r\n"), logcount++, mystr);
	/**
	*@brief 将处理后的字符串加到日志记录尾部
	*/
	Log += temp;
	/**
	*@brief 更新窗口显示
	*/
	SetWindowText(Log);
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l, l, false);
	((CEdit *)this)->SetFocus();
	return 0;
}
/**
*清空日志窗口\n
*函数在LogView接收到CLEAN消息时被触发。函数清空日志记录，并重置日志标号，\n
*同时设置焦点，以使得滚动条回到起始位置，保证滚动条和文本的一致
*/
LRESULT LogView::Clean(WPARAM wparam, LPARAM lparam)
{
	/**
	*@brief 清空日志
	*/
	Log.Empty();
	/**
	*@brief 更新窗口内容
	*/
	SetWindowText(Log);
	logcount = 0;
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l, l, false);
	((CEdit *)this)->SetFocus();
	return 0;
}