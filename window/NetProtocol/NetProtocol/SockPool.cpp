#pragma once
#include "stdafx.h"
#include "SockPool.h"


/**
* @author ACM2012
* @note  SockPool构造函数,创建共享内存文件接收应用程序套接字注册,创建命名信号量来实现套接字注册的同步
*/
SockPool::SockPool()
{
	sockconnum = 0;
}

bool SockPool::Create(CString user)
{
	Wsemaphore = CreateSemaphore(NULL, 1, 100, user + _T("NetProtocolWsemaphore"));//创建信号量P
	Rsemaphore = CreateSemaphore(NULL, 0, 100, user + _T("NetProtocolRsemaphore"));//创建信号量C
	Dsemaphore = CreateSemaphore(NULL, 0, 100, user + _T("NetProtocolDsemaphore"));//创建信号量S
	MFile = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(regstruct), user + _T("NetProtocolListen"));///<创建注册文件
	//if (MFile == NULL)
		//AfxMessageBox(user + _T("NetProtocolListen") + _T("   failed"));
	//AfxMessageBox(user + _T("NetProtocolListen") + _T("   success"));
	SH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _getpid());///<获取本机IP
	CString soc;
	int len = user.GetLength();
	soc = user.Mid(len - 6, len);
	if (soc.Compare(_T("server"))==0){
		HANDLE Success = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, user + _T("NetProtocolCreate"));
		if (Success == NULL){
			AfxMessageBox(_T("参数传递失败!"));
			exit(0);
			return false;
		}
		ReleaseSemaphore(Success, 1, NULL);
	}
	preg = (regstruct *)MapViewOfFile(MFile, FILE_MAP_WRITE, 0, 0, sizeof(regstruct)); ///<映射注册文件
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewConnThread, (LPVOID) this, NULL, NULL);
	return true;
}

SockPool::~SockPool()
{

}

/**
* @author ACM2012
* @param [in] ErrorCause为打印的错误信息,returnvalue为返回的
* @return  返回值由输入参数returnvalue决定
* @note    函数为简化在程序运行过程中的异常处理。函数打印提示的错误信息，并返回指定的布尔变量值
*/

