/**@file
*@brief  ��ҳ��ȡ��
*@date 2015.4.20
*@version <0.1>
*@note
*������Ƶ�ʵ��
*/
#pragma once
#include "afxinet.h"
/**
*@class <NetHtml>
*@brief  ���Ի�ȡ��ҳ�࣬����ʵ�ֵĹ�����ͨ���׽��ֻ�ȡ�����ϵ�url��Դ
*@author ACM2012
*@note  NetHtmlΪ���Ի�ȡ��ҳ�࣬����ʵ�ֵĹ�����ͨ���׽��ֻ�ȡ�����ϵ�url��Դ�����н���һ����Ա����
*getURLContext,�ú����Ĺ�����ʵ��ͨ��url��ȡ��Ӧ����ҳ�ı��������ı�ת��ΪUnicode�����ظ����ú�����
*/
class NetHtml
{
public:
	NetHtml();
	~NetHtml();
	/**@brief   ͨ��url��ȡhtml�ַ���������*/
	CString  NetHtml::getURLContext(LPCTSTR pstrURL);
};

