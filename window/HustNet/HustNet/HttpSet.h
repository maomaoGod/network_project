/** @file
* @brief Http方法集
* @author ACM2012
* @date 2015/04/19
* @version 0.1
* @note
* 该文件声明Http类，该类封装了所实现的Http请求和响应方法.
* HTTP请求报文格式： (<sp>为空格, " "并不代表一个合法的空格)
* Method <sp> URL <sp> HttpVersion <crlf>
* Header0: <sp> HeaderValue <crlf>
* ...
* HeaderN: <sp> HeaderValue <crlf>
* <crlf> (which is important)
* EntityBody (empty for GET, something for POST)
* HTTP响应报文格式： (<sp>为空格, " "并不代表一个合法的空格)
* HttpVersion <sp> StateCode <sp> StateHint <crlf>
* Header0: <sp> HeaderValue <crlf>
* ...
* HeaderN: <sp> HeaderValue <crlf>
* <crlf> (which is important)
* EntityBody (empty for GET, something for POST)
* @remark
* 该文件中的Http类尚未完全实现.
*/
#pragma once
#include "afxinet.h"

/**
* @class
* @brief 该类封装Http协议方法.
* @author ACM2012
* @note
* 此类包含该类的构造函数和析构函数，将实现Http协议的响应和请求.
*/
class HttpSet
{
public:
	/** @brief 构造函数*/
	HttpSet();
	/** @brief 析构函数*/
	~HttpSet();
	/** @brief 根据URL和方法发送请求报文*/
	CString HttpSet::request(LPCTSTR pstrURL, string method);
	/** @brief 根据IP地址和方法发送响应报文*/
	CString HttpSet::response(LPCTSTR pstrURL, string method);
};