bool  SockPool::PrintLog(LPCTSTR ErrorCause, bool returnvalue)
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
bool  SockPool::AddToTail(PM &pWriteQueue, HANDLE NewNode)
{
	PN  LastNode = (PN)MapViewOfFile(pWriteQueue->Tail, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	DuplicateHandle(SH, NewNode, SH, &LastNode->Next, NULL, true, DUPLICATE_SAME_ACCESS);
	UnmapViewOfFile(LastNode);
	CloseHandle(pWriteQueue->Tail);
	pWriteQueue->Tail = NULL;///<添加节点到队列尾
	DuplicateHandle(SH,NewNode, SH, &pWriteQueue->Tail, NULL, true, DUPLICATE_SAME_ACCESS);
	PN phead = (PN)MapViewOfFile(pWriteQueue->Head, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	PN ptail = (PN)MapViewOfFile(pWriteQueue->Tail, FILE_MAP_WRITE, 0, 0, sizeof(Node));
	if (pWriteQueue->Tail == NULL)
		return PrintLog(_T("添加节点到写队列失败"),false);
	return true;
}

/**
* @author ACM2012
* @param [in] 需要清除节点的队列句柄
* @note    释放已经被读取的数据节点句柄，释放共享内存
*/

void   SockPool::ClearNode(PM &pWriteQueue)
{
	while (pWriteQueue->hid<pWriteQueue->cid) ///<循环读取以完成的节点数据
	{
		PN temp = (PN)MapViewOfFile(pWriteQueue->Head, FILE_MAP_WRITE, 0, 0, sizeof(Node));
		CloseHandle(pWriteQueue->Head);
		DuplicateHandle(SH, temp->Next, SH, &pWriteQueue->Head, NULL, true, DUPLICATE_SAME_ACCESS);
		CloseHandle(temp->Next);
		CloseHandle(temp->Data);///<关闭动态分配的共享内存句柄
		UnmapViewOfFile(temp);///<关闭节点文件本地映射
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
	struct Para *mypara = (struct Para *)lParam;
	SockPool *pthis = (SockPool *)mypara->pthis;
	pthis->ReadSock(mypara->CH, mypara->SockMark, mypara->Queue, mypara->pQueue,mypara->Destory);
	return 0;
}

/**
* @author ACM2012
* @param [in] CH 被服务程序句柄，pReadQueue 读队列控制块结构, ReadQueue 读队列控制块句柄,SockMark 套接字标志
* @note    函数的主要功能是从应用程序写队列中读取数据并传送到传输层。函数提取
*/
void SockPool::ReadSock(HANDLE CH, unsigned int SockMark, HANDLE ReadQueue, PM pReadQueue,HANDLE &CDestory)
{
	PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//获取Cur映射内存块
	transstruct  AppData;
	HANDLE HData = NULL;
	portin myportsrc;
    CString myid;
	while (SockMark2ReadState[SockMark]) { ///<当状态变为false时退出
		if (pCur->Next == NULL){
			Sleep(100);
			continue;
		}
		CloseHandle(pReadQueue->Cur);//释放Cur
		DuplicateHandle(CH, pCur->Next, SH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
		UnmapViewOfFile(pCur);
		pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));

		PN pNode = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));
		memset(&AppData, 0, sizeof(transstruct));///初始化为0
		
		myid.Format(_T("%s %d"), AfxGetApp()->m_lpCmdLine,pNode->FuncID);
		//AfxMessageBox(myid);
		switch (pNode->FuncID){///获取当前命令类型
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
		case SOCKSENDTO: ///<发送数据
			memcpy(AppData.dstip, pNode->dstip, 20);
			AppData.dstport = pNode->dstport;
			AppData.datalength = pNode->DataLen;///<填充源端口
			AppData.srcport = (Port2PortOut.find(SockMark2Port[SockMark]) == Port2PortOut.end()) ? SockMark2Port[SockMark] : Port2PortOut[SockMark2Port[SockMark]];
			AppData.function = pNode->FuncID;
			DuplicateHandle(CH, pNode->Data, SH, &HData, NULL, true, DUPLICATE_SAME_ACCESS);
			AppData.data = (char *)MapViewOfFile(HData, FILE_MAP_WRITE, 0, 0, pNode->DataLen);
			AfxGetApp()->m_pMainWnd->SendMessage(TRANSTOIP, (WPARAM)&AppData, (LPARAM)pNode->FuncID);
			break;
		case SOCKCONNECT:
		case SOCKCLOSE:
			memcpy(AppData.dstip, pNode->dstip, 20);
			AppData.dstport = pNode->dstport;
			AppData.srcport = SockMark2Port[SockMark];
			AppData.function = pNode->FuncID;
			//if (pNode->FuncID == SOCKCONNECT)
			AfxGetApp()->m_pMainWnd->SendMessage(TRANSTOIP, (WPARAM)&AppData, (LPARAM)pNode->FuncID);
			break;
		case SOCKLISTEN:
			AppData.srcport  = SockMark2Port[SockMark];
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
		if (pNode->FuncID == SOCKCLOSE){
			sockconnum--;
			UnmapViewOfFile(pNode);
			break;
		}
		UnmapViewOfFile(pNode);
		pReadQueue->cid++;
		
	}
	UnmapViewOfFile(pCur);  ///<释放读队列资源
	SockMark2ReadState.erase(SockMark);
	UnmapViewOfFile(pReadQueue);
	CloseHandle(ReadQueue);
	ReleaseSemaphore(CDestory,1,NULL);
	if (SockMark2State[SockMark])
		CloseSock(SockMark);
	else
		SockMark2State[SockMark] = true;
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
	pthis->WriteSock(mypara->CH,mypara->SockMark, mypara->Queue, mypara->pQueue,mypara->Destory);///<线程调用类函数
	return 0;
}

/**
* @author ACM2012
* @param [in] pNode新创建的节点句柄,SockMark套接字唯一标示符
* @note    函数的主要功能是将类中缓存的接收到的数据打包到新创建的节点
*/
bool   SockPool::SockDataToNode(PN pNode, unsigned int SockMark)///<将数据打包成节点形式
{
	bool tmpflag = true;
	transstruct *psockstruct = SockMark2SockStruct[SockMark];
	memcpy(pNode->dstip, psockstruct->dstip, 20);///<填充字段
	memcpy(pNode->srcip, psockstruct->srcip, 20);
	pNode->dstport = psockstruct->dstport;
	pNode->srcport = psockstruct->srcport;
	pNode->DataLen = psockstruct->datalength;
	pNode->FuncID = psockstruct->function;
	CString myid;
	myid.Format(_T("%d"), pNode->FuncID);
	//AfxMessageBox(myid);
	if (pNode->FuncID == SOCKSEND || pNode->FuncID == SOCKSENDTO)
		pNode->Data = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, pNode->DataLen, NULL);
	if (pNode->FuncID == SOCKCLOSE)
		tmpflag = false;
	void  *pData = (void *)MapViewOfFile(pNode->Data, FILE_MAP_WRITE, 0, 0, pNode->DataLen);
	if (psockstruct->data!=NULL)
	    memcpy(pData, psockstruct->data, pNode->DataLen);///<拷贝数据
	UnmapViewOfFile(pData);
	return tmpflag;
}

/**
* @author ACM2012
* @param [in] pReadQueue 读队列控制块结构, ReadQueue 读队列控制块句柄,SockMark 套接字标志
* @note    函数的主要功能是从应用程序写队列中读取数据并传送到传输层。函数提取
*/
void SockPool::WriteSock(HANDLE CH, unsigned int SockMark, HANDLE WriteQueue, PM pWriteQueue,HANDLE &SDestory)
{
	bool closeflag = false;
	int   readnum;
	while (SockMark2WriteState[SockMark]){
		HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);
		PN pNode = (PN)MapViewOfFile(NewNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));
        WaitForSingleObject(*SockMark2REvent[SockMark], INFINITE);///<等待传输层数据到来

		ClearNode(pWriteQueue);
		if (!SockDataToNode(pNode, SockMark))
			break;
		UnmapViewOfFile(pNode);
		SockMark2WEvent[SockMark]->SetEvent();
		AddToTail(pWriteQueue,NewNode);///<添加数据到写队列
		CloseHandle(NewNode);
	}
	ClearNode(pWriteQueue);///<释放写队列资源
	CloseHandle(pWriteQueue->Head);
	CloseHandle(pWriteQueue->Tail);
	CloseHandle(pWriteQueue->Cur);
	UnmapViewOfFile(pWriteQueue);
	CloseHandle(WriteQueue);
	SockMark2WriteState.erase(SockMark);
	SockMark2WEvent.erase(SockMark);
	if (SockMark2State[SockMark])
		CloseSock(SockMark);
	else
		SockMark2State[SockMark] = true;
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
	pthis->Connect();///<调用监听套接字注册函数
	return 0;
}


/**
* @author ACM2012
* @param [in] SockMark 套接字唯一标识符
* @note 注册套接字时调用，函数根据套接字标识分配套接字资源
*/
void  SockPool::AllocResource(unsigned int SockMark)///<分配资源
{
	Port2SockMark[nPort] = SockMark;
	SockMark2Port[SockMark] = nPort++;///<端口号自增
	SockMark2ReadState[SockMark] = true;///<初始化读写状态
	SockMark2WriteState[SockMark] = true;
	SockMark2State[SockMark] = false;
	SockMark2SockStruct[SockMark] = new transstruct();
	SockMark2WEvent[SockMark] = new CEvent();///<初始化读写信号量
	SockMark2WEvent[SockMark]->SetEvent();
	SockMark2REvent[SockMark] = new CEvent();
	SockMark2REvent[SockMark]->ResetEvent();
}


/**
* @author ACM2012
* @param [in] mypara 线程参数结构，CH 被服务程序句柄，Queue 被操作队列句柄，SockMark 套接字唯一标识符
* @note 注册套接字时调用，函数根据套接字标识分配套接字资源
*/
void  SockPool::InitalThreadPara(Para &mypara, HANDLE CH, HANDLE Queue, PM pQueue, unsigned int SockMark,HANDLE &Destory)
{
	mypara.CH = CH;///<初始化线程调用参数集合
	mypara.Queue = Queue;
	mypara.pQueue = pQueue;
	mypara.pthis = this;
	mypara.SockMark = SockMark;
	DuplicateHandle(SH, Destory, SH, &mypara.Destory, NULL, true, DUPLICATE_SAME_ACCESS);
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
			HANDLE CDestory, SDestory;
			struct Para rPara, wPara;

			AllocResource(preg->SockMark);///<为套接字分配资源
			ReadQueue = OpenFileMapping(FILE_MAP_WRITE, FALSE, preg->WriteQueueName);
			InitalReadQueue(ReadQueue,pReadQueue,CH);///<初始化读队列
			CDestory = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, preg->CDestoryName);
			WriteQueue = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Manager), preg->ReadQueueName);
			InitalWriteQueue(WriteQueue, pWriteQueue);///<初始化写队列
			SDestory = CreateSemaphore(NULL, 0, 100, preg->SDestoryName);
			InitalThreadPara(rPara, CH, ReadQueue, pReadQueue, preg->SockMark,CDestory);
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewReadThread, (LPVOID)&rPara, NULL, NULL);
			InitalThreadPara(wPara, CH, WriteQueue, pWriteQueue, preg->SockMark,SDestory);
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewWriteThread, (LPVOID)&wPara, NULL, NULL);
			ReleaseSemaphore(Rsemaphore, 1, NULL);///<释放读信号量
			sockconnum++;
			//AfxMessageBox(preg->CDestoryName);
			//AfxMessageBox(preg->SDestoryName);
			//AfxMessageBox(preg->ReadQueueName);
			//AfxMessageBox(preg->WriteQueueName);
			CString pid;
			pid.Format(_T("%d"), preg->SockMark);
			//AfxMessageBox(pid);
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
	CH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pReadQueue->writer);///<获取进程句柄（权限控制）
	DuplicateHandle(CH, pReadQueue->Head, SH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
	if (pReadQueue->Cur == NULL)
		return PrintLog(_T("读队列节点初始化失败"), false);
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
		return PrintLog(_T("写控制块映射失败"), false);
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
		return PrintLog(_T("控制管理块初始化失败"), false);
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
	memcpy(tempsrc.srcip, pmysock->srcip, 20); ///<根据源端口源地址目的端口找到通信端口
	tempsrc.srcport = pmysock->srcport;
	tempsrc.dstport = pmysock->dstport;  ///<映射到真正连接的
	nPort = (PortIn2ScokMark.find(tempsrc) == PortIn2ScokMark.end()) ? pmysock->dstport : SockMark2Port[PortIn2ScokMark[tempsrc]];
	if (Port2SockMark.find(nPort) == Port2SockMark.end())
		return;
	unsigned int SockMark = Port2SockMark[nPort];///<获取目标端口的套接字标志
	WaitForSingleObject(*SockMark2WEvent[SockMark], INFINITE);///
	memcpy(SockMark2SockStruct[SockMark], pmysock, sizeof(transstruct));
	SockMark2REvent[SockMark]->SetEvent();
	delete psock;
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
	SockMark2ReadState.erase(SockMark);
	SockMark2WriteState.erase(SockMark);
	return true;
}