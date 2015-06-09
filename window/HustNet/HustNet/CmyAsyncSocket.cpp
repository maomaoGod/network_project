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

	if (InitalWriteQueue(myreg) == false){                      ///<初始化写队列
		AfxMessageBox(_T("初始化写队列失败"));
		return;
	}

	if (InitalReadQueue(myreg) == false){                      ///<初始化读队列
		AfxMessageBox(_T("初始化读队列失败"));
		return;
	}
	Pthread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewGetSockEventThread, (LPVOID) this, NULL, NULL);
	flag = true;
}

CmyAsyncSocket::~CmyAsyncSocket()
{

}

/**
* @author ACM2012
* @note   映射写队列控制块到进程，并初始化控制块中的值，包括读写节点个数，写方的进程ID，控制块中句柄的初值。为保证队列读写一致性，加入头部节点
*/
bool   CmyAsyncSocket::InitalWriteQueue(regstruct &myreg)
{

	WaitForSingleObject(Wsemaphore, INFINITE);  ///<互斥信号量等待上一个套接字注册完成

	GetSockMark(SockMark, myreg);                    ///<获取读写命名队列管理模块命名和套接字标识

	WriteQueue = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Manager), myreg.WriteQueueName);///<创建写队列模块
	pWriteQueue = (PM)MapViewOfFile(WriteQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<映射写队列控制块到本地	
	if (pWriteQueue == NULL)
		return PrintLog(_T("写控制块映射失败"), false);
	pWriteQueue->writer = _getpid();
	pWriteQueue->hid = 0;   ///<初始化写控制块参数
	pWriteQueue->cid = 0;
	pWriteQueue->Cur = NULL;
	HANDLE EmptyNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);///<创建一个空节点
	PN pNode = (PN)MapViewOfFile(EmptyNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));///<映射节点到本地
	memset(pNode, 0, sizeof(Node));
	CH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pWriteQueue->writer);
	if (CH == NULL)
		return PrintLog(_T("打开协议进程句柄失败"), false);
	DuplicateHandle(CH, EmptyNode, CH, &pWriteQueue->Tail, NULL, true, DUPLICATE_SAME_ACCESS);  ///<控制块头部指向空节点
	DuplicateHandle(CH, EmptyNode, CH, &pWriteQueue->Head, NULL, true, DUPLICATE_SAME_ACCESS);///<控制块尾部指向空节点
	UnmapViewOfFile(pNode);///<初始化为空
	CloseHandle(EmptyNode);///<关闭空节点句柄,此时节点的引用计数为2，,故不会被释放

	ReleaseSemaphore(Dsemaphore, 1, NULL);    ///<向协议栈注册套接字
	return true;
}

/**
* @author ACM2012
* @note   映射读队列控制块到进程，并初始化控制块中的值，包括读方进程ID
*/

bool   CmyAsyncSocket::InitalReadQueue(regstruct &myreg)
{
	WaitForSingleObject(Rsemaphore, INFINITE);     ///<等待注册完成

	ReadQueue = OpenFileMapping(FILE_MAP_WRITE, FALSE, myreg.ReadQueueName);///<打开读队列模块
	if (!ReadQueue){
		AfxMessageBox(_T("打开共享内存失败"));
		return false;
	}
	pReadQueue = (PM)MapViewOfFile(ReadQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<映射读队列控制块到本地
	if (pReadQueue == NULL)
		return PrintLog(_T("读队列映射失败"), false);
	pReadQueue->reader = _getpid();
	SH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pReadQueue->writer);
	DuplicateHandle(SH, pReadQueue->Head, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);///<初始化读节点初值
	if (pReadQueue->Cur == NULL)
		return PrintLog(_T("读队列节点初始化失败"), false);
	ReleaseSemaphore(Wsemaphore, 1, NULL);
	return true;
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
		while (pCur->Next == NULL)//永久等待数据到来
			Sleep(100);
		HANDLE Next;
		DuplicateHandle(SH, pCur->Next, CH, &Next, NULL, true, DUPLICATE_SAME_ACCESS);
		UnmapViewOfFile(pCur);

		PN pNode = (PN)MapViewOfFile(Next, FILE_MAP_WRITE, 0, 0, sizeof(Node));
		switch (pNode->FuncID){
		case SOCKSENDTO: 
		case SOCKSEND: OnReceive(true); break;
		case SOCKCONNECT: OnAccept(true); break;
		case SOCKCLOSE: OnClose(true); break;
		default:  RemoveRead(); break;
		}
		UnmapViewOfFile(pNode);
		CloseHandle(Next);
	}
}

void   CmyAsyncSocket::RemoveRead()
{
	HANDLE Next;
	PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//获取Cur映射内存块
	while (pCur->Next == NULL);
	DuplicateHandle(SH, pCur->Next, CH, &Next, NULL, true, DUPLICATE_SAME_ACCESS);
	UnmapViewOfFile(pCur);
	CloseHandle(pReadQueue->Cur);
	pReadQueue->Cur = NULL;
	DuplicateHandle(SH, Next, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
	pReadQueue->cid++;
}

/**
* @author ACM2012
* @param [in] NewNode新创建的节点句柄
* @return  若节点成功添加到写链表尾部，返回true，否则返回true
* @note    函数将新创建的节点添加到写队列尾部，构造先入先出队列
*/
bool  CmyAsyncSocket::AddToTail(HANDLE NewNode)
{
	PN  LastNode = (PN)MapViewOfFile(pWriteQueue->Tail, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	DuplicateHandle(CH, NewNode, CH, &LastNode->Next, NULL, true, DUPLICATE_SAME_ACCESS);
	UnmapViewOfFile(LastNode);
	CloseHandle(pWriteQueue->Tail);
	pWriteQueue->Tail = NULL;
	DuplicateHandle(CH, NewNode, CH, &pWriteQueue->Tail, NULL, true, DUPLICATE_SAME_ACCESS);
	if (pWriteQueue->Tail == NULL)
		return PrintLog(_T("添加到写队列节点失败"), false);
	return true;
}

/**
* @author ACM2012
* @return  函数返回true
* @note    函数创建一个监听命令节点，加队列添加到写队列尾部
*/
bool CmyAsyncSocket::Listen()
{
	HANDLE NewNode = PackNode(SOCKLISTEN);
	AddToTail(NewNode);
	CloseHandle(NewNode);
	return true;
}

/**
* @author ACM2012
* @param [in] lpBuf为应用层发送数据首地址,nBufLen为应用层发送的数据长度,nHostPort为数据发送的目的端口,lpszHostAddress为数据发送的目的IP
* @return 返回数据成功发送的大小
* @note   UDP数据发送函数。应用层将ip和端口号以及指定发送的数据作为参数传递给函数，函数构造共享节点，将对应参数填入;函数
* 创建新的共享内存区，并将数据部分拷贝到共享内存。将节点添加到写队列后程序返回，数据发送过程交给下面几层完成
*/
int CmyAsyncSocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress)
{
	HANDLE NewNode = PackNode(SOCKSENDTO, lpBuf, nBufLen, nHostPort, lpszHostAddress);
	AddToTail(NewNode);
	CloseHandle(NewNode); ///<Close LocalHandle
	return nBufLen;
}

/**
* @author ACM2012
* @param [in] lpBuf为应用层发送数据首地址,nBufLen为应用层发送的数据长度,nHostPort为数据发送的目的端口,lpszHostAddress为数据发送的目的IP
* @return 返回数据成功发送的大小
* @note   UDP数据发送函数。应用层将ip和端口号以及指定发送的数据作为参数传递给函数，函数构造共享节点，将对应参数填入;函数
* 创建新的共享内存区，并将数据部分拷贝到共享内存。将节点添加到写队列后程序返回，数据发送过程交给下面几层完成
*/

int    CmyAsyncSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags)
{
	if (done){
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//获取Cur映射内存块
		while (pCur->Next == NULL) //永久等待数据到来
			Sleep(100);
		CloseHandle(pReadQueue->Cur);//释放Cur
		pReadQueue = NULL;
		DuplicateHandle(SH, pCur->Next, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
		UnmapViewOfFile(pCur);
		pReadQueue->cid++;
		PN pNode = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));
		if (pNode->FuncID = SOCKCLOSE){
			char2Tchar(rSocketAddress, pNode->srcip, 20);
			rSocketPort = pNode->srcport;
			return 0;
		}
		else if (pNode->FuncID != SOCKSEND)
			RemoveRead();

		memcpy(srcip, pNode->srcip, 20);
		srcport = pNode->srcport;

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
		char2Tchar(rSocketAddress, srcip, 20);
		rSocketPort = srcport;

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

/**
* @author ACM2012
* @param [in] nHostPort为数据发送的目的端口
* @return  若成功绑定端口，程序返回true
* @note    该函数将套接字绑定在一个固定的端口。若不使用Bind函数，将随机分配套接字。
*/

bool CmyAsyncSocket::Bind(UINT nSocketPort)
{
	HANDLE NewNode = PackNode(SOCKBIND, nSocketPort);
	AddToTail(NewNode);
	CloseHandle(NewNode);
	return true;
}

/**
* @author ACM2012
* @return  若成功创建套接字，程序返回true
* @note   该函数返回套接字是否创建成功标志
*/
bool CmyAsyncSocket::Create()
{
	return flag;
}

/**
* @author ACM2012
* @param [in] nHostPort 创建套接字时绑定的端口
* @return 若套接字创建成功且成功绑定端口，函数返回true，否则返回false
* @note   该函数为Create的重载函数，函数中还需要实现端口的绑定
*/
bool CmyAsyncSocket::Create(UINT nHostPort) //重载创建socket并绑定端口
{
	if (true == Bind(nHostPort) && flag)
		return true;
	return false;
}

/**
* @author ACM2012
* @param [in] nErrorCode错误码
* @note  该函数表明接收到一个报文段
*/
void   CmyAsyncSocket::OnReceive(int nErrorCode)
{

}

/**
* @author ACM2012
* @param [in] lpBuf为应用程序接收数据报文的缓冲区首地址，nBufLen接收的数据大小
* @return 函数返回接收到的数据报大小，若应用程序需要的数据大小超过缓冲区中当前数据，
拷贝所有数据并返回拷贝的数据大小；否则拷贝应用程序需要的数据大小
* @note   函数从接收数据队列中拷贝数据到应用程序的接收缓冲区中
*/
int  CmyAsyncSocket::Receive(void* lpBuf, int nBufLen)
{
	if (done){
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//获取Cur映射内存块
		while (pCur->Next == NULL) //永久等待数据到来
			Sleep(100);
		CloseHandle(pReadQueue->Cur);//释放Cur
		pReadQueue = NULL;
		DuplicateHandle(SH, pCur->Next, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
		UnmapViewOfFile(pCur);
		pReadQueue->cid++;
		PN pNode = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));
		if (pNode->FuncID = SOCKCLOSE){
			return 0;
		}
		else if (pNode->FuncID != SOCKSEND)
			RemoveRead();

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

/**
* @author ACM2012
* @param [in] nErrorCode错误码
* @note    该函数表明接收到请求连接
*/
void   CmyAsyncSocket::OnAccept(int nErrorCode)
{

}

/**
* @author ACM2012
* @param [in] rConnectedSocket 负责和请求连接套接字收发数据的新套接字
* @note   函数在接收到一个请求连接时被调用，应用程序调用该函数将接收到的请求连接套接字转交给另一个套接字处理，处理完成
应用套接字继续监听请求连接
*/

void  CmyAsyncSocket::Accept(CmyAsyncSocket& rConnectedSocket)
{
	HANDLE NewNode = PackNode(SOCKACCEPT, rConnectedSocket);
	AddToTail(NewNode);
	memcpy(rConnectedSocket.dstip, csrcip, 20);
	rConnectedSocket.dstport = csrcport;
	CloseHandle(NewNode);//Close LocalHandle
}

/**
* @author ACM2012
* @param [in] nErrorCode错误码
* @note  该函数表明接收到对方套接字关闭消息
*/
void   CmyAsyncSocket::OnClose(int nErrorCode)
{

}

/**
* @author ACM2012
* @note  该函数发送关闭套接字报文
*/
void CmyAsyncSocket::Close()
{
	HANDLE NewNode = PackNode(SOCKCLOSE);
	AddToTail(NewNode);
	CloseHandle(NewNode);
}

/**
* @author ACM2012
* @note  该函数表明当前可发送数据报文
*/
void   CmyAsyncSocket::OnSend(int nErrorCode)
{

}

/**
* @author ACM2012
* @param [in] lpBuf为应用程序发送数据报文的缓冲区首地址，nBufLen为发送的数据大小
* @return  成功发送的数据大小
* @note    函数将发送的数据拷贝到发送队列中
*/
int CmyAsyncSocket::Send(const void* lpBuf, int nBufLen) //发送数据
{
	HANDLE NewNode = PackNode(SOCKSEND, lpBuf, nBufLen);
	AddToTail(NewNode);
	CloseHandle(NewNode);
	return nBufLen;
}

/**
* @author ACM2012
* @param [in] nErrorCode错误码
* @note    函数接收到该消息说明Connect连接完成
*/

void   CmyAsyncSocket::OnConnect(int nErrorCode)
{

}

/**
* @author ACM2012
* @param [in] lpszHostAddress 请求连接的目的IP地址，nHostPort请求连接的目的端口
* @note    函数请求连接报文
*/
bool  CmyAsyncSocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	HANDLE  NewNode = PackNode(SOCKCONNECT, lpszHostAddress, nHostPort);
	AddToTail(NewNode);
	CloseHandle(NewNode);
	Tchar2char(dstip, lpszHostAddress);
	dstport = nHostPort;
	return true;
}
