#include "stdafx.h"
#include "SocketClient.h"

SocketClient::SocketClient()
{
}

SocketClient::~SocketClient()
{

}

SocketClient::SocketClient(CString ServeIP)
{
	flag = false;
	Create();
	if (!Connect(ServeIP, 6666)){
		AfxMessageBox(_T("服务器已断开!"));
		return;
	}
	flag = true;
}

bool SocketClient::Check(CString A, CString B)
{
	CString Info;
	TCHAR Respond[1024];
	User = A;
	Password = B;
	if (flag == false)
		return false;
	Info = User + _T(" ") + Password + _T(" ");
	Send(Info, Info.GetLength()*sizeof(TCHAR));
	memset(Respond, 0, sizeof(Respond));
	Receive(Respond, sizeof(Respond));
	if (StrCmpW(Respond, _T("checked")) != 0){
		Close();
		return false;
	}
	return true;
}

bool SocketClient::UploadFile(CFile &myfile)
{
	TCHAR respond[1024];
	char context[1024];
	CString cmdline;
	CString Path;
	CString Len;
	int nsize;

	Send(_T("upload"), sizeof(_T("upload")));
	memset(respond, 0, 1024 * sizeof(TCHAR));
	Receive(respond, 1024 * sizeof(TCHAR));
	if (StrCmpW(respond, _T("waitpath")) != 0){
		Send(_T("Error"), sizeof(_T("Error")));
		return false;
	}
	Path =_T("user\\")+User+_T("\\")+myfile.GetFileName();
	Send(Path, Path.GetLength()*sizeof(TCHAR));
	memset(respond, 0, 1024 * sizeof(TCHAR));
	Receive(respond, 1024 * sizeof(TCHAR));
	if (StrCmpW(respond, _T("waitlen")) != 0){
		Send(_T("Error"), sizeof(_T("Error")));
		return false;
	}
	Len.Format(_T("%d"), myfile.GetLength());
	Send(Len, Len.GetLength()*sizeof(TCHAR));
	memset(respond, 0, 1024 * sizeof(TCHAR));
	Receive(respond, 1024 * sizeof(TCHAR));
		if (StrCmpW(respond, _T("ready")) == 0){
			while ((nsize = myfile.Read(context, 1024)) != 0)
				Send(context, nsize);
		}
		else
			return false;
	myfile.Close();
	memset(respond, 0, 1024 * sizeof(TCHAR));
	Receive(respond, 1024 * sizeof(TCHAR));
	Close();
	if (StrCmpW(respond, _T("success")) == 0){
		AfxMessageBox(_T("上传成功!\n"));
		return true;
	}
	return false;
}

int SocketClient::ConnectServe()
{
	TCHAR Buf[1024];
	int nport;
	Send(_T("connect"), sizeof(_T("connect")));
	memset(Buf, 0, sizeof(Buf));
	Receive(Buf, 1024);
	swscanf_s(Buf, _T("%d"), &nport);
	return nport;
}