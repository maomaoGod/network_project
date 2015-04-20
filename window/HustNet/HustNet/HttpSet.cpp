/** @file
* @brief Http方法集
* @author ACM2012
* @date 2015/04/19
* @version 0.1
* @note
* 该文件实现Http类所封装的Http请求和响应方法.
* HTTP请求报文格式： (<sp>为空格, " "并不代表一个合法的空格)
* Method <sp> URL <sp> HttpVersion <crlf>
* Header0: <sp> HeaderValue <crlf>
* ...
* HeaderN: <sp> HeaderValue <crlf>
* <crlf> (which is important)
* EntityBody (empty for PATH, something for POST)
* HTTP响应报文格式： (<sp>为空格, " "并不代表一个合法的空格)
* HttpVersion <sp> StateCode <sp> StateHint <crlf>
* Header0: <sp> HeaderValue <crlf>
* ...
* HeaderN: <sp> HeaderValue <crlf>
* <crlf> (which is important)
* EntityBody (empty for PATH, something for POST)
* @remark
* 该文件中的Http类尚未完全实现.
*/

#include "stdafx.h"
#include "HttpSet.h"
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
HttpSet::HttpSet()
{

}

/**
* @author ACM2012
* @note
* 析构函数，以便扩展
* @since 0.1
*/
HttpSet::~HttpSet()
{

}

/**
* @author ACM2012
* @param[in] pstrURL 所给URL，LPCTSTR类型
* @param[in] method 使用的Http方法，string类型
* @param[in] content 报文内容，string类型
* @return 获取的报文内容
* @note
* 对于所给的URL，该函数将其划分为两个部分：
* 第一个部分为域名，该域名指向承载需要获取的HTML文件的服务器；
* 第二个部分为路径+查询串+片段符，其中路径为域名下服务器的文件系统路径，查询串为
* 提交服务器后台进行解析的串，用于动态地返回用户所需求的内容，片段符用于描述用户
* 需求或当前所处于所有资源中的位置，有时被包含在查询串中；
* 若第二部分为空，则设为默认“/”，表示当前目录下.
* 接下来根据域名与服务器之间创立链接，之后使用method所给的方法，组装报文并发送.
* 发送后断开连接，否则长等待直到被动丢失连接；
* 获取响应报文内容.
* @remark
* 该函数可能需要重写.
* @since 0.1
*/
CString HttpSet::request(LPCTSTR pstrURL, string method, string content)
{
	/** @brief 初始化Socket*/
	AfxSocketInit();
	CString myhtml;
	/** @brief 声明一个Socket*/
	CSocket mySocket;
	CString tempURL = pstrURL;
	string pHost;
	string PATH;
	/** @brief 在URL中找到第一个斜杠，用于分开两个部分*/
	int i = tempURL.Find(_T('/'), 0);
	if (i == -1)
	{
		/** @brief 第二部分为空*/
		i = tempURL.GetLength();
		PATH = "/";
	}
	else
	{
		/** @brief 获取第二部分并转码*/
		CString tempPATH = tempURL.Mid(i);
		PATH = Tstr::Tchar2string(tempPATH.GetBuffer());
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
		header = method + " " + PATH + " HTTP/1.1\nHOST: " + pHost + "\n" + content + "\n";
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
}

/**
* @author ACM2012
* @param[in] pstrURL 所给URL，LPCTSTR类型
* @param[in] method 使用的Http方法，string类型
* @param[in] content 报文内容，string类型
* @return 获取的报文内容
* @note
* 对于所给的URL，该函数将其划分为两个部分：
* 第一个部分为域名，该域名指向承载需要获取的HTML文件的服务器；
* 第二个部分为路径+查询串+片段符，其中路径为域名下服务器的文件系统路径，查询串为
* 提交服务器后台进行解析的串，用于动态地返回用户所需求的内容，片段符用于描述用户
* 需求或当前所处于所有资源中的位置，有时被包含在查询串中；
* 若第二部分为空，则设为默认“/”，表示当前目录下.
* 接下来根据域名与服务器之间创立链接，之后使用method所给的方法，组装报文并发送.
* 发送后断开连接，否则长等待直到被动丢失连接；
* 无需获取响应报文内容.
* @remark
* 该函数可能需要重写.
* @since 0.1
*/
HttpSet::response(LPCTSTR pstrURL, string method, string content)
{
	/** @brief 初始化Socket*/
	AfxSocketInit();
	/** @brief 声明一个Socket*/
	CSocket mySocket;
	CString tempURL = pstrURL;
	string pHost;
	string PATH;
	/** @brief 在URL中找到第一个斜杠，用于分开两个部分*/
	int i = tempURL.Find(_T('/'), 0);
	if (i == -1)
	{
		/** @brief 第二部分为空*/
		i = tempURL.GetLength();
		PATH = "/";
	}
	else
	{
		/** @brief 获取第二部分并转码*/
		CString tempPATH = tempURL.Mid(i);
		PATH = Tstr::Tchar2string(tempPATH.GetBuffer());
	}
	/** @brief 获取域名，即第一部分，并转码*/
	CString tempHOST = tempURL.Mid(0, i);
	pHost = Tstr::Tchar2string(tempHOST.GetBuffer());
	LPCWSTR host_name = Tstr::String2LPCWSTR(pHost);
	string header = "";
	try{
		/** @brief 创建一个Socket*/
		mySocket.Create();
		/** @brief 显式地连接到服务器的80端口*/
		mySocket.Connect(host_name, 80);	// 连接80端口
		/** @brief 组装报文头*/
		header = method + " " + PATH + " HTTP/1.1\nHOST: " + pHost + "\n" + content + "\n";
		/** @brief 报文头转码*/
		char *temp = Tstr::S2Cstar(header);
		/** @brief 发送组装好的请求报文*/
		mySocket.Send(temp, header.length() + 1, 0);
		/** @brief 关闭到服务器的连接，否则长等待直到被动丢失连接*/
		mySocket.Send("Connection: Close\n\n", 20, 0);
		/** @brief 释放转码所用的字符串的空间*/
		free(temp);
	}
	catch (CException *e)
	{
		/** @brief 异常处理为打印Log信息*/
		PrintLog(_T("获取数据失败!"));
	}
}
