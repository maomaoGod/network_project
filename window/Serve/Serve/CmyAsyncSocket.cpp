#include "stdafx.h"

#include "CmyAsyncSocket.h"


/**
* @author ACM2012
* @note  同步套接字构造函数,向协议栈注册套接字,创建写队列控制块和打开读队列控制块
*/

CmyAsyncSocket::CmyAsyncSocket()
{
	flag = false;
	regstruct myreg;
	WaitForSingleObject(Csemaphore, INFINITE);  ///<互斥信号量等待上一个套接字注册完成
	GetSockMark(SockMark, myreg);                    ///<获取读写命名队列管理模块命名和套接字标识
	WriteQueue = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Manager), myreg.WriteQueueName);///<创建写队列模块
	if (InitalWriteQueue() == false){                       ///<映射写队列管理模块到本地并初始化
		AfxMessageBox(_T("初始化写队列失败"));
		return;
	}
	ReleaseSemaphore(Psemaphore, 1, NULL);    ///<向协议栈注册套接字
	WaitForSingleObject(Ssemaphore, INFINITE);     ///<等待注册完成
	ReadQueue = OpenFileMapping(FILE_MAP_WRITE, FALSE, myreg.ReadQueueName);///<打开读队列模块
	if (!(WriteQueue || ReadQueue)){
		AfxMessageBox(_T("打开共享内存失败"));
		return;
	}
	if (InitalReadQueue() == false){                      ///<映射读队列模块到本地
		AfxMessageBox(_T("初始化读队列失败"));
		return;
	}
	flag = true;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewGetSockEventThread, (LPVOID) this, NULL, NULL);
}

CmyAsyncSocket::~CmyAsyncSocket()
{

}

DWORD WINAPI CmyAsyncSocket::NewGetSockEventThread(LPVOID lParam)
{
	CmyAsyncSocket *pthis = (CmyAsyncSocket *)lParam;
	pthis->GetSockEvent();
	return 0;
}

/**
* @author ACM2012
* @note   映射写队列控制块到进程，并初始化控制块中的值，包括读写节点个数，写方的进程ID，控制块中句柄的初值。为保证队列读写一致性，加入头部节点
*/

void CmyAsyncSocket::GetSockEvent()
{
	while (state)
	{
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//获取Cur映射内存块
		while (pCur->Next == NULL);//永久等待数据到来
		HANDLE Next;
		DuplicateHandle(SH, pCur->Next, CH, &Next, NULL, true, DUPLICATE_SAME_ACCESS);
		UnmapViewOfFile(pCur);

		PN pNode = (PN)MapViewOfFile(Next, FILE_MAP_WRITE, 0, 0, sizeof(Node));
		switch (pNode->FuncID){
		case SOCKSENDTO:
		case SOCKSEND: OnReceive(true); break;
		case SOCKCONNECT: OnAccept(true); break;
		case SOCKCLOSE: OnClose(true); break;
		default: RemoveNode(Next);  break;
		}
		UnmapViewOfFile(pNode);
		CloseHandle(Next);
	}
}

void   CmyAsyncSocket::RemoveNode( HANDLE Next)
{
	CloseHandle(pReadQueue->Cur);
	DuplicateHandle(SH, Next, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
	pReadQueue->cid++;
}

bool   CmyAsyncSocket::InitalWriteQueue()
{
	pWriteQueue = (PM)MapViewOfFile(WriteQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<映射写队列控制块到本地
	if (pWriteQueue == NULL)
		return PrintLog(_T("写控制块映射失败"), false);
	pWriteQueue->hid = 0;   ///<初始化写控制块参数
	pWriteQueue->cid = 0;
	pWriteQueue->writer = _getpid();
	pWriteQueue->Cur = NULL;

	HANDLE EmptyNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);///<创建一个空节点
	PN pNode = (PN)MapViewOfFile(EmptyNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));///<映射节点到本地
	memset(pNode, 0, sizeof(Node));
	UnmapViewOfFile(pNode);

	CH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pWriteQueue->writer);
	if (CH == NULL)
		return PrintLog(_T("打开协议进程句柄失败"), false);
	DuplicateHandle(CH, EmptyNode, CH, &pWriteQueue->Tail, NULL, true, DUPLICATE_SAME_ACCESS); ///<空节点加入链表
	DuplicateHandle(CH, EmptyNode, CH, &pWriteQueue->Head, NULL, true, DUPLICATE_SAME_ACCESS);
	CloseHandle(EmptyNode);
	if (!(pWriteQueue->Tail || pWriteQueue->Head))
		return PrintLog(_T("控制管理块初始化失败"), false);
	return true;
}

/**
* @author ACM2012
* @note   映射读队列控制块到进程，并初始化控制块中的值，包括读方进程ID
*/

bool   CmyAsyncSocket::InitalReadQueue()
{
	pReadQueue = (PM)MapViewOfFile(ReadQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<映射读队列控制块到本地
	if (pReadQueue == NULL)
		return PrintLog(_T("读队列映射失败"), false);
	pWriteQueue->reader = pReadQueue->writer;
	pReadQueue->reader = _getpid();
	SH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pReadQueue->writer);
	DuplicateHandle(SH, pReadQueue->Head, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);///<初始化读节点初值
	if (pReadQueue->Cur == NULL)
		return PrintLog(_T("读队列节点初始化失败"), false);
	return true;
}

bool  CmyAsyncSocket::AddToTail(HANDLE NewNode)
{
	PN  LastNode = (PN)MapViewOfFile(pWriteQueue->Tail, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	DuplicateHandle(CH, NewNode, CH, &LastNode->Next, NULL, true, DUPLICATE_SAME_ACCESS);
	UnmapViewOfFile(LastNode);
	CloseHandle(pWriteQueue->Tail);
	DuplicateHandle(CH, NewNode, CH, &pWriteQueue->Tail, NULL, true, DUPLICATE_SAME_ACCESS);
	if (pWriteQueue->Tail == NULL)
		return PrintLog(_T("添加到写队列节点失败"), false);
	return true;
}

bool CmyAsyncSocket::Listen()
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);//创建一个空节点\

	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	pNode->FuncID = SOCKLISTEN;
	pNode->SockMark = SockMark;
	UnmapViewOfFile(pNode);

	AddToTail(NewNode);
	CloseHandle(NewNode);
	return true;
}

/**
* @author ACM2012
* @param [in] lpBuf为应用层发送数据首地址,nBufLen为应用层发送的数据长度,nHostPort为数据发送的目的端口,lpszHostAddress为数据发送的目的IP
* @return 返回数据成功发送的大小
* @note
*/
int CmyAsyncSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress)
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);

	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));	///<map  LocalNode
	pNode->FuncID = SOCKSENDTO;
	pNode->SockMark = SockMark;
	pNode->dstport = nHostPort;
	pNode->DataLen = nBufLen;
	Tchar2char(pNode->dstip, lpszHostAddress);
	pNode->Data = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, nBufLen, NULL);	///<Create DataHandle
	void  *pData = (void *)MapViewOfFile(pNode->Data, FILE_MAP_WRITE, 0, 0, nBufLen);///<map Data
	memcpy(pData, lpBuf, nBufLen);
	UnmapViewOfFile(pData);///<unmap Data	
	UnmapViewOfFile(pNode);
	///<backpacp finished
	AddToTail(NewNode);
	CloseHandle(NewNode); ///<Close LocalHandle
	return nBufLen;
}

int    CmyAsyncSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags)
{
	if (done){
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//获取Cur映射内存块
		while (pCur->Next == NULL) //永久等待数据到来
			Sleep(10000);
		CloseHandle(pReadQueue->Cur);//释放Cur
		DuplicateHandle(SH, pCur->Next, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
		UnmapViewOfFile(pCur);

		PN pNode = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));
		if (pNode->FuncID = SOCKCLOSE)
			return 0;

		char2Tchar(rSocketAddress, pNode->srcip, 20);
		rSocketPort = pNode->srcport;

		HANDLE HData;
		DuplicateHandle(SH, pNode->Data, CH, &HData, NULL, true, DUPLICATE_SAME_ACCESS);
		pReadData = (char *)MapViewOfFile(HData, FILE_MAP_WRITE, 0, 0, pNode->DataLen);
		CloseHandle(HData);
		DataLen = pNode->DataLen;
		ReadDataLen = 0;
		UnmapViewOfFile(pNode);
		done = false;
	}
	if (!done){
		if (DataLen - ReadDataLen < nBufLen){       ///<
			memcpy(lpBuf, pReadData + ReadDataLen, DataLen - ReadDataLen);
			UnmapViewOfFile(pReadData);
			pReadQueue->cid++;
			done = true;
			return DataLen - ReadDataLen;
		}
		else {
			memcpy(lpBuf, pReadData, nBufLen);
			ReadDataLen += nBufLen;
			return nBufLen;
		}
	}
}

bool CmyAsyncSocket::Bind(UINT nSocketPort)
{

	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);

	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	pNode->FuncID = SOCKBIND;
	pNode->SockMark = SockMark;
	pNode->bindport = nSocketPort;
	UnmapViewOfFile(pNode);

	AddToTail(NewNode);
	CloseHandle(NewNode);
	return true;
}


bool CmyAsyncSocket::Create()
{
	return flag;
}

bool CmyAsyncSocket::Create(UINT nHostPort) //重载创建socket并绑定端口
{
	if (true == Bind(nHostPort) && flag)
		return true;
	return false;
}

void   CmyAsyncSocket::OnReceive(int nErrorCode)
{

}

int  CmyAsyncSocket::Receive(void* lpBuf, int nBufLen)
{
	if (done){
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//获取Cur映射内存块
		while (pCur->Next == NULL) //永久等待数据到来
			Sleep(10000);
		CloseHandle(pReadQueue->Cur);//释放Cur
		DuplicateHandle(SH, pCur->Next, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
		UnmapViewOfFile(pCur);

		PN pNode = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));
		if (pNode->FuncID = SOCKCLOSE)
			return 0;

		HANDLE HData;
		DuplicateHandle(SH, pNode->Data, CH, &HData, NULL, true, DUPLICATE_SAME_ACCESS);
		pReadData = (char *)MapViewOfFile(HData, FILE_MAP_WRITE, 0, 0, pNode->DataLen);
		CloseHandle(HData);
		DataLen = pNode->DataLen;
		ReadDataLen = 0;
		UnmapViewOfFile(pNode);
		done = false;
	}
	if (!done){
		if (DataLen - ReadDataLen < nBufLen){       ///<
			memcpy(lpBuf, pReadData + ReadDataLen, DataLen - ReadDataLen);
			UnmapViewOfFile(pReadData);
			pReadQueue->cid++;
			done = true;
			return DataLen - ReadDataLen;
		}
		else {
			memcpy(lpBuf, pReadData, nBufLen);
			ReadDataLen += nBufLen;
			return nBufLen;
		}
	}
}

void   CmyAsyncSocket::OnAccept(int nErrorCode)
{

}

void  CmyAsyncSocket::Accept(CmyAsyncSocket& rConnectedSocket)
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);//创建一个空节点

	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	pNode->SockMark = SockMark;
	pNode->FuncID = SOCKACCEPT;
	pNode->AcceptSockMark = rConnectedSocket.SockMark;
	memcpy(pNode->srcip, pConnQueue->srcip, 20);
	memcpy(pNode->dstip, pConnQueue->dstip, 20);
	pNode->srcport = pConnQueue->srcport;
	pNode->dstport = pConnQueue->dstport;
	memcpy(rConnectedSocket.srcip, pConnQueue->srcip, 20);
	memcpy(rConnectedSocket.srcip, pConnQueue->srcip, 20);
	rConnectedSocket.srcport = pConnQueue->srcport;
	rConnectedSocket.dstport = pConnQueue->dstport;
	UnmapViewOfFile(pNode);

	AddToTail(NewNode);
	CloseHandle(NewNode);//Close LocalHandle
}

void   CmyAsyncSocket::OnClose(int nErrorCode)
{

}

void CmyAsyncSocket::Close()
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);//创建一个空节点

	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	pNode->SockMark = SockMark;
	pNode->FuncID = SOCKCLOSE;
	UnmapViewOfFile(pNode);

	AddToTail(NewNode);
	CloseHandle(NewNode);
}

void   CmyAsyncSocket::OnSend(int nErrorCode)
{

}

int CmyAsyncSocket::Send(const void* lpBuf, int nBufLen) //发送数据
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);//创建一个空节点

	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	pNode->FuncID = SOCKSEND;
	pNode->SockMark = SockMark;
	memcpy(pNode->dstip, dstip, 20);
	pNode->dstport = dstport;
	pNode->DataLen = nBufLen;
	pNode->Data = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, nBufLen, NULL);
	void  *pData = (void *)MapViewOfFile(pNode->Data, FILE_MAP_WRITE, 0, 0, nBufLen);
	memcpy(pData, lpBuf, nBufLen);
	UnmapViewOfFile(pData);
	UnmapViewOfFile(pNode);

	AddToTail(NewNode);
	CloseHandle(NewNode);
	return nBufLen;
}

void   CmyAsyncSocket::OnConnect(int nErrorCode)
{

}

bool  CmyAsyncSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);

	PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	pNode->FuncID = SOCKCONNECT;
	pNode->SockMark = SockMark;
	pNode->dstport = nHostPort;
	Tchar2char(pNode->dstip, lpszHostAddress);
	UnmapViewOfFile(pNode);
	dstport = nHostPort;
	Tchar2char(dstip, lpszHostAddress);

	AddToTail(NewNode);
	CloseHandle(NewNode);
	return true;
}
