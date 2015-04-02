#include "stdafx.h"
#include "NetHtml.h"

NetHtml::NetHtml()
{

}

NetHtml::~NetHtml()
{
}
CString  NetHtml::getURLContext(LPCTSTR pstrURL){
	CString myhtml;
	CInternetSession mySession(NULL, 0);
	CHttpFile* htmlFile = NULL;
	TCHAR src[1024];
	try{
	htmlFile = (CHttpFile*)mySession.OpenURL(pstrURL);//建立连接获取输入;
	while (htmlFile->ReadString(src, 1024)){//处理流中每一行
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
	AfxMessageBox(_T("获取数据失败"));
	} 
	return myhtml;
};
