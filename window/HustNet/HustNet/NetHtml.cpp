/** @file
* @brief 根据URL获取HTML内容.
* @author ACM2012
* @date 2015/04/12
* @version 0.1
* @note
* 此文件包含对类NetHtml中函数的实现，其中
* 包括构造函数、析构函数和HTML获取函数.
* HTTP请求报文格式： (<sp>为空格, " "并不代表一个合法的空格)
* Method <sp> URL <sp> HttpVersion <crlf>
* Header0: <sp> HeaderValue <crlf>
* ...
* HeaderN: <sp> HeaderValue <crlf>
* <crlf> (which is important)
* EntityBody (empty for GET, something for POST)
* @remark
* 此文件将来可能需要修改，以便扩充其他内容；也有可能并入HTTPSet.cpp
*/

#include "stdafx.h"
#include "NetHtml.h"
#include "Tools.h"
using namespace Tools;

#define MAXLEN 1024

extern void PrintLog(CString);

/**
* @author ACM2012
* @note
* 构造函数，以便扩展
* @since 0.1
*/
NetHtml::NetHtml()
{

}

/**
* @author ACM2012
* @note
* 析构函数，以便扩展
* @since 0.1
*/
NetHtml::~NetHtml()
{

}

/**
* @author ACM2012
* @param[in] pstrURL 所给URL，LPCTSTR类型
* @return 获取的HTML内容（目前为获取的整个响应报文），Unicode编码，CString类型
* @note
* 对于所给的URL，该函数将其划分为两个部分：
* 第一个部分为域名，该域名指向承载需要获取的HTML文件的服务器；
* 第二个部分为路径+查询串+片段符，其中路径为域名下服务器的文件系统路径，查询串为
* 提交服务器后台进行解析的串，用于动态地返回用户所需求的内容，片段符用于描述用户
* 需求或当前所处于所有资源中的位置，有时被包含在查询串中；
* 若第二部分为空，则设为默认“/”，表示当前目录下.
* 接下来根据域名与服务器之间创立链接，之后使用GET方法，根据上述的第二部分获取
* HTML内容.
* @remark
* 最初使用了内部控件，目前改用CSocket实现，将来预计使用本项目重构的Socket类实现.
* @since 0.1
*/
CString NetHtml::getURLContext(LPCTSTR pstrURL){
	/*CString myhtml;
	CInternetSession mySession(NULL, 0);
	CHttpFile* htmlFile = NULL;
	TCHAR src[1024];
	try{
	PrintLog(_T("正在连接!"));
	htmlFile = (CHttpFile*)mySession.OpenURL(pstrURL);
	PrintLog(_T("正在取回本地!"));
	while (htmlFile->ReadString(src, 1024)){
	
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, NULL, 0);
	TCHAR *pBuffer = new TCHAR[nBufferSize + 1];
	memset(pBuffer, 0, (nBufferSize + 1)*sizeof(TCHAR));
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, pBuffer, nBufferSize*sizeof(TCHAR));//UTF-8转Unicode
	myhtml += pBuffer;
	myhtml += _T("\r\n");
	delete pBuffer;
	}
	htmlFile->Close();
	mySession.Close(); //关闭会话
	}
	catch (CException *e) {  //异常处理
		PrintLog(_T("获取数据失败!"));
	} 
	return myhtml;*/
	/** @brief 初始化Socket*/
	AfxSocketInit();
	CString myhtml;
	/** @brief 声明一个Socket*/
	CSocket mySocket;
	CString tempURL = pstrURL;
	string pHost;
	string GET;
	/** @brief 在URL中找到第一个斜杠，用于分开两个部分*/
	int i = tempURL.Find(_T('/'), 0);
	if (i == -1)
	{
		/** @brief 第二部分为空*/
		i = tempURL.GetLength();
		GET = "/";
	}
	else
	{
		/** @brief 获取第二部分并转码*/
		CString tempGET = tempURL.Mid(i);
		GET = Tstr::Tchar2string(tempGET.GetBuffer());
	}
	/** @brief 获取域名，即第一部分，并转码*/
	CString tempHOST = tempURL.Mid(0, i);
	pHost = Tstr::Tchar2string(tempHOST.GetBuffer());
	LPCWSTR host_name = Tstr::String2LPCWSTR(pHost);
	string header = "";
	char buf[MAXLEN];
	TCHAR TCHAR_buf[MAXLEN];
	try{
		/** @brief 创建一个Socket*/
		mySocket.Create();
		/** @brief 显式地连接到服务器的80端口*/
		mySocket.Connect(host_name, 80);	// 连接80端口
		/** @brief 组装报文头*/
		header = "GET " + GET + " HTTP/1.1\nHOST: " + pHost + "\n\n";
		/** @brief 报文头转码*/
		char *temp = Tstr::S2Cstar(header);
		/** @brief 发送组装好的请求报文*/
		mySocket.Send(temp, header.length() + 1, 0);
		/** @brief 关闭到服务器的连接，否则长等待直到被动丢失连接*/
		mySocket.Send("Connection: Close\n\n", 20, 0);
		/** @brief 释放转码所用的字符串的空间*/
		free(temp);
		/** @brief 接收返回的响应报文并转码*/
		while (mySocket.Receive(buf, MAXLEN, 0) > 0)
		{
			int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buf, -1, NULL, 0);
			TCHAR *pBuffer = new TCHAR[nBufferSize + 1];
			memset(pBuffer, 0, (nBufferSize + 1)*sizeof(TCHAR));
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buf, -1, pBuffer, nBufferSize*sizeof(TCHAR));//UTF-8转Unicode
			myhtml += pBuffer;
		}
	}
	catch (CException *e)
	{
		/** @brief 异常处理为打印Log信息*/
		PrintLog(_T("获取数据失败!"));
	}
	/** @brief 返回所获取的Html内容*/
	return myhtml;
};
