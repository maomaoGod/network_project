/**
*LogView.cpp : 实现文件
*/
#include "stdafx.h"
#include "HustNet.h"
#include "LogView.h"

// LogView
IMPLEMENT_DYNCREATE(LogView, CEditView)


/**
* @author ACM2012
* @note    LogView构造函数
*/
LogView::LogView()
{
	Log.Empty();  
	logcount = 0;
}

/**
* @author ACM2012
* @note    LogView析构函数
*/
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
* @author ACM2012
* @param [in] pDC设备句柄
* @note  获取文档类指针
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
* @author ACM2012
* @note  LogView 初始化函数在视图附加到文档后触发，主要完成界面的初始化工作
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
* @author ACM2012
* @param cs  窗体创建风格结构体
* @note  本函数在即将创建窗体前触发，函数主要通过传递进来的结构引用变量来设置相应的窗体风格,
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
* @author ACM2012
* @param point 鼠标点击点坐标
* @note  屏蔽鼠标单击本函数在鼠标单击时触发，函数通过单击选中文本可以修改相应的文本，
*本函数屏蔽该消息，防止修改日志历史
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
*@author ACM2012
*@param point 鼠标点击点坐标
*@note 本函数在鼠标双击时触发，屏蔽鼠标双击，函数通过双击选中文本可以修改相应的文本，
*本函数屏蔽该消息，防止修改日志历史
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
*@author ACM2012
*@note 屏蔽按键输入本函数在非系统键被按下时触发。LogView仅显示日志历史，通过
*屏蔽按键消息使得不允许用户直接输入改变日志历史
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
*@author ACM2012
*@param  wparam  需要打印的字符串  \n
*@note 在LogView打印一行文字输出函数在窗口接收到PRINT消息时被触发。需要打印的字符串参数
*为wparam，函数自动添加记录标号自动添加换行符，并将输入焦点设置为尾部，实现滚动条随着字体
*的增加而向下移动
*/
LRESULT LogView::Print(WPARAM wparam, LPARAM lparam)
{
	CString mystr = *((CString *)wparam);
	CString temp;
	temp.Format(_T("log%d: %s \r\n"), logcount++, mystr);
	Log += temp;
	SetWindowText(Log);
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l, l, false);
	((CEdit *)this)->SetFocus();
	return 0;
}

/**
*@author ACM2012
*@param  wparam  需要打印的字符串  \n
*@note*清空日志窗口函数在LogView接收到CLEAN消息时被触发。函数清空日志记录，
*并重置日志标号，同时设置焦点，以使得滚动条回到起始位置，保证滚动条和文本的一致
*/
LRESULT LogView::Clean(WPARAM wparam, LPARAM lparam)
{
	Log.Empty();
	SetWindowText(Log);
	logcount = 0;
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l, l, false);
	((CEdit *)this)->SetFocus();
	return 0;
}