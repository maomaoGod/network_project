/** @file
* @brief Http������
* @author ACM2012
* @date 2015/04/19
* @version 0.1
* @note
* ���ļ�����Http�࣬�����װ����ʵ�ֵ�Http�������Ӧ����.
* HTTP�����ĸ�ʽ�� (<sp>Ϊ�ո�, " "��������һ���Ϸ��Ŀո�)
* Method <sp> URL <sp> HttpVersion <crlf>
* Header0: <sp> HeaderValue <crlf>
* ...
* HeaderN: <sp> HeaderValue <crlf>
* <crlf> (which is important)
* EntityBody (empty for GET, something for POST)
* HTTP��Ӧ���ĸ�ʽ�� (<sp>Ϊ�ո�, " "��������һ���Ϸ��Ŀո�)
* HttpVersion <sp> StateCode <sp> StateHint <crlf>
* Header0: <sp> HeaderValue <crlf>
* ...
* HeaderN: <sp> HeaderValue <crlf>
* <crlf> (which is important)
* EntityBody (empty for GET, something for POST)
* @remark
* ���ļ��е�Http����δ��ȫʵ��.
*/
#pragma once
#include "afxinet.h"

/**
* @class
* @brief �����װHttpЭ�鷽��.
* @author ACM2012
* @note
* �����������Ĺ��캯����������������ʵ��HttpЭ�����Ӧ������.
*/
class HttpSet
{
public:
	/** @brief ���캯��*/
	HttpSet();
	/** @brief ��������*/
	~HttpSet();
	/** @brief ����URL�ͷ�������������*/
	CString HttpSet::request(LPCTSTR pstrURL, string method, string content);
	/** @brief ����IP��ַ�ͷ���������Ӧ����*/
	HttpSet::response(LPCTSTR pstrURL, string method, string content);
};