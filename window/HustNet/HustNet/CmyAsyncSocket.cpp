#pragma once 
#include "stdafx.h"
#include "CmyAsyncSocket.h"
#include "HustNet.h"

int    SOCKCOUNT = 0;

CmyAsyncSocket::CmyAsyncSocket()
{
	flag = false;
	HANDLE MFile = OpenFileMapping(FILE_MAP_WRITE, FALSE, _T("NetProtocolListen"));//打开连接文件
	if (NULL == MFile){
		AfxMessageBox(_T("失败"));
		return;
	}
	regstruct *preg  = (regstruct*)MapViewOfFile(MFile, FILE_MAP_WRITE, 0, 0, sizeof(regstruct));//映射连接文件到本地
	if (NULL == preg){
		AfxMessageBox(_T("失败"));
	   return;
	}
	HANDLE Csemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolCsemaphore"));//打开应用程序信号量
	if (NULL == Csemaphore){
		AfxMessageBox(_T("失败"));
		return;
	}
	HANDLE Psemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolPsemaphore"));//打开协议服务程序信号量
	if (NULL == Psemaphore){
		AfxMessageBox(_T("失败"));
		return;
	}
	regstruct myreg;
	WaitForSingleObject(Csemaphore,INFINITE);  //等待信号到来
	GetSockMark(preg,&myreg);  //准备好共享内存名和使用的信号量名
	ReleaseSemaphore(Psemaphore, 1, NULL); //注册socket
	HANDLE Success = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolCreateSuccess"));
	WaitForSingleObject(Success, 10000); //等待注册完成，信号量和共享内存创建完毕
	if (!InitalEvent(&myreg)) {//映射共享文件和信号量到本地
		AfxMessageBox(_T("创建套接字失败"));
		return;
	}
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReadSock, (LPVOID)this, NULL, NULL);//创建接收线程
	flag = true;
}


CmyAsyncSocket::~CmyAsyncSocket()
{

}

bool CmyAsyncSocket::InitalEvent(regstruct *myreg)
{
	myEvent.WFile = OpenFileMapping(FILE_MAP_WRITE, FALSE, myreg->writefilename);  //打开共享内存
	myEvent.RFile  = OpenFileMapping(FILE_MAP_WRITE, FALSE, myreg->readfilename);
	if (myEvent.WFile == NULL || myEvent.RFile == NULL)
		return false;
	myEvent.Rpro=   (prostruct *)MapViewOfFile(myEvent.RFile,  FILE_MAP_WRITE, 0, 0, sizeof(prostruct)); //映射共享内存
	myEvent.Wpro = (prostruct *)MapViewOfFile(myEvent.WFile, FILE_MAP_WRITE, 0, 0, sizeof(prostruct));
	if (myEvent.Rpro == NULL || myEvent.Wpro == NULL)
		return false;
	myEvent.PSsock  = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->PSname); //获取信号量句柄
	myEvent.PRsock  = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->PRname);
	myEvent.PWsock = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->PWname);
	myEvent.CSsock  = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->CSname);
	myEvent.CRsock  = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->CRname);
	myEvent.CWsock = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->CWname);
	if (!(myEvent.PRsock && myEvent.PSsock && myEvent.PWsock && myEvent.CSsock && myEvent.CRsock && myEvent.CWsock))
		return false;
	myEvent.Wpro->SockMark = SockMark;
	return true;
}
void  CmyAsyncSocket::Tchar2char(LPCTSTR src, LPSTR dst)//Tchar 转换为char 
{
	int iLength = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, src, -1, dst, iLength, NULL, NULL);
}

void  CmyAsyncSocket::char2Tchar(LPSTR src, LPCTSTR dst, int maxlen) //char转换为Tchar
{
	MultiByteToWideChar(CP_ACP, 0, src, -1, (LPWSTR)dst, maxlen);
}


int CmyAsyncSocket::Send(const void* lpBuf, int nBufLen) //发送数据
{
	WaitForSingleObject(myEvent.CSsock, INFINITE); //等待协议读取数据结束
	myEvent.Wpro->FuncID = SOCKSEND;
	memset(myEvent.Wpro->mysock.data, 0, 2048);
	memcpy(myEvent.Wpro->mysock.data, lpBuf, nBufLen);
	myEvent.Wpro->mysock.datalength = nBufLen;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
	return nBufLen;
}

/** Udp发数据
*/
int CmyAsyncSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress)
{
	WaitForSingleObject(myEvent.CSsock, INFINITE); //等待协议读取数据结束
	myEvent.Wpro->FuncID = SOCKSENDTO;
	memset(myEvent.Wpro->mysock.data, 0, 2048);
	memcpy(myEvent.Wpro->mysock.data, lpBuf, nBufLen);
	Tchar2char(lpszHostAddress, myEvent.Wpro->mysock.dstip);
	myEvent.Wpro->mysock.dstport = nHostPort;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
	return nBufLen;
}
/** 接收数据
*/
void  CmyAsyncSocket::OnReceive(int nErrorCode)
{

}

int  CmyAsyncSocket::Receive(void* lpBuf, int nBufLen)
{
	memcpy(lpBuf, myEvent.Rpro->mysock.data, nBufLen);  //读取数据
	return nBufLen;
}

