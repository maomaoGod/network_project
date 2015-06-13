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
	sockstate =  INIT_FLAG;
	readstate =  READBEGIN;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewGetSockEventThread, (LPVOID)this, NULL, NULL);
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
	pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node)); 
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
	{   ///<pCur指向已读的最后一个节点
		while (pCur->Next == NULL) ///<永久等待数据到来
			Sleep(100);
		CloseHandle(pReadQueue->Cur);
		pReadQueue->Cur = NULL;///未读的第一个节点
		DuplicateHandle(SH, pCur->Next, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
		UnmapViewOfFile(pCur);
		pCur = NULL;
		pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));///<映射未读的第一个节点
		switch (pCur->FuncID){
		case SOCKSENDTO: 
		case SOCKSEND: OnReceive(true); readstate = READBEGIN;  break;
		case SOCKCONNECT: OnConnect(true); break;
		case SOCKCLOSE: OnClose(true); break;
		case SOCKACCEPT: memcpy(csrcip, pCur->srcip, 20);  ///<保存原数据IP,端口信息
			                            memcpy(cdstip, pCur->dstip, 20);
										csrcport = pCur->srcport;
										cdstport = pCur->dstport;
			                            OnAccept(true);
		default:  break;
		}
		pReadQueue->cid++;  ///<已读完成，已读节点数+1 
	}
}

void   CmyAsyncSocket::RemoveRead()
{
	if (pCur->Next == NULL) return;
	CloseHandle(pReadQueue->Cur);
	pReadQueue->Cur = NULL;
	DuplicateHandle(SH, pCur->Next, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
	UnmapViewOfFile(pCur);
	pCur = NULL;
	pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));
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
	if (sockstate != INIT_FLAG){
		LastError = SOCK_LS_FAILED;
		return  true;
	}
	HANDLE NewNode = PackNode(SOCKLISTEN);
	AddToTail(NewNode);
	CloseHandle(NewNode);
	sockstate = LISTEN_FLAG;
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
	if (sockstate != INIT_FLAG && sockstate != UDP_FLAG) {
		LastError = SOCK_NOT_UDP;
		return 0;
	}
	HANDLE NewNode = PackNode(SOCKSENDTO, lpBuf, nBufLen, nHostPort, lpszHostAddress);
	AddToTail(NewNode);
	CloseHandle(NewNode); ///<Close LocalHandle
	sockstate = UDP_FLAG;
	return nBufLen;
}

/**
* @author ACM2012
* @param [in] lpBuf为应用层发送数据首地址,nBufLen为应用层发送的数据长度,nHostPort为数据发送的目的端口,lpszHostAddress为数据发送的目的IP
* @return 返回数据成功发送的大小
* @note   UDP数据发送函数。应用层将ip和端口号以及指定发送的数据作为参数传递给函数，函数构造共享节点，将对应参数填入;函数
* 创建新的共享内存区，并将数据部分拷贝到共享内存。将节点添加到写队列后程序返回，数据发送过程交给下面几层完成
*/

/*int    CmyAsyncSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags)
{

	if (sockstate != INIT_FLAG&&sockstate != UDP_FLAG) {
		LastError = SOCK_NOT_UDP;
		return 0;
	}
	sockstate = UDP_FLAG;

	if (readstate == READBEGIN){///<新的数据节点
		return RcvBegin(lpBuf, nBufLen);
	}
	else if (readstate == READMID){///<上次节点数据未读完
		if (DataLen - ReadDataLen < nBufLen){       ///<数据一次可以接受完成
			memcpy(lpBuf, pReadData + ReadDataLen, DataLen - ReadDataLen);
			UnmapViewOfFile(pReadData);
			pReadQueue->cid++;
			readstate = READFIN;
			return DataLen - ReadDataLen;
		}
		else {                                                 ///<数据长度过大
			memcpy(lpBuf, pReadData, nBufLen);
			ReadDataLen += nBufLen;
			readstate = READMID;
			return nBufLen;
		}
	}
	else {
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));
		while (pCur->Next == NULL);
		CloseHandle(pReadQueue->Cur);
		pReadQueue->Cur = NULL;
		DuplicateHandle(SH, pCur->Next, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
		UnmapViewOfFile(pCur);
		return RcvBegin(lpBuf, nBufLen);
	}
}*/

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
	if (sockstate != INIT_FLAG&&sockstate!=TCP_FLAG) {
		LastError = SOCK_NOT_TCP;
		return 0;
	}

	sockstate = TCP_FLAG; 

	if (readstate==READBEGIN){///<新的数据节点
		return RcvBegin(lpBuf, nBufLen);
	}
	else if (readstate == READMID){ ///<上次节点数据未读完

		if (DataLen - ReadDataLen < nBufLen){       ///<数据一次可以接受完成
			memcpy(lpBuf, pReadData + ReadDataLen, DataLen - ReadDataLen);
			UnmapViewOfFile(pReadData);
			pReadQueue->cid++;
			readstate = READFIN;
			return DataLen - ReadDataLen;
		}
		else {                                                 ///<数据长度过大
			memcpy(lpBuf, pReadData, nBufLen);
			ReadDataLen += nBufLen;
			readstate = READMID;
			return nBufLen;
		}

	}
	else {  ///<READFIN只会出现在循环收数据时的情形
		while (pCur->Next == NULL) Sleep(100);
		RemoveRead();  ///<移除前一个已读节点
		return RcvBegin(lpBuf, nBufLen);
	}
}


int  CmyAsyncSocket::RcvBegin(void* lpBuf, int nBufLen)///<当收到一个新的数据节点
{
	PN pNode = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	if (pNode->FuncID == SOCKCLOSE){
		LastError = SOCKCLOSE;
		return 0;  ///<收到连接关闭，返回0
	}
	else if (pNode->FuncID != SOCKSEND)///<收到错误数据，返回0
		return 0;

	HANDLE HData;
	DuplicateHandle(SH, pNode->Data, CH, &HData, NULL, true, DUPLICATE_SAME_ACCESS);
	pReadData = (char *)MapViewOfFile(HData, FILE_MAP_WRITE, 0, 0, pNode->DataLen);
	CloseHandle(HData);
	DataLen = pNode->DataLen;
	ReadDataLen = 0;
	UnmapViewOfFile(pNode);

	if (DataLen - ReadDataLen < nBufLen){       ///<数据一次可以接受完成
		memcpy(lpBuf, pReadData + ReadDataLen, DataLen - ReadDataLen);
		UnmapViewOfFile(pReadData);
		readstate = READFIN;
		return DataLen - ReadDataLen;
	}
	else {                                                 ///<数据长度过大
		memcpy(lpBuf, pReadData, nBufLen);
		ReadDataLen += nBufLen;
		readstate = READMID;
		return nBufLen;
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

bool CmyAsyncSocket::Accept(CmyAsyncSocket& rConnectedSocket)
{
	if (sockstate != LISTEN_FLAG){
		LastError = SOCK_NOT_LISTEN;
		return false;
	}
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
	sockstate = CLOSE_FLAG;
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
	if (sockstate != INIT_FLAG && sockstate != TCP_FLAG){
		LastError = SOCK_NOT_TCP;
		return 0;
	}
	if (sockstate == CLOSE_FLAG){
		LastError = SOCK_IS_CLOSED;
		return 0;
	}
	HANDLE NewNode = PackNode(SOCKSEND, lpBuf, nBufLen);
	AddToTail(NewNode);
	CloseHandle(NewNode);
	sockstate = TCP_FLAG;
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
	if (sockstate != INIT_FLAG){
		LastError = SOCK_NOT_TCP;
		return false;
	}
	HANDLE  NewNode = PackNode(SOCKCONNECT, lpszHostAddress, nHostPort);
	AddToTail(NewNode);
	CloseHandle(NewNode);
	Tchar2char(dstip, lpszHostAddress);
	dstport = nHostPort;
	return WaitForSockEvent(SOCKCONNECT);
}

bool CmyAsyncSocket::WaitForSockEvent(unsigned int SOCKEVENT)
{
	bool rvalue;
	while (pCur->Next == NULL) ///<永久等待数据到来
		Sleep(100);
	RemoveRead();///<pCur指向下一个节点
	rvalue = (pCur->FuncID == SOCKEVENT) ? true : false;///<查看是否是目的事件，若是,返回true,否则返回false
	pReadQueue->cid++;
	return rvalue;
}