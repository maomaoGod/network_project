/** @file
* @brief 根据URL获取HTML内容.
* @author ACM2012
* @date 2015/04/12
* @version 0.1
* @note
* 此文件包含对类NetHtml的声明，该类封装用于获取HTML内容的函数.
* HTTP请求报文格式： (<sp>为空格, " "并不代表一个合法的空格)
* Method <sp> URL <sp> HttpVersion <crlf>
* Header0: <sp> HeaderValue <crlf>
* ...
* HeaderN: <sp> HeaderValue <crlf>
* <crlf> (which is important)
* EntityBody (empty for GET, something for POST)
* @remark
* 此文件将来可能需要修改，以便扩充其他内容；也有可能并入HTTPSet.h
*/
#pragma once
#include "afxinet.h"

/**
* @class
* @brief 该类封装用于获取HTML内容的函数.
* @author ACM2012
* @note
* 此类包含该类的构造函数和析构函数，实现了getURLContext函数用于
* 获取HTML内容.
*/
class NetHtml
{
public:
	/** @brief 构造函数*/
	NetHtml();
	/** @brief 析构函数*/
	~NetHtml();
	/** @brief 根据URL获取HTML内容*/
	CString NetHtml::getURLContext(LPCTSTR pstrURL);
};

