#pragma once
#include "stdafx.h"
#include "SockPool.h"


/**
* @author ACM2012
* @note  SockPool构造函数,创建共享内存文件接收应用程序套接字注册,创建命名信号量来实现套接字注册的同步
*/
SockPool::SockPool()
{
	Wsemaphore = CreateSemaphore(NULL, 1, 100, _T("NetProtocolWsemaphore"));//创建信号量P
	Rsemaphore  = CreateSemaphore(NULL, 0, 100,  _T("NetProtocolRsemaphore"));//创建信号量C
	Dsemaphore  = CreateSemaphore(NULL, 0, 100,  _T("NetProtocolDsemaphore"));//创建信号量S
	MFile = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(regstruct), _T("NetProtocolListen"));
	SH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _getpid());
	preg = (regstruct *)MapViewOfFile(MFile, FILE_MAP_WRITE, 0, 0, sizeof(regstruct));
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewConnThread, (LPVOID) this, NULL, NULL);
}																																																																											

SockPool::~SockPool()
{

}

/**
* @author ACM2012
* @param [in] ErrorCause为
* @return 返回FALSE说明新节点添加到写队列尾失败，返回TRUE说明添加到写队列尾成功
* @note    函数功能主要是将打包好的传输层数据送入写队列
*/

bool  SockPool::PrintLog(LPCTSTR ErrorCause,bool returnvalue)
{
	AfxMessageBox(ErrorCause);
	return returnvalue;
}

/**
* @author ACM2012
* @param [in] pWriteQueue套接字写队列控制块,NewNode创建的已初始化的新节点
* @return 返回FALSE说明新节点添加到写队列尾失败，返回TRUE说明添加到写队列尾成功
* @note    函数功能主要是将打包好的传输层数据送入写队列
*/
bool  SockPool::AddToTail(PM &pWriteQueue,HANDLE NewNode)
{
	PN  LastNode = (PN)MapViewOfFile(pWriteQueue->Tail, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	DuplicateHandle(SH, NewNode, SH, &LastNode->Next, NULL, true, DUPLICATE_SAME_ACCESS);
	UnmapViewOfFile(LastNode);
	CloseHandle(pWriteQueue->Tail);
	pWriteQueue->Tail = NULL;
	DuplicateHandle(SH,NewNode, SH, &pWriteQueue->Tail, NULL, true, DUPLICATE_SAME_ACCESS);
	PN phead = (PN)MapViewOfFile(pWriteQueue->Head, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	PN ptail = (PN)MapViewOfFile(pWriteQueue->Tail, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	if (pWriteQueue->Tail == NULL)
		return PrintLog(_T("添加到写队列节点失败"),false);
	return true;
}

/**
* @author ACM2012
* @param [in] 需要清除节点的队列句柄
* @note    释放已经被读取的数据节点句柄，释放共享内存
*/ 

void   SockPool::ClearNode(PM &pWriteQueue)
{
	while (pWriteQueue->hid<pWriteQueue->cid)
	{
		PN temp = (PN)MapViewOfFile(pWriteQueue->Head, FILE_MAP_WRITE, 0, 0, sizeof(Node));
		CloseHandle(pWriteQueue->Head);
		DuplicateHandle(SH, temp->Next, SH, &pWriteQueue->Head, NULL, true, DUPLICATE_SAME_ACCESS);
		CloseHandle(temp->Next);
		CloseHandle(temp->Data);
		UnmapViewOfFile(temp);
		pWriteQueue->hid++;
	}
}

/**
* @author ACM2012
* @param [in] IParam 传递给套接字读线程的参数，包括读队列控制块结构pReadQueue,读队列控制块句柄ReadQueue,套接字标志SockMark
* @return  ReadSock函数结束程序返回0
* @note    函数的主要功能是使用新创建的线程调用读队列函数
*/
DWORD WINAPI SockPool::NewReadThread(LPVOID lParam)
{
	struct Para *mypara= (struct Para *)lParam;
	SockPool *pthis = (SockPool *)mypara->pthis;
	pthis->ReadSock(mypara->CH,mypara->SockMark,mypara->Queue,mypara->pQueue);
	return 0;
}

/**
* @author ACM2012
* @param [in] CH 被服务程序句柄，pReadQueue 读队列控制块结构, ReadQueue 读队列控制块句柄,SockMark 套接字标志
* @note    函数的主要功能是从应用程序写队列中读取数据并传送到传输层。函数提取
*/
void SockPool::ReadSock(HANDLE CH,unsigned int SockMark,HANDLE ReadQueue,PM pReadQueue)
{
	PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//获取Cur映射内存块
	transstruct  AppData;
	HANDLE HData=NULL;
	portin myportsrc;
	while (SockMark2ReadState[SockMark]) {

		if (pCur->Next == NULL)
			continue;
		CloseHandle(pReadQueue->Cur);//释放Cur
		DuplicateHandle(CH, pCur->Next, SH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
		UnmapViewOfFile(pCur);
		pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));

		PN pNode = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));
		memset(&AppData, 0, sizeof(transstruct));
		switch (pNode->FuncID){
		case SOCKBIND:
			    Port2SockMark.erase(SockMark2Port[SockMark]);
				SockMark2Port.erase(SockMark);
				SockMark2Port[SockMark] = pNode->bindport;
				Port2SockMark[pNode->bindport] = SockMark;
				break;
		case SOCKACCEPT:
			    memcpy(myportsrc.srcip, pNode->srcip, 20);
		    	myportsrc.srcport = pNode->srcport;
			    myportsrc.dstport = SockMark2Port[SockMark];
			    PortIn2ScokMark[myportsrc] = pNode->AcceptSockMark;
				Port2PortOut[SockMark2Port[pNode->AcceptSockMark]] = pNode->dstport;
				break;
		case SOCKSEND:
		case SOCKSENDTO:
			    memcpy(AppData.dstip, pNode->dstip,20);
			    AppData.dstport = pNode->dstport;
			    AppData.datalength = pNode->DataLen;
				AppData.srcport = (Port2PortOut.find(SockMark2Port[SockMark]) == Port2PortOut.end()) ? SockMark2Port[SockMark] : Port2PortOut[SockMark2Port[SockMark]];
				AppData.function = pNode->FuncID;
				DuplicateHandle(CH,pNode->Data, SH, &HData, NULL, true, DUPLICATE_SAME_ACCESS);
			    AppData.data = (char *) MapViewOfFile(HData, FILE_MAP_WRITE, 0, 0, pNode->DataLen);
				AfxGetApp()->m_pMainWnd->SendMessage(TRANSTOIP, (WPARAM)&AppData, (LPARAM)pNode->FuncID);
				break;
		case SOCKCONNECT:
		case SOCKCLOSE:
		    	memcpy(AppData.dstip, pNode->dstip,20);
			    AppData.dstport = pNode->dstport;
			    AppData.srcport = SockMark2Port[SockMark];
				AppData.function = pNode->FuncID;
				AfxGetApp()->m_pMainWnd->SendMessage(TRANSTOIP, (WPARAM)&AppData, (LPARAM)pNode->FuncID);
				break;
		case SOCKLISTEN:
			     AppData.srcport = SockMark2Port[SockMark];
				 AppData.function = pNode->FuncID;
				 AfxGetApp()->m_pMainWnd->SendMessage(TRANSTOIP, (WPARAM)&AppData, (LPARAM)pNode->FuncID);
				 break;
		default:
			     break;
		}
		if (AppData.data != NULL){
			UnmapViewOfFile(AppData.data);
			AppData.data = NULL;
		}
		if (HData != NULL){
			CloseHandle(HData);
			HData = NULL;
		}
		UnmapViewOfFile(pNode);
		pReadQueue->cid++;
	}
	SockMark2WriteState.erase(SockMark);
	UnmapViewOfFile(ReadQueue);
	CloseHandle(ReadQueue);
}

/**
* @author ACM2012
* @param [in] IParam 传递给套接字写线程的参数
* @return  线程结束返回0
* @note    函数的主要功能是使用新创建的线程写队列
*/
DWORD WINAPI SockPool::NewWriteThread(LPVOID lParam)
{
	struct Para *mypara = (struct Para *)lParam;
	SockPool *pthis = (SockPool *)mypara->pthis;
	pthis->WriteSock(mypara->CH,mypara->SockMark, mypara->Queue, mypara->pQueue);
	return 0;
}

/**
* @author ACM2012
* @param [in] pNode新创建的节点句柄,SockMark套接字唯一标示符
* @note    函数的主要功能是将类中缓存的接收到的数据打包到新创建的节点
*/
void   SockPool::SockDataToNode(PN pNode, unsigned int SockMark)
{
	transstruct *psockstruct = SockMark2SockStruct[SockMark];
	memcpy(pNode->dstip, psockstruct->dstip, 20);
	memcpy(pNode->srcip, psockstruct->srcip, 20);
	pNode->dstport = psockstruct->dstport;
	pNode->srcport = psockstruct->srcport;
	pNode->DataLen = psockstruct->datalength;
	pNode->FuncID = psockstruct->function;
	if (pNode->FuncID == SOCKSEND || pNode->FuncID == SOCKSENDTO)
		pNode->Data = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, pNode->DataLen, NULL);
	void  *pData = (void *)MapViewOfFile(pNode->Data, FILE_MAP_WRITE, 0, 0, pNode->DataLen);
	memcpy(pData, psockstruct->data, pNode->DataLen);
	UnmapViewOfFile(pData);
}

/**
* @author ACM2012
* @param [in] pReadQueue 读队列控制块结构, ReadQueue 读队列控制块句柄,SockMark 套接字标志
* @note    函数的主要功能是从应用程序写队列中读取数据并传送到传输层。函数提取
*/
void SockPool::WriteSock(HANDLE CH,unsigned int SockMark, HANDLE WriteQueue, PM pWriteQueue)
{
	
	while (SockMark2WriteState[SockMark]){		
	    HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);
		PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));

        WaitForSingleObject(*SockMark2REvent[SockMark], INFINITE);

		ClearNode(pWriteQueue);
		SockDataToNode(pNode, SockMark);
		UnmapViewOfFile(pNode);

		SockMark2WEvent[SockMark]->SetEvent();
		AddToTail(pWriteQueue,NewNode);
		CloseHandle(NewNode);
	}
	SockMark2WriteState.erase(SockMark);
	UnmapViewOfFile(pWriteQueue);
	CloseHandle(WriteQueue);
}

/**
* @author ACM2012
* @param [in] IParam 传递给接受套接字注册线程的参数
* @return  线程结束返回0
* @note    函数的主要功能是使用新创建的线程执行套接字注册监听
*/
DWORD WINAPI SockPool::NewConnThread(LPVOID lParam)
{
	SockPool *pthis = (SockPool *)lParam;
	pthis->Connect();
	return 0;
}


/**
* @author ACM2012
* @param [in] SockMark 套接字唯一标识符
* @note 注册套接字时调用，函数根据套接字标识分配套接字资源
*/
void  SockPool::AllocResource(unsigned int SockMark)
{
	Port2SockMark[nPort] = SockMark;
	SockMark2Port[SockMark] = nPort++;
	SockMark2ReadState[SockMark] = true;
	SockMark2WriteState[SockMark] = true;
	SockMark2SockStruct[SockMark] = new transstruct();
	SockMark2WEvent[SockMark] = new CEvent();
	SockMark2WEvent[SockMark]->SetEvent();
	SockMark2REvent[SockMark] = new CEvent();
	SockMark2REvent[SockMark]->ResetEvent();
}


/**
* @author ACM2012
* @param [in] mypara 线程参数结构，CH 被服务程序句柄，Queue 被操作队列句柄，SockMark 套接字唯一标识符
* @note 注册套接字时调用，函数根据套接字标识分配套接字资源
*/
void  SockPool::InitalThreadPara(Para &mypara, HANDLE CH, HANDLE Queue, PM pQueue,unsigned int SockMark)
{
	mypara.CH = CH;
	mypara.Queue = Queue;
	mypara.pQueue = pQueue;
	mypara.pthis = this;
	mypara.SockMark = SockMark;
}

/**
* @author ACM2012
* @note  监听线程注册，若有线程注册，创建读写队列并初始化，创建线程对队列进行读写
*/
void SockPool::Connect()
{
		while (state){
			WaitForSingleObject(Dsemaphore, INFINITE);//等待应用程序请求连接
			PM pReadQueue, pWriteQueue;
	        HANDLE CH,ReadQueue, WriteQueue;
			struct Para rPara, wPara;

			AllocResource(preg->SockMark);

			ReadQueue = OpenFileMapping(FILE_MAP_WRITE, FALSE, preg->WriteQueueName);
			InitalReadQueue(ReadQueue,pReadQueue,CH);
			WriteQueue = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Manager), preg->ReadQueueName);
			InitalWriteQueue(WriteQueue, pWriteQueue);

			InitalThreadPara(rPara, CH, ReadQueue, pReadQueue, preg->SockMark);
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewReadThread, (LPVOID)&rPara, NULL, NULL);
			InitalThreadPara(wPara, CH, WriteQueue, pWriteQueue, preg->SockMark);
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewWriteThread, (LPVOID)&wPara, NULL, NULL);
			ReleaseSemaphore(Rsemaphore, 1, NULL);

			AfxGetApp()->m_pMainWnd->SendMessage(SOCKSTATEUPDATE, 1);
		}
}


/**
* @author ACM2012
* @param [in] ReadQueue 读队列句柄, pReadQueuek指向读队列控制块的指针，CH 被服务程序句柄
* @return  线程结束返回0
* @note   函数初始化读队列，主要完成读队列指针的初始化
*/

bool   SockPool::InitalReadQueue(HANDLE ReadQueue, PM &pReadQueue, HANDLE &CH)
{
	pReadQueue = (PM)MapViewOfFile(ReadQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<映射读队列控制块到本地
	pReadQueue->reader = _getpid();
	CH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pReadQueue->writer);
	DuplicateHandle(CH, pReadQueue->Head, SH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
	if (pReadQueue->Cur == NULL)
		return PrintLog(_T("读队列节点初始化失败"),false);
	return true;
}

/**
* @author ACM2012
* @param [in] WriteQueue 读队列句柄, pWriteQueuek指向读队列控制块的指针，CH 被服务程序句柄
* @return  线程结束返回0
* @note    函数初始化写队列，写队列由协议栈创建，大部分初始化在本处完成，被服务程序只初始化读指针和读进程id
*/

bool   SockPool::InitalWriteQueue(HANDLE WriteQueue, PM &pWriteQueue)
{
	pWriteQueue = (PM)MapViewOfFile(WriteQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<映射写队列控制块到本地
	if (pWriteQueue == NULL)
		return PrintLog(_T("写控制块映射失败"),false);
	pWriteQueue->hid = 0;   ///<初始化写控制块参数
	pWriteQueue->cid = 0;
	pWriteQueue->writer = _getpid();
	pWriteQueue->Cur = NULL;

	HANDLE EmptyNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);///<创建一个空节点
	PN pNode = (PN)MapViewOfFile(EmptyNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));///<映射节点到本地
	memset(pNode, 0, sizeof(Node));
	UnmapViewOfFile(pNode);

	DuplicateHandle(SH, EmptyNode, SH, &pWriteQueue->Tail, NULL, true, DUPLICATE_SAME_ACCESS); ///<空节点加入链表
	DuplicateHandle(SH, EmptyNode, SH, &pWriteQueue->Head, NULL, true, DUPLICATE_SAME_ACCESS);
	CloseHandle(EmptyNode);
	if (!(pWriteQueue->Tail || pWriteQueue->Head))
		return PrintLog(_T("控制管理块初始化失败"),false);
	return true;
}

/**
* @author ACM2012
* @param [in] psock 传输层发送的报文段结构指针
* @note    将传输层传送的数据拷贝到套接字缓冲区
*/

void    SockPool::SendToApp(void *psock)
{
	portin        tempsrc;
	unsigned short nPort;
	transstruct *pmysock = (transstruct *)psock;
	memcpy(tempsrc.srcip, pmysock->srcip, 20); //根据源端口源地址目的端口找到通信端口
	tempsrc.srcport = pmysock->srcport;
	tempsrc.dstport = pmysock->dstport;
	nPort = (PortIn2ScokMark.find(tempsrc) == PortIn2ScokMark.end()) ? pmysock->dstport : SockMark2Port[PortIn2ScokMark[tempsrc]];
	if (Port2SockMark.find(nPort) == Port2SockMark.end())
		return;
	unsigned int SockMark = Port2SockMark[nPort];
	WaitForSingleObject(*SockMark2WEvent[SockMark],INFINITE);
	memcpy(SockMark2SockStruct[SockMark], pmysock, sizeof(transstruct));
	SockMark2REvent[SockMark]->SetEvent();
}

/**
* @author ACM2012
* @param [in] SockMark 套接字唯一标示符
* @note    注销套接字时调用此函数，释放分配给套接字的资源
*/
bool   SockPool::CloseSock(unsigned int SockMark)
{
	Port2SockMark.erase(SockMark2Port[SockMark]);
	SockMark2Port.erase(SockMark);
	delete SockMark2SockStruct[SockMark];
	SockMark2SockStruct.erase(SockMark);
	CloseHandle(*SockMark2REvent[SockMark]);
	CloseHandle(*SockMark2WEvent[SockMark]);
	SockMark2REvent.erase(SockMark);
	SockMark2WEvent.erase(SockMark);
	SockMark2ReadState[SockMark] = false;
	SockMark2WriteState[SockMark] = false;
	WaitForSingleObject(SockMark2ReadThread[SockMark],INFINITE);
	WaitForSingleObject(SockMark2WriteThread[SockMark],INFINITE);
	SockMark2ReadState.erase(SockMark);
	SockMark2WriteThread.erase(SockMark);
	return true;
}