/** 建立TCP连接
*/
bool  CmyAsyncSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{ 
	WaitForSingleObject(myEvent.CSsock, INFINITE);  //请求连接
	myEvent.Wpro->FuncID = SOCKCONNECT;
	Tchar2char(lpszHostAddress, myEvent.Wpro->mysock.dstip);
	myEvent.Wpro->mysock.dstport = nHostPort;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
	return true;
}
/** 监听连接请求
*/
bool CmyAsyncSocket::Listen()
{
	WaitForSingleObject(myEvent.CSsock, INFINITE);
	myEvent.Wpro->FuncID = SOCKLISTEN;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
	return true;
}
/** 创建socket
*/
bool CmyAsyncSocket::Create()
{
	return flag;
}

bool CmyAsyncSocket::Create(UINT nHostPort)
{

	if (true == Bind(nHostPort)&&flag)
		return true;
	return false;
}

DWORD WINAPI CmyAsyncSocket::ReadSock(LPVOID lParam)
{
	CmyAsyncSocket *pthis = (CmyAsyncSocket *)lParam;
	while (true){
		WaitForSingleObject(pthis->myEvent.CRsock, INFINITE);
		switch (pthis->myEvent.Rpro->FuncID){
		case SOCKSEND:  //如果是协议程序为发送数据到来，则收
		case SOCKSENDTO:
			pthis->OnReceive(true);
			break;
		case SOCKCONNECT:
			pthis->OnAccept(true);
		case SOCKCLOSE:
			pthis->OnClose(true);
		}
		ReleaseSemaphore(pthis->myEvent.PSsock,1,NULL);
	}
	return 0;
}

bool CmyAsyncSocket::Bind(UINT nSocketPort)
{
	WaitForSingleObject(myEvent.CSsock, INFINITE);
	myEvent.Wpro->FuncID = SOCKBIND;
	myEvent.Wpro->mysock.bindport = nSocketPort;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
	return true;
}

void  CmyAsyncSocket::GetSockMark(regstruct *preg,regstruct *myreg) //获取互斥量标记   互斥量名+进程编号（5位）+套接字编号（5位）
{
	int LEN = 20 * sizeof(TCHAR);
	int pid = _getpid();
	int socknum = SOCKCOUNT;
	TCHAR C;
	int A = 10000, B = 100;
	int index = 9;
	SockMark = pid * 100 + SOCKCOUNT;
	preg->SockMark = SockMark;
	SOCKCOUNT++;
	memset(preg->readfilename, 0, LEN);
	memset(preg->writefilename, 0,LEN);
	memset(preg->PRname, 0, LEN);
	memset(preg->PWname, 0, LEN);
	memset(preg->CRname, 0, LEN);
	memset(preg->CWname, 0, LEN);
	memcpy(preg->readfilename, _T("RSOCK"), sizeof(_T("RSOCK")));  //命名互斥量
	memcpy(preg->writefilename, _T("WSOCK"),sizeof(_T("WSOCK")));
	memcpy(preg->PSname, _T("PSSEG"), sizeof(_T("PSSEG")));
	memcpy(preg->PRname, _T("PRSEG"), sizeof(_T("PRSEG")));
	memcpy(preg->PWname, _T("PWSEG"), sizeof(_T("PWSEG")));
	memcpy(preg->CSname, _T("CSSEG"), sizeof(_T("CSSEG")));
	memcpy(preg->CRname, _T("CRSEG"), sizeof(_T("CRSEG")));
	memcpy(preg->CWname, _T("CWSEG"), sizeof(_T("CWSEG")));
	while (index!=4)  //增加进程编号
	{
		C = pid % 10 + _T('0');
		preg->readfilename[index] = C;
		preg->writefilename[index] = C;
		preg->PRname[index] = C;
		preg->PWname[index] = C;
		preg->CRname[index] = C;
		preg->CWname[index] = C;
		preg->PSname[index] = C;
		preg->CSname[index] = C;
		pid  /= 10;
		index--;
	}
	index = 11;
	while (index != 9) //加入socket 顺序号
	{
		C = socknum % 10 + _T('0');
		preg->readfilename[index] = C;
		preg->writefilename[index] = C;
		preg->PSname[index] = C;
		preg->PRname[index] = C;
		preg->PWname[index] = C;
		preg->CSname[index] = C;
		preg->CRname[index] = C;
		preg->CWname[index] = C;
		socknum /= 10;
		index--;
	}
	memcpy(myreg, preg, sizeof(regstruct));
}

void  CmyAsyncSocket::OnAccept(int nErrorCode)
{

}

void  CmyAsyncSocket::Accept(CmyAsyncSocket& rConnectedSocket)
{
	WaitForSingleObject(myEvent.CSsock, INFINITE);
	myEvent.Wpro->FuncID = SOCKACCEPT;
	myEvent.Wpro->AcceptSockMark = rConnectedSocket.myEvent.Wpro->SockMark;//标志转接套接字的编号
	memcpy(myEvent.Wpro->mysock.srcip, myEvent.Rpro->mysock.dstip, 20);
	memcpy(myEvent.Wpro->mysock.dstip, myEvent.Rpro->mysock.srcip, 20);
	myEvent.Wpro->mysock.srcport = myEvent.Wpro->mysock.dstport;
	myEvent.Wpro->mysock.dstport = myEvent.Wpro->mysock.srcport;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
}

void  CmyAsyncSocket::OnClose(int nErrorCode)
{

}

void CmyAsyncSocket::Close()
{
	WaitForSingleObject(myEvent.CSsock, INFINITE);
	myEvent.Wpro->FuncID = SOCKCLOSE;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
}