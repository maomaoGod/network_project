
#pragma once 
#include "stdafx.h"
#include "CmyAsyncSocket.h"
#include "Serve.h"

extern void PrintView(CString e);

int    SOCKCOUNT = 0;

CmyAsyncSocket::CmyAsyncSocket()
{
	flag = false;
	HANDLE MFile = OpenFileMapping(FILE_MAP_WRITE, FALSE, _T("NetProtocolListen"));//�������ļ�
	if (NULL == MFile){
		AfxMessageBox(_T("�������ļ�ʧ��"));
		return;
	}
	regstruct *preg = (regstruct*)MapViewOfFile(MFile, FILE_MAP_WRITE, 0, 0, sizeof(regstruct));//ӳ�������ļ�������
	if (NULL == preg){
		AfxMessageBox(_T("ʧ��"));
		return;
	}
	HANDLE Csemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolCsemaphore"));//��Ӧ�ó����ź���
	if (NULL == Csemaphore){
		AfxMessageBox(_T("ʧ��"));
		return;
	}
	HANDLE Psemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolPsemaphore"));//��Э���������ź���
	if (NULL == Psemaphore){
		AfxMessageBox(_T("ʧ��"));
		return;
	}
	regstruct myreg;
	WaitForSingleObject(Csemaphore, INFINITE);  //�ȴ��źŵ���
	GetSockMark(preg, &myreg);  //׼���ù����ڴ�����ʹ�õ��ź�����
	ReleaseSemaphore(Psemaphore, 1, NULL); //ע��socket
	HANDLE Success = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolCreateSuccess"));
	WaitForSingleObject(Success, 10000); //�ȴ�ע����ɣ��ź����͹����ڴ洴�����
	if (!InitalEvent(&myreg)) {//ӳ�乲���ļ����ź���������
		AfxMessageBox(_T("�����׽���ʧ��"));
		return;
	}
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewThread, (LPVOID)this, NULL, NULL);//���������߳�
	flag = true;
}

CmyAsyncSocket::~CmyAsyncSocket()
{

}

bool CmyAsyncSocket::InitalEvent( regstruct *myreg)
{
	myEvent.WFile = OpenFileMapping(FILE_MAP_WRITE, FALSE, myreg->writefilename);  //�򿪹����ڴ�
	myEvent.RFile =  OpenFileMapping(FILE_MAP_WRITE, FALSE, myreg->readfilename);
	if (myEvent.WFile == NULL || myEvent.RFile == NULL)
		return false;
	myEvent.Rpro = (prostruct *)MapViewOfFile(myEvent.RFile, FILE_MAP_WRITE, 0, 0, sizeof(prostruct)); //ӳ�乲���ڴ�
	myEvent.Wpro = (prostruct *)MapViewOfFile(myEvent.WFile, FILE_MAP_WRITE, 0, 0, sizeof(prostruct));
	if (myEvent.Rpro == NULL || myEvent.Wpro == NULL)
		return false;
	myEvent.PSsock = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->PSname); //��ȡ�ź������
	myEvent.PRsock = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->PRname);
	myEvent.PWsock = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->PWname);
	myEvent.CSsock = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->CSname);
	myEvent.CRsock = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->CRname);
	myEvent.CWsock = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, (LPCWSTR)myreg->CWname);
	if (!(myEvent.PRsock || myEvent.PSsock || myEvent.PWsock || myEvent.CSsock || myEvent.CRsock || myEvent.CWsock))
		return false;
	myEvent.Wpro->SockMark = SockMark;
	return true;
}
void  CmyAsyncSocket::Tchar2char(LPCTSTR src, LPSTR dst)//Tchar ת��Ϊchar 
{
	int iLength = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, src, -1, dst, iLength, NULL, NULL);
}

void  CmyAsyncSocket::char2Tchar(LPSTR src, LPCTSTR dst, int maxlen) //charת��ΪTchar
{
	MultiByteToWideChar(CP_ACP, 0, src, -1, (LPWSTR)dst, maxlen);
}


int CmyAsyncSocket::Send(const void* lpBuf, int nBufLen) //��������
{
	WaitForSingleObject(myEvent.CSsock, INFINITE); //�ȴ�Э���ȡ���ݽ���
	myEvent.Wpro->FuncID = SOCKSEND;
	memset(myEvent.Wpro->mysock.data, 0, 2048);
	memcpy(myEvent.Wpro->mysock.data, lpBuf, nBufLen);
	myEvent.Wpro->mysock.datalength = nBufLen;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
	return nBufLen;
}

/** Udp������
*/
int CmyAsyncSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress)
{
	WaitForSingleObject(myEvent.CSsock, INFINITE); //�ȴ�Э���ȡ���ݽ���
	myEvent.Wpro->FuncID = SOCKSENDTO;
	memset(myEvent.Wpro->mysock.data, 0, 2048);
	memcpy(myEvent.Wpro->mysock.data, lpBuf, nBufLen);
	Tchar2char(lpszHostAddress, myEvent.Wpro->mysock.dstip);
	myEvent.Wpro->mysock.dstport = nHostPort;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
	return nBufLen;
}
/** ��������
*/

int  CmyAsyncSocket::Receive(void* lpBuf, int nBufLen)
{
	memcpy(myEvent.Wpro->mysock.srcip, myEvent.Rpro->mysock.dstip, 20);
	memcpy(myEvent.Wpro->mysock.dstip, myEvent.Rpro->mysock.srcip, 20);
	myEvent.Wpro->mysock.srcport = myEvent.Rpro->mysock.dstport;
	myEvent.Wpro->mysock.dstport = myEvent.Rpro->mysock.srcport;
	memset(lpBuf, 0, nBufLen);
	memcpy(lpBuf, myEvent.Rpro->mysock.data, myEvent.Rpro->mysock.datalength);  //��ȡ����
	return nBufLen;
}

 void CmyAsyncSocket::OnReceive(int nErrorCode)
{

}


/** ����TCP����
*/
bool  CmyAsyncSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{ 
	WaitForSingleObject(myEvent.CSsock, INFINITE);  //��������
	myEvent.Wpro->FuncID = SOCKCONNECT;
	Tchar2char(lpszHostAddress, myEvent.Wpro->mysock.dstip);
	myEvent.Wpro->mysock.dstport = nHostPort;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
	return true;
}
/** ������������
*/
bool CmyAsyncSocket::Listen()
{
	WaitForSingleObject(myEvent.CSsock, INFINITE);
	myEvent.Wpro->FuncID = SOCKLISTEN;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
	return true;
}
/** ����socket
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

DWORD WINAPI CmyAsyncSocket::NewThread(LPVOID lParam)
{
	CmyAsyncSocket *pthis = (CmyAsyncSocket *)lParam;
	pthis->ReadSock();
	return 0;
}

void CmyAsyncSocket::ReadSock()
{	
	while (true){
		WaitForSingleObject(myEvent.CRsock, INFINITE);
		switch (myEvent.Rpro->FuncID){
		case SOCKSEND:  //�����Э�����Ϊ�������ݵ���������
		case SOCKSENDTO:
			OnReceive(true);
			break;
		case SOCKCONNECT:
			OnAccept(true);
		case SOCKCLOSE:
			OnClose(true);
		}
		ReleaseSemaphore(myEvent.PSsock, 1, NULL);
	}
}

bool CmyAsyncSocket::Bind(UINT nSocketPort)
{
	WaitForSingleObject(myEvent.CSsock, INFINITE);
	myEvent.Wpro->FuncID = SOCKBIND;
	myEvent.Wpro->mysock.bindport = nSocketPort;
	ReleaseSemaphore(myEvent.PRsock, 1, NULL);
	return true;
}

void  CmyAsyncSocket::GetSockMark(regstruct *preg,regstruct *myreg) //��ȡ���������   ��������+���̱�ţ�5λ��+�׽��ֱ�ţ�5λ��
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
	memcpy(preg->readfilename, _T("RSOCK"), sizeof(_T("RSOCK")));  //����������
	memcpy(preg->writefilename, _T("WSOCK"),sizeof(_T("WSOCK")));
	memcpy(preg->PSname, _T("PSSEG"), sizeof(_T("PSSEG")));
	memcpy(preg->PRname, _T("PRSEG"), sizeof(_T("PRSEG")));
	memcpy(preg->PWname, _T("PWSEG"), sizeof(_T("PWSEG")));
	memcpy(preg->CSname, _T("CSSEG"), sizeof(_T("CSSEG")));
	memcpy(preg->CRname, _T("CRSEG"), sizeof(_T("CRSEG")));
	memcpy(preg->CWname, _T("CWSEG"), sizeof(_T("CWSEG")));
	while (index!=4)  //���ӽ��̱��
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
	while (index != 9) //����socket ˳���
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
	myEvent.Wpro->AcceptSockMark = rConnectedSocket.myEvent.Wpro->SockMark;//��־ת���׽��ֵı��
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