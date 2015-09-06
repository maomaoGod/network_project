#include "stdafx.h"
#include "SocketManager.h"

#include <sys/stat.h>
#include <sys/types.h>   
#include <io.h>
#include "HttpServeSocket.h"

extern void PrintView(CString);
extern CString NowUser;

int SocketManager::NowPort = 6500;

SocketManager::SocketManager()
{
	MState = MINIT;
	curlen = 0;
}


SocketManager::~SocketManager()
{

}


void SocketManager::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	Close();
	CAsyncSocket::OnClose(nErrorCode);
}

bool SocketManager::Check()
{
	CFile myfile;
	TCHAR Buf[100];
	memset(Buf, 0, sizeof(Buf));
	myfile.Open(_T("user\\") + User + _T("\\password.txt"), CFile::modeRead);
	myfile.Read(Buf, sizeof(Buf));
	myfile.Close();
	if (Password.Compare(Buf) == 0)
		return true;
	else 
		return false;
}

void   SocketManager::InitCmd(TCHAR Buf[1024])
{
	CString tmp;
	CFile myfile;
	CFileFind FileFind;
	tmp.Format(_T("%s"), Buf);
	int index = tmp.Find(_T(" "));
	User = tmp.Mid(0, index);
	tmp = tmp.Mid(index + 1);
	index = tmp.Find(_T(" "));
	Password = tmp.Mid(0, index);
	if (!PathIsDirectory(_T("user\\") + User))
		CreateDirectory(_T("user\\") + User,NULL);
	if (!FileFind.FindFile(_T("user\\") + User + _T("\\password.txt"))){
		myfile.Open(_T("user\\") + User + _T("\\password.txt"), CFile::modeCreate|CFile::modeWrite);
		myfile.Write(Password, Password.GetLength()*sizeof(TCHAR));
		myfile.Close();
	}
	if (!FileFind.FindFile(_T("user\\") + User + _T("\\port.txt"))){
		myfile.Open(_T("user\\") + User + _T("\\port.txt"), CFile::modeCreate | CFile::modeWrite);
		CString Port;
		Port.Format(_T("%d"), NowPort);
		NowPort++;
		myfile.Write(Port, Port.GetLength()*sizeof(TCHAR));
		myfile.Close();
	}
}

void SocketManager::OnReceive(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	int nchar;
	CString mystr;
	TCHAR Buf[1024];
	memset(Buf, 0, sizeof(Buf));
	nchar = Receive(Buf, 1024);
	if (MState == MINIT){
		InitCmd(Buf);
		if (Check()){
			Send(_T("checked"), sizeof(_T("checked")));
			MState = MCHECKED;
		}
		else {
			Send(_T("Failed"), sizeof(_T("Failed")));
			MState = MINIT;
		}
	}
	else if (MState == MCHECKED){
		Cmd.Format(_T("%s"), Buf);
		if (Cmd.Compare(_T("upload")) == 0){
			Send(_T("waitpath"), sizeof(_T("waitpath")));
			MState = MPATH;
		}
		else if (Cmd.Compare(_T("connect")) == 0){
			CString cmdline;
			cmdline = _T("NetProtocol.exe");
			CString para = cmdline + _T("  -") + User + _T("server");
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&pi, sizeof(pi));
			ZeroMemory(&si, sizeof(si));
			if (!CreateProcess(NULL, para.GetBuffer(),
				NULL,
				NULL,
				FALSE,
				0,
				NULL,
				NULL,
				&si,
				&pi)){
				int error = GetLastError();
				CString vs;
				vs.Format(_T("%s %d"), _T("启动协议栈失败!"), error);
				AfxMessageBox(vs);
				Send(_T("-1"), sizeof(_T("-1")));
				MState = MINIT;
			}
			else {
				CFile myport;
				CString Port;
				HANDLE Success = CreateSemaphore(NULL, 0, 100, _T("-") + User + _T("server") + _T("NetProtocolCreate"));
				if (WaitForSingleObject(Success, 100000) == WAIT_TIMEOUT){
					AfxMessageBox(_T("开启超时!\n"));
					Send(_T("-1"), sizeof(_T("-1")));
					MState = MINIT;
					return;
				}
				if (!myport.Open(_T("user\\") + User + _T("\\port.txt"), CFile::modeRead)){
					Send(_T("-1"), sizeof(_T("-1")));
					MState = MINIT;
					myport.Close();
				}
				else {
					NowUser = _T("-") + User + _T("server");
					int nport;
					TCHAR portbuf[100];
					memset(portbuf, 0, sizeof(portbuf));
					myport.Read(portbuf, sizeof(portbuf));
					swscanf_s(portbuf, _T("%d"), &nport);
					Send(portbuf, wcslen(portbuf)*sizeof(TCHAR));
					myport.Close();
					AfxMessageBox(portbuf);
					HttpServeSocket *psock;
					psock = new HttpServeSocket();
					psock->Create(nport);
					psock->Listen();
				}
			}
		}
		else if (MState == MPATH){
			CFileException fe;
			if (!myfile.Open(Buf, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary, &fe)){
				TCHAR strCause[256];
				fe.GetErrorMessage(strCause, 255);
				AfxMessageBox(strCause);
				Send(_T("Failed"), sizeof(_T("Failed")));
				MState = MINIT;
			}
			else{
				Send(_T("waitlen"), sizeof(_T("waitlen")));
				MState = MLEN;
			}
		}
		else if (MState == MLEN){
			swscanf_s(Buf, _T("%d"), &filelen);
			Send(_T("ready"), sizeof(_T("ready")));
			MState = MRECEIVE;
		}
		else if (MState == MRECEIVE){
			myfile.Write(Buf, nchar);
			curlen += nchar;
			mystr.Format(_T("%d\n"), curlen);
			PrintView(mystr);
			if (curlen == filelen){
				myfile.Close();
				MState = MINIT;
				Send(_T("success"), sizeof(_T("success")));
			}
		}
		CAsyncSocket::OnReceive(nErrorCode);
	}
}