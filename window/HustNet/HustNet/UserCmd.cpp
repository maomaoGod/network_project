#include "stdafx.h"
#include "UserCmd.h"
#include "Tools.h"
#include "UICtrl.h"
#include "NetHtml.h"
#include "NetWork.h"
#include "Cmysocket.h"

using namespace Tools;
using namespace NetWork;

UserCmd::UserCmd()
{
}


UserCmd::~UserCmd()
{
}

/**
*通过动态链接库实现加法运算\n
*函数通过动态链接库实现加法运算。函数首先对算数公式进行解析，获取两个加数，\n
*然后从动态链接库中加载加法函数，并调用该函数进行计算，将公式和计算结果打印到\n
*RespondView窗口
* @param e  运算公式字符串
*/
void UserCmd::Compute(CString e){
	int pos;
	CString a, b, result;
	int c1, c2;
	pos = e.Find(_T('+'));
	a.Format(_T("%s"), e.Mid(0, pos));
	b.Format(_T("%s"), e.Mid(pos));
	c1 = _wtoi(a);
	c2 = _wtoi(b);
	typedef int(*ADDPROC)(int, int);
	ADDPROC Add = (ADDPROC)mydll.GetFun("add");
	if (Add == NULL){
		PrintLog(_T("未加载动态链接库"));
		return;
	}
	result.Format(_T("%d+%d=%d"), c1, c2, Add(c1, c2));
	PrintRp(result);
}

/*void UserCmd::Test(CString e){
	CString mystr;
	TakeOverCmd(_T("myTest>"));
	while ((mystr =GetLine()).Compare(_T("exit")) != 0)
		PrintRp(mystr);
}*/

/**
*实现和服务器的TCP通信\n
*函数实现和服务器的通信，客户端发送数据到服务器，服务器响应并返回响应报文。\n
*函数首先接管cmd,通过调用CSocket和服务器建立连接，绑定端口固定为6500。\n
*获取一行输入，发送该字符串到服务器，并接收从服务返回的信息，并打印到RespondView\n
*窗口；若获取的输入为exit，则函数跳出循环，并关闭连接*/
void  UserCmd::Connect(CString e){
	AfxSocketInit();
	/**
	*创建 CSocket 对象
	*/
	CSocket aSocket;

	CString strIP;
	strIP.Format(_T("%s"), _T("127.0.0.1"));
	if (!aSocket.Create())
	{
		CString error;
		error.Format(_T("创建失败:%d"), aSocket.GetLastError());
		PrintLog(error);
		return;
	}
	/**
	*转换需要连接的端口内容类型
	*/
	PrintLog(_T("套接字创建成功"));
	/**
	*连接指定的地址和端口
	*/
	if (aSocket.Connect(strIP, 6500))
	{
		PrintLog(_T("连接服务器成功"));
		TCHAR szRecValue[1024] = { 0 };
		CString  sed, rev;
		int times = 0;
		TakeOverCmd(_T("client>: "));
		aSocket.Receive((void *)szRecValue, 1024);
		rev.Format(_T("来自服务器的消息:%s"), szRecValue);
		PrintRp(rev);
		while ((sed = GetLine()).Compare(_T("exit")) != 0){
			aSocket.Send(sed, sed.GetLength()*sizeof(TCHAR));
			memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			aSocket.Receive((void *)szRecValue, 1024);
			rev.Format(_T("来自服务器的消息:%s"), szRecValue);
			PrintRp(rev);
		}
	}
	else{
		CString error;
		error.Format(_T("连接服务器失败:%d"), aSocket.GetLastError());
		PrintLog(error);
	}
	aSocket.Close();
}

void UserCmd::Http(CString e)
{
	//HttpSend Http;
	//Http.Begin();
	AppLayerHttp html;
	html.Begin();
}

void UserCmd::Chat(CString e){
	ChatWork chat;
	chat.Begin();
}

/**
* 函数通过路径加载Dll\n
* 函数通过动态链接库路径尝试加载指定位置的动态链接库，若加载失败，将失败信息打印到状态输出窗口
* @param 动态链接库路径
*/
void UserCmd::SetDll(CString e){
	if (mydll.LoadDll(e) == FALSE){
	    PrintLog(_T("未找到指定的动态链接库"));
		return;
	}
}

/**
*函数通过url获取网页html的字符流\n
*函数通过url获取网页html的字符流，并将其显示到响应输出窗口
* @param url  请求网页html的统一资源定义符
*/
void UserCmd::GetHtml(CString url){
	NetHtml myhtml;
	CString html = myhtml.getURLContext(url);
	SetRp(html);
}

/**
*清空响应窗口
*/
void UserCmd::cleanrp(CString e){
	CleanRp(NULL);
}

/**
*清空状态窗口
*/
void UserCmd::cleanlog(CString e){
    CleanLog(NULL);
}

void UserCmd::Visit(CString e){
	if (e.Compare(_T("http")) == 0){
		AppLayerHttp myhttp;
		myhttp.Begin();
	}
}

void UserCmd::sendout(CString e){
	SendOut(e);
}

void UserCmd::mytest(CString e){
	CString sed;
	TakeOverCmd(_T("Test>"));
	Cmysocket mysock;
	mysock.Connect(_T("127.0.0.1"),6500);
	while ((sed = GetLine()).Compare(_T("exit")) != 0){
		mysock.Send(sed,sed.GetLength()*sizeof(TCHAR));
	}
}