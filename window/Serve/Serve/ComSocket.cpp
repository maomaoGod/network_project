#include "stdafx.h"
#include "ComSocket.h"

int    CComSocket::SockNum= _getpid()*100;
HANDLE CComSocket::Wsemaphore = NULL;
HANDLE CComSocket::Rsemaphore = NULL;
HANDLE CComSocket::Dsemaphore = NULL;
HANDLE CComSocket::MFile = NULL;
regstruct *CComSocket::preg = NULL;

extern CString NowUser;
CComSocket::CComSocket()
{
	    CString usertmp;
	    usertmp = _T("-xianserver");
		MFile = OpenFileMapping(FILE_MAP_WRITE, FALSE, NowUser + _T("NetProtocolListen"));///<打开注册连接文件
		if (NULL == MFile){
			AfxMessageBox(_T("打开连接文件失败"));
			return;
		}
		preg = (regstruct*)MapViewOfFile(MFile, FILE_MAP_WRITE, 0, 0, sizeof(regstruct));///<映射注册连接文件到本地
		Wsemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, NowUser + _T("NetProtocolWsemaphore"));///<打开应用程序信号量
		Rsemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, NowUser + _T("NetProtocolRsemaphore"));///<打开协议服务程序信号量
		Dsemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, NowUser + _T("NetProtocolDsemaphore"));///<打开协议服务成功信号量
		if (!(preg||Wsemaphore||Rsemaphore)){    
			AfxMessageBox(_T("打开协议服务程序信号量失败"));
			return;
		}
		memset(preg->ReadQueueName, 0, sizeof(preg->ReadQueueName));
		memset(preg->WriteQueueName, 0, sizeof(preg->WriteQueueName));
		memset(preg->CDestoryName, 0, sizeof(preg->CDestoryName));
		memset(preg->SDestoryName, 0, sizeof(preg->SDestoryName));
		CString Tmp;
		Tmp = NowUser + _T("ReadQueue");
		memcpy(preg->ReadQueueName, Tmp.GetBuffer(), sizeof(TCHAR)*wcslen(Tmp.GetBuffer()));///<拷贝到注册文件读队列名
		Tmp = NowUser + _T("WriteQueue");
		memcpy(preg->WriteQueueName, Tmp.GetBuffer(), sizeof(TCHAR)*wcslen(Tmp.GetBuffer()));///<拷贝到注册文件写队列名
		Tmp = NowUser + _T("CDestory");
		memcpy(preg->CDestoryName, Tmp.GetBuffer(), sizeof(TCHAR)*wcslen(Tmp.GetBuffer()));
		Tmp = NowUser + _T("SDestory");
		memcpy(preg->SDestoryName, Tmp.GetBuffer(), sizeof(TCHAR)*wcslen(Tmp.GetBuffer()));
}

CComSocket::~CComSocket()
{
	
}

/**
* @author ACM2012
* @param [in] sockmark返回的套接字唯一标识符
* @note    函数返回套接字唯一标识符，并将唯一标示符添加到读写队列名中，构建唯一的读写队列名
*/
void  CComSocket::GetSockMark(unsigned int &sockmark, regstruct &myreg)
{
	TCHAR Buf[7];
	int temp;
	int Wlen, Rlen, Clen, Dlen;
	SockNum = SockNum + 1;
	temp = SockNum;
	for (int index = 6; index >=0 ; index--)///<套接字标志转为固定的7位字符串
	{
		Buf[index] = temp % 10 + _T('0');
		temp /= 10;
	}
	Wlen = wcslen(preg->WriteQueueName);
	Rlen = wcslen(preg->ReadQueueName);
	Clen = wcslen(preg->CDestoryName);
	Dlen = wcslen(preg->SDestoryName);
	memcpy(preg->WriteQueueName+Wlen, Buf, sizeof(Buf));///<拷贝套接字标志到写队列
	memcpy(preg->ReadQueueName+Rlen, Buf, sizeof(Buf));///<拷贝套接字标志到读队列名
	memcpy(preg->CDestoryName + Clen, Buf, sizeof(Buf));
	memcpy(preg->SDestoryName + Dlen, Buf, sizeof(Buf));
	preg->SockMark = SockNum;//注册文件赋值套接字标志 
	sockmark = SockNum;///<返回套接字唯一标示符
	memcpy(&myreg, preg, sizeof(regstruct));//返回套接字共享队列名
}

void  CComSocket::Tchar2char(LPSTR dst, LPCTSTR src)//Tchar 转换为char 
{
	int iLength = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, src, -1, dst, iLength, NULL, NULL);
}

/**
* @author ACM2012
* @note   辅助函数，数据传输使用char数据，应用程序使用TCHAR编码数据。本函数将char转换为tchar
*/
void  CComSocket::char2Tchar(LPCTSTR dst, LPSTR src, int maxlen) //char转换为Tchar
{
	MultiByteToWideChar(CP_ACP, 0, src, -1, (LPWSTR)dst, maxlen);
}

/**
* @author ACM2012
* @note   辅助函数，应用程序使用TCHAR编码数据，数据传输使用char数据。本函数将tchar转换为char
*/

bool   CComSocket::PrintLog(LPCTSTR ErrorCause, bool returnvalue)
{
	AfxMessageBox(ErrorCause);
	return returnvalue;
}

HANDLE CComSocket::PackNode(unsigned int function)
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);//创建一个空节点
	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	pNode->FuncID = function;
	memcpy(pNode->dstip, dstip, 20);
	pNode->dstport = dstport;
	UnmapViewOfFile(pNode);
	return NewNode;
}

HANDLE CComSocket::PackNode(unsigned int function, UINT nSocketPort)
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);
	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	memset(pNode, 0, sizeof(Node));
	pNode->FuncID =  function;
	pNode->bindport = nSocketPort;
	UnmapViewOfFile(pNode);
	return NewNode;
}

HANDLE CComSocket::PackNode(unsigned int function, LPCTSTR lpszHostAddress, UINT nHostPort)
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);
	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	memset(pNode, 0, sizeof(Node));
	pNode->FuncID = function;
	pNode->dstport = nHostPort;
	Tchar2char(pNode->dstip, lpszHostAddress);
	UnmapViewOfFile(pNode);
	return NewNode;
}

HANDLE CComSocket::PackNode(unsigned int function, CComSocket &rConnectedSocket)
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);
	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	pNode->FuncID = function;
	pNode->AcceptSockMark = rConnectedSocket.SockMark;
	memcpy(pNode->srcip, csrcip, 20);
	memcpy(pNode->dstip, cdstip, 20);
	pNode->srcport = csrcport;
	pNode->dstport = cdstport;
	UnmapViewOfFile(pNode);
	return NewNode;
}

HANDLE CComSocket::PackNode(unsigned int function, const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress)
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);
	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));	///<map  LocalNode
	pNode->FuncID = function;
	pNode->dstport = nHostPort;
	pNode->DataLen = nBufLen;
	Tchar2char(pNode->dstip, lpszHostAddress);
	pNode->Data = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, nBufLen, NULL);	///<Create DataHandle
	void  *pData = (void *)MapViewOfFile(pNode->Data, FILE_MAP_WRITE, 0, 0, nBufLen);///<map Data
	memcpy(pData, lpBuf, nBufLen);
	UnmapViewOfFile(pData);///<unmap Data	
	UnmapViewOfFile(pNode);
	return NewNode;
}

HANDLE CComSocket::PackNode(unsigned int function, const void* lpBuf, int nBufLen)
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);
	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));	///<map  LocalNode
	pNode->FuncID = function;
	memcpy(pNode->dstip, dstip, 20);
	pNode->dstport = dstport;
	pNode->DataLen = nBufLen;
	pNode->Data = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, nBufLen, NULL);	///<Create DataHandle
	void  *pData = (void *)MapViewOfFile(pNode->Data, FILE_MAP_WRITE, 0, 0, nBufLen);///<map Data
	memcpy(pData, lpBuf, nBufLen);
	UnmapViewOfFile(pData);///<unmap Data	
	UnmapViewOfFile(pNode);
	return NewNode;
}