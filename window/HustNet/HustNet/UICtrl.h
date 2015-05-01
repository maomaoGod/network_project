#pragma once
#include "stdafx.h"
/**
*UICtrl主要实现了从界面的输入输出功能，为UserCmd类中自定义程序提供输入输出接口。自定义程序通过调用UICtrl的全局函数
*实现在框架界面的输入输出，通过UICtrl可以打印文字到LogView,RespondView窗口，可以清空LogView和RespondView窗口内容，
*可以在用户命令模式下从CmdView中获取一行输入。UICtrl提供接管CmdView窗口的功能，也可以设置命令提示语。UICtrl并不直
*接操作窗口，而是通过向指定窗口发送命令的方式通知对应窗口完成。
*/
typedef enum   { OLDLINE, NEWLINE }  LINESTATE;
typedef enum   { THREAD_EXIT, THREAD_WAIT, THREAD_RUN }  STATE;
typedef enum   { ROOT, USER } CMDMODE;

/**
*@brief 打印一行文字到日志窗口
* @param log 需要打印的日志字符串
*/
void PrintLog(CString log);
/**
*@brief 打印一行文字到响应窗口
*@param rp 需要打印的字符串
*/
void PrintRp(CString rp);
/**
*@brief 清空响应窗口
*函数发送CLEAN消息到LogView窗口，RespondView窗口接收到该消息后打印相应字符串到LogView窗口
*/
void CleanRp(CString e);
/**
*@brief 清空日志窗口\n
*/
void CleanLog(CString e);
/**
*@brief 设置相应窗口的内容
*@param e 需要设置的字符串
*/
void SetRp(CString e);
/**
*Brief 设置CmdView的命令提示语
*@param e 需要设置的命令提示语
*/
void SetHint(CString e);
/**
*@brief 设置CmdView的命令提示语，接管CmdView的输入
*param e 需要设置的命令提示语
*/
void TakeOverCmd(CString e);
/**
*@brief 从CmdView中获取一行输入
*@retval null 不在用户命令模式
*/
CString  GetLine();
/**
*@brief 通知主窗口命令已获取完成*/
void EndInput();

void SendOut(CString);
