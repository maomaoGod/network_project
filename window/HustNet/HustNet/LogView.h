#pragma once
/**@file
*date 2015.4.20
*@version <0.1>
*@brief LogView 视图
*/

/**
*LogView位于主界面的左下角，继承自CEditView类，主要负责状态信息的输出。该窗口只能从函数内部打印输出，不接受外
*界输入；在窗口内部进行单击和双击操作将被屏蔽。该类提供对外提供两个操作，输出一行字符串和清空窗口。这两个操作
*均由外部消息触发，消息ID为PRINT和CLEAN，分别触发Print和Clean函数，即当外界需要打印状态信息或者清空Logview时，
只需要发送对应的消息即可。打印的字符串指针以消息参数wparam的形式传递进来。Logview会对打印的状态信息进行计数，
在清空时计数器同时也会被清零。
*/

class LogView : public CEditView
{
	DECLARE_DYNCREATE(LogView)

protected:
	/**
	*动态创建所使用的受保护的构造函数
	*/
	LogView();
	virtual ~LogView();
	int logcount;
public:
	CString  Log;
public:
	/**
	*重写以绘制该视图
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
	/**
	*@param  wparam  需要打印的字符串
	*@brief 在LogView打印一行文字输出
	*/
	afx_msg LRESULT Print(WPARAM wparam, LPARAM lparam);
	/**
	*@brief 清空日志窗口
	*/
	afx_msg LRESULT Clean(WPARAM wparam, LPARAM lparam);
public:
	/**
	*@brief LogView 初始化
	*/
	virtual void    OnInitialUpdate();
	/**
	*param cs  窗体创建风格结构体
	*@brief CmdView框体的属性设置
	*/
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	/**
	*@brief 屏蔽鼠标单击
	*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	/**
	*@brief 屏蔽鼠标双击
	*/
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	/**
	*屏蔽按键输入\n
	*/
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};



