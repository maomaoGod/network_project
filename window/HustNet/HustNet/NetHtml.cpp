#include "stdafx.h"
#include "NetHtml.h"
#include "Tools.h"
#include "UICtrl.h"

using namespace Tools;

#define MAXLEN 1024
extern void PrintLog (CString);

NetHtml::NetHtml()
{

}

NetHtml::~NetHtml()
{
}

/**
*����ͨ��CSocket��ȡ��Ӧ��html�ļ��ַ�����\n
*����ͨ��CSocket��url��Ӧ�ķ�������������,����������Դ���ķ��͸����������ɹ���Ӧ��һ���еĻ�ȡ�ӷ��������������ݣ�\n
*��������ת��ΪUnicode��ƴ�ӵ����ص��ַ���myhtml
* @param pstrURL  �����ʵ�url�����ַ���
*/

CString  NetHtml::getURLContext(LPCTSTR pstrURL){
	/*CString myhtml;
	CInternetSession mySession(NULL, 0);
	CHttpFile* htmlFile = NULL;
	TCHAR src[1024];
	try{
	PrintLog(_T("��������!"));
	htmlFile = (CHttpFile*)mySession.OpenURL(pstrURL);
	PrintLog(_T("����ȡ�ر���!"));
	while (htmlFile->ReadString(src, 1024)){
	
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, NULL, 0);
	TCHAR *pBuffer = new TCHAR[nBufferSize + 1];
	memset(pBuffer, 0, (nBufferSize + 1)*sizeof(TCHAR));
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, pBuffer, nBufferSize*sizeof(TCHAR));//UTF-8תUnicode
	myhtml += pBuffer;
	myhtml += _T("\r\n");
	delete pBuffer;
	}
	htmlFile->Close();
	mySession.Close(); //�رջỰ
	}
	catch (CException *e) {  //�쳣����
		PrintLog(_T("��ȡ����ʧ��!"));
	} 
	return myhtml;*/
	AfxSocketInit();	// ��ʼ��
	CString myhtml;
	CSocket mySocket;
	CString tempURL = pstrURL;
	//string myurl = Tstr::Tchar2string(tempURL.GetBuffer());
	string pHost;
	string GET;
	int i = tempURL.Find(_T('/'), 0);
	if (i == -1)
	{
		i = tempURL.GetLength();
		GET = "/";
	}
	else
	{
		CString tempGET = tempURL.Mid(i);
		GET = Tstr::Tchar2string(tempGET.GetBuffer());
	}
	//for (i = 0; myurl[i] != '/' && i < myurl.length(); i++) ;
	//if (i == myurl.length()) GET = "/";
	//else GET = myurl.substr(i, myurl.length() - i);
	CString tempHOST = tempURL.Mid(0, i);
	pHost = Tstr::Tchar2string(tempHOST.GetBuffer());
	LPCWSTR host_name = Tstr::String2LPCWSTR(pHost);
	string header = "";
	char buf[MAXLEN];
	TCHAR TCHAR_buf[MAXLEN];
	try{
		mySocket.Create();
		/**
		*����80�˿�
		*/
		mySocket.Connect(host_name, 80);	// ����80�˿�
		//mySocket.Receive(buf, MAXLEN, 0);
		header = "GET " + GET + " HTTP/1.1\nHOST: " + pHost + "\n\n";
		char *temp = Tstr::S2Cstar(header);
		mySocket.Send(temp, header.length() + 1, 0);
		mySocket.Send("Connection: Close\n\n", 20, 0);
		free(temp);
		while (mySocket.Receive(buf, MAXLEN, 0) > 0)
		{
			int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buf, -1, NULL, 0);
			TCHAR *pBuffer = new TCHAR[nBufferSize + 1];
			memset(pBuffer, 0, (nBufferSize + 1)*sizeof(TCHAR));
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)buf, -1, pBuffer, nBufferSize*sizeof(TCHAR));//UTF-8תUnicode
			myhtml += pBuffer;
		}
	}
	/**
	*�쳣����
	*/
	catch (CException *e) {  //�쳣����
		PrintLog(_T("��ȡ����ʧ��!"));
	}
	/**
	*�����ı��ַ���
	*/
	return myhtml;  //�����ı��ַ���
};
