#pragma once

/**
*RespondView位于主界面的右侧，继承自CEditView类，主要负责响应信息的输出。该窗口只能从函数内部打印输出，不接受外\n
*界输入；在窗口内部进行单击和双击操作将被屏蔽。该类提供对外提供三个操作，输出一行字符串，设置RespondView内容，以\n
*及清空窗口。这三个操作均由外部消息触发，消息ID为PRINT，SETTEXT和CLEAN，分别触发Print，SetText和Clean函数，即当\n
*外界需要打印状态信息或者清空Logview时，只需要发送对应的消息即可。打印的字符串指针以消息参数wparam的形式传递进来,\n
*RespondView会自动头部添加提示信息，而SETTEXT则完全由打印的内容决定，不会添加头部提示信息。
*/

class RespondView : public CEditView
{
	DECLARE_DYNCREATE(RespondView)

protected:
	/**
	*@brief 动态创建所使用的受保护的构造函数
	*/
	RespondView();
	virtual ~RespondView();
public:
	CString  Res;
public:
	/**
	*重写以绘制该视图
	*/
	virtual void OnDraw(CDC* pDC);
#ifdef _DEBUG
	/**
	*@brief RespondView 诊断
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
	*@brief 在RespondView打印一行文字输出\n
	* @param  wparam  需要打印的字符串
	*/
	afx_msg LRESULT Print(WPARAM wparam, LPARAM lparam);
	/**
	*@brief 清空日志窗口
	*/
	afx_msg LRESULT Clean(WPARAM wparam, LPARAM lparam);
	/**
	*@brief 设置响应窗口内容
	* @param  wparam  设置的窗口内容字符串
	*/
	afx_msg LRESULT SetText(WPARAM wparam, LPARAM lparam);
public:
	/**
	*@brief RespondView 消息处理程序
	*/
	virtual void OnInitialUpdate();
	/**
	* @param cs  窗体创建风格结构体
	* @brief RespondView框体的属性设置\n
	*/
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	/**
	*@brief 屏蔽鼠标双击\n
	*/
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	/**
	*@brief 屏蔽按键输入\n
	*/
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	/**
	*@brief 屏蔽鼠标单击
	*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


