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
*NetHtmlΪ���Ի�ȡ��ҳ�࣬����ʵ�ֵĹ�����ͨ���׽��ֻ�ȡ�����ϵ�url��Դ�����н���һ����Ա����getURLContext��\n
*�ú����Ĺ�����ʵ��ͨ��url��ȡ��Ӧ����ҳ�ı��������ı�ת��ΪUnicode�����ظ����ú�����\n
*/
class NetHtml
{
public:
	NetHtml();
	~NetHtml();
	/**
	*@brief   ͨ��url��ȡhtml�ַ���������
	*@return ��ȡ���ַ���������
	*@param pstrURL  �����ʵ�url�����ַ���
	*/
	CString  NetHtml::getURLContext(LPCTSTR pstrURL);
};

