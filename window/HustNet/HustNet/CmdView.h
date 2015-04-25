/**
 *CmdView位于主界面左上角,主要完成命令的输入(界面控制),命令解析和命令映射(功能映射)。\n
 *该部分工作由两各线程共同完成，\n
 *主线程完成界面维护控制工作,界面控制方面包括控制cmdview的输入响应,控制光标以及鼠标响应以防止随意修改提示和命令输入\n
 *历史,并提供接管命令行的功能,即UserCmd命令集合中的命令可以获取相应的子命令输入(如进入Http会话模式后获取Get命令输入);\n
 *子线程在接收到主线程的命令解析完成信号后,调用命令集合中相应的命令处理函数,在本程序中,使用了简单的map映射机制,即把\n
 *命令字符串映射到命令的处理函数指针,通过指针调用相应的命令处理函数,在命令完成后,通知主线程,主线程完成相应的处理，如\n
 *输出提示信息字符串。CmdView的工作模式有2种,一种为普通命令模式，即一次完成一条命令，命令的解析和映射由框架完成；\n
 *另一种为用户接管模式,UserCmd中的命令接管CmdView后,可以获取其输入,从而调用子命令,子命令的解析工作由用户命令自行完成
 */

#pragma once
/**
 *CmdView 视图*/
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
	*动态创建所使用的受保护的构造函数
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
	*实现从命令字符串到命令处理函数的映射结构
	*/
	map<CString, CMDPROC> my_map; 
	/**
	 *CmdView字体信息结构
	 */
	TEXTMETRIC tm; 
	/**
	*CmdView显示的字体对象
	*/
	CFont   myfont; 
	/**
	*CmdView的文本框
	*/
	CEdit  *myedit;
	/**
	*命令提示语
	*/
	CString Hint;
	/** 
	*命令提示语屏显示长度
	*/
	int HintPLen;
	/**
	*命令提示语存储长度*/
	int HintSLen; 
	/**
	*命令提示标记行行号
	*/
	int HintLine;     
protected:
	DECLARE_MESSAGE_MAP()
public:
	/**
	*@brief 函数在按下非系统键时触发,在OnKeyDown之后,OnKeyUp之前。
	*/
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	/**
	*@brief 函数在视图附加到文档后触发,主要完成界面的初始化工作
	*/
	virtual void OnInitialUpdate();
	//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	/**
	*@brief CmdView框体的属性设置
	*/
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	/**
	*@brief 屏蔽鼠标单击\n
	*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	/**
	*屏蔽鼠标单击\n
	*/
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	/**
	*@brief 限制光标移动\n
	*/
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	/**
	*@brief 窗口大小改变时重新确定提示行行号\n
	*/
	afx_msg void OnSize(UINT nType, int cx, int cy);
	/**
	*@brief 保持拥有输入焦点
	*/
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	/**
	*@brief 设置命令提示语\n
	* @param wparam 接管后的命令提示语\n
	*/
	afx_msg  LRESULT  OnSetHint(WPARAM wparam, LPARAM lparam);
	/**
	*@brief 接管命令行输入窗口\n
	*@param   wparam  接管后的命令提示语\n
	*/
	afx_msg  LRESULT  OnTakeOverCmd(WPARAM wparam, LPARAM lparam);
	/**
	*@brief 用户模式下子命令已处理完成\n
	*/
	afx_msg  LRESULT  OnEndInput(WPARAM wparam, LPARAM lparam);
	/**
	*@brief 获取命令字符串\n
	*@param   wparam  复制command的接收地址\n
	*/
	afx_msg  LRESULT  OnGetCommand(WPARAM wparam, LPARAM lparam);
	/**
	*@brief 获取输入状态
	*@param   wparam  复制LINEFLAG的接收地址\n
	*/
	afx_msg  LRESULT  OnGetLineState(WPARAM wparam, LPARAM lparam);
	/**
	*@brief 获取命令输入行状态
	*@param   wparam  复制CMDSTATE的接收地址\n
	*/
	afx_msg  LRESULT  OnGetCmdState(WPARAM wparam, LPARAM lparam);
	/**
	*@brief 实现命令字符串到命令处理函数之间的映射
	*/
	void  MapTask();
	/**
	*@brief 监听命令处理状态
	*/
	static DWORD WINAPI DEALCMD(LPVOID lParam);
protected:
	/**
	*@brief 处理回车键消息,触发命令处理函数
	*/
	void DealEnter();
	/**
	*@brief 创建消息处理线程,注册消息映射
	*/
	void Initialcmd();
	/**
	*@brief 命令已处理完成\n
	*/
	void DealCmdOver();
};