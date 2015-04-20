/** @file
* @brief Http������
* @author ACM2012
* @date 2015/04/19
* @version 0.1
* @note
* ���ļ�ʵ��Http������װ��Http�������Ӧ����.
* HTTP�����ĸ�ʽ�� (<sp>Ϊ�ո�, " "��������һ���Ϸ��Ŀո�)
* Method <sp> URL <sp> HttpVersion <crlf>
* Header0: <sp> HeaderValue <crlf>
* ...
* HeaderN: <sp> HeaderValue <crlf>
* <crlf> (which is important)
* EntityBody (empty for PATH, something for POST)
* HTTP��Ӧ���ĸ�ʽ�� (<sp>Ϊ�ո�, " "��������һ���Ϸ��Ŀո�)
* HttpVersion <sp> StateCode <sp> StateHint <crlf>
* Header0: <sp> HeaderValue <crlf>
* ...
* HeaderN: <sp> HeaderValue <crlf>
* <crlf> (which is important)
* EntityBody (empty for PATH, something for POST)
* @remark
* ���ļ��е�Http����δ��ȫʵ��.
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
* ���캯�����Ա���չ
* @since 0.1
*/
HttpSet::HttpSet()
{

}

/**
* @author ACM2012
* @note
* �����������Ա���չ
* @since 0.1
*/
HttpSet::~HttpSet()
{

}

/**
* @author ACM2012
* @param[in] pstrURL ����URL��LPCTSTR����
* @param[in] method ʹ�õ�Http������string����
* @param[in] content �������ݣ�string����
* @return ��ȡ�ı�������
* @note
* ����������URL���ú������仮��Ϊ�������֣�
* ��һ������Ϊ������������ָ�������Ҫ��ȡ��HTML�ļ��ķ�������
* �ڶ�������Ϊ·��+��ѯ��+Ƭ�η�������·��Ϊ�����·��������ļ�ϵͳ·������ѯ��Ϊ
* �ύ��������̨���н����Ĵ������ڶ�̬�ط����û�����������ݣ�Ƭ�η����������û�
* �����ǰ������������Դ�е�λ�ã���ʱ�������ڲ�ѯ���У�
* ���ڶ�����Ϊ�գ�����ΪĬ�ϡ�/������ʾ��ǰĿ¼��.
* ���������������������֮�䴴�����ӣ�֮��ʹ��method�����ķ�������װ���Ĳ�����.
* ���ͺ�Ͽ����ӣ����򳤵ȴ�ֱ��������ʧ���ӣ�
* ��ȡ��Ӧ��������.
* @remark
* �ú���������Ҫ��д.
* @since 0.1
*/
CString HttpSet::request(LPCTSTR pstrURL, string method, string content)
{
	/** @brief ��ʼ��Socket*/
	AfxSocketInit();
	CString myhtml;
	/** @brief ����һ��Socket*/
	CSocket mySocket;
	CString tempURL = pstrURL;
	string pHost;
	string PATH;
	/** @brief ��URL���ҵ���һ��б�ܣ����ڷֿ���������*/
	int i = tempURL.Find(_T('/'), 0);
	if (i == -1)
	{
		/** @brief �ڶ�����Ϊ��*/
		i = tempURL.GetLength();
		PATH = "/";
	}
	else
	{
		/** @brief ��ȡ�ڶ����ֲ�ת��*/
		CString tempPATH = tempURL.Mid(i);
		PATH = Tstr::Tchar2string(tempPATH.GetBuffer());
	}
	/** @brief ��ȡ����������һ���֣���ת��*/
	CString tempHOST = tempURL.Mid(0, i);
	pHost = Tstr::Tchar2string(tempHOST.GetBuffer());
	LPCWSTR host_name = Tstr::String2LPCWSTR(pHost);
	string header = "";
	char buf[MAXLEN];
	TCHAR TCHAR_buf[MAXLEN];
	try{
		/** @brief ����һ��Socket*/
		mySocket.Create();
		/** @brief ��ʽ�����ӵ���������80�˿�*/
		mySocket.Connect(host_name, 80);	// ����80�˿�
		/** @brief ��װ����ͷ*/
		header = method + " " + PATH + " HTTP/1.1\nHOST: " + pHost + "\n" + content + "\n";
		/** @brief ����ͷת��*/
		char *temp = Tstr::S2Cstar(header);
		/** @brief ������װ�õ�������*/
		mySocket.Send(temp, header.length() + 1, 0);
		/** @brief �رյ������������ӣ����򳤵ȴ�ֱ��������ʧ����*/
		mySocket.Send("Connection: Close\n\n", 20, 0);
		/** @brief �ͷ�ת�����õ��ַ����Ŀռ�*/
		free(temp);
		/** @brief ���շ��ص���Ӧ���Ĳ�ת��*/
		while (mySocket.Receive(buf, MAXLEN, 0) > 0)
		{
			int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buf, -1, NULL, 0);
			TCHAR *pBuffer = new TCHAR[nBufferSize + 1];
			memset(pBuffer, 0, (nBufferSize + 1)*sizeof(TCHAR));
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buf, -1, pBuffer, nBufferSize*sizeof(TCHAR));//UTF-8תUnicode
			myhtml += pBuffer;
		}
	}
	catch (CException *e)
	{
		/** @brief �쳣����Ϊ��ӡLog��Ϣ*/
		PrintLog(_T("��ȡ����ʧ��!"));
	}
	/** @brief ��������ȡ��Html����*/
	return myhtml;
}

/**
* @author ACM2012
* @param[in] pstrURL ����URL��LPCTSTR����
* @param[in] method ʹ�õ�Http������string����
* @param[in] content �������ݣ�string����
* @return ��ȡ�ı�������
* @note
* ����������URL���ú������仮��Ϊ�������֣�
* ��һ������Ϊ������������ָ�������Ҫ��ȡ��HTML�ļ��ķ�������
* �ڶ�������Ϊ·��+��ѯ��+Ƭ�η�������·��Ϊ�����·��������ļ�ϵͳ·������ѯ��Ϊ
* �ύ��������̨���н����Ĵ������ڶ�̬�ط����û�����������ݣ�Ƭ�η����������û�
* �����ǰ������������Դ�е�λ�ã���ʱ�������ڲ�ѯ���У�
* ���ڶ�����Ϊ�գ�����ΪĬ�ϡ�/������ʾ��ǰĿ¼��.
* ���������������������֮�䴴�����ӣ�֮��ʹ��method�����ķ�������װ���Ĳ�����.
* ���ͺ�Ͽ����ӣ����򳤵ȴ�ֱ��������ʧ���ӣ�
* �����ȡ��Ӧ��������.
* @remark
* �ú���������Ҫ��д.
* @since 0.1
*/
HttpSet::response(LPCTSTR pstrURL, string method, string content)
{
	/** @brief ��ʼ��Socket*/
	AfxSocketInit();
	/** @brief ����һ��Socket*/
	CSocket mySocket;
	CString tempURL = pstrURL;
	string pHost;
	string PATH;
	/** @brief ��URL���ҵ���һ��б�ܣ����ڷֿ���������*/
	int i = tempURL.Find(_T('/'), 0);
	if (i == -1)
	{
		/** @brief �ڶ�����Ϊ��*/
		i = tempURL.GetLength();
		PATH = "/";
	}
	else
	{
		/** @brief ��ȡ�ڶ����ֲ�ת��*/
		CString tempPATH = tempURL.Mid(i);
		PATH = Tstr::Tchar2string(tempPATH.GetBuffer());
	}
	/** @brief ��ȡ����������һ���֣���ת��*/
	CString tempHOST = tempURL.Mid(0, i);
	pHost = Tstr::Tchar2string(tempHOST.GetBuffer());
	LPCWSTR host_name = Tstr::String2LPCWSTR(pHost);
	string header = "";
	try{
		/** @brief ����һ��Socket*/
		mySocket.Create();
		/** @brief ��ʽ�����ӵ���������80�˿�*/
		mySocket.Connect(host_name, 80);	// ����80�˿�
		/** @brief ��װ����ͷ*/
		header = method + " " + PATH + " HTTP/1.1\nHOST: " + pHost + "\n" + content + "\n";
		/** @brief ����ͷת��*/
		char *temp = Tstr::S2Cstar(header);
		/** @brief ������װ�õ�������*/
		mySocket.Send(temp, header.length() + 1, 0);
		/** @brief �رյ������������ӣ����򳤵ȴ�ֱ��������ʧ����*/
		mySocket.Send("Connection: Close\n\n", 20, 0);
		/** @brief �ͷ�ת�����õ��ַ����Ŀռ�*/
		free(temp);
	}
	catch (CException *e)
	{
		/** @brief �쳣����Ϊ��ӡLog��Ϣ*/
		PrintLog(_T("��ȡ����ʧ��!"));
	}
}
