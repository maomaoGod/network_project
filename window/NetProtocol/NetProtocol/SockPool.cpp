#pragma once
#include "stdafx.h"
#include "SockPool.h"


/**
* @author ACM2012
* @note  SockPool���캯��,���������ڴ��ļ�����Ӧ�ó����׽���ע��,���������ź�����ʵ���׽���ע���ͬ��
*/
SockPool::SockPool()
{
	Wsemaphore = CreateSemaphore(NULL, 1, 100, _T("NetProtocolWsemaphore"));//�����ź���P
	Rsemaphore  = CreateSemaphore(NULL, 0, 100,  _T("NetProtocolRsemaphore"));//�����ź���C
	Dsemaphore  = CreateSemaphore(NULL, 0, 100,  _T("NetProtocolDsemaphore"));//�����ź���S
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
* @param [in] ErrorCauseΪ
* @return ����FALSE˵���½ڵ���ӵ�д����βʧ�ܣ�����TRUE˵����ӵ�д����β�ɹ�
* @note    ����������Ҫ�ǽ�����õĴ������������д����
*/

bool  SockPool::PrintLog(LPCTSTR ErrorCause,bool returnvalue)
{
	AfxMessageBox(ErrorCause);
	return returnvalue;
}

/**
* @author ACM2012
* @param [in] pWriteQueue�׽���д���п��ƿ�,NewNode�������ѳ�ʼ�����½ڵ�
* @return ����FALSE˵���½ڵ���ӵ�д����βʧ�ܣ�����TRUE˵����ӵ�д����β�ɹ�
* @note    ����������Ҫ�ǽ�����õĴ������������д����
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
		return PrintLog(_T("��ӵ�д���нڵ�ʧ��"),false);
	return true;
}

/**
* @author ACM2012
* @param [in] ��Ҫ����ڵ�Ķ��о��
* @note    �ͷ��Ѿ�����ȡ�����ݽڵ������ͷŹ����ڴ�
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
* @param [in] IParam ���ݸ��׽��ֶ��̵߳Ĳ��������������п��ƿ�ṹpReadQueue,�����п��ƿ���ReadQueue,�׽��ֱ�־SockMark
* @return  ReadSock�����������򷵻�0
* @note    ��������Ҫ������ʹ���´������̵߳��ö����к���
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
* @param [in] CH �������������pReadQueue �����п��ƿ�ṹ, ReadQueue �����п��ƿ���,SockMark �׽��ֱ�־
* @note    ��������Ҫ�����Ǵ�Ӧ�ó���д�����ж�ȡ���ݲ����͵�����㡣������ȡ
*/
void SockPool::ReadSock(HANDLE CH,unsigned int SockMark,HANDLE ReadQueue,PM pReadQueue)
{
	PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//��ȡCurӳ���ڴ��
	transstruct  AppData;
	HANDLE HData=NULL;
	portin myportsrc;
	while (SockMark2ReadState[SockMark]) {

		if (pCur->Next == NULL)
			continue;
		CloseHandle(pReadQueue->Cur);//�ͷ�Cur
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
* @param [in] IParam ���ݸ��׽���д�̵߳Ĳ���
* @return  �߳̽�������0
* @note    ��������Ҫ������ʹ���´������߳�д����
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
* @param [in] pNode�´����Ľڵ���,SockMark�׽���Ψһ��ʾ��
* @note    ��������Ҫ�����ǽ����л���Ľ��յ������ݴ�����´����Ľڵ�
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
* @param [in] pReadQueue �����п��ƿ�ṹ, ReadQueue �����п��ƿ���,SockMark �׽��ֱ�־
* @note    ��������Ҫ�����Ǵ�Ӧ�ó���д�����ж�ȡ���ݲ����͵�����㡣������ȡ
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
* @param [in] IParam ���ݸ������׽���ע���̵߳Ĳ���
* @return  �߳̽�������0
* @note    ��������Ҫ������ʹ���´������߳�ִ���׽���ע�����
*/
DWORD WINAPI SockPool::NewConnThread(LPVOID lParam)
{
	SockPool *pthis = (SockPool *)lParam;
	pthis->Connect();
	return 0;
}


/**
* @author ACM2012
* @param [in] SockMark �׽���Ψһ��ʶ��
* @note ע���׽���ʱ���ã����������׽��ֱ�ʶ�����׽�����Դ
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
* @param [in] mypara �̲߳����ṹ��CH �������������Queue ���������о����SockMark �׽���Ψһ��ʶ��
* @note ע���׽���ʱ���ã����������׽��ֱ�ʶ�����׽�����Դ
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
* @note  �����߳�ע�ᣬ�����߳�ע�ᣬ������д���в���ʼ���������̶߳Զ��н��ж�д
*/
void SockPool::Connect()
{
		while (state){
			WaitForSingleObject(Dsemaphore, INFINITE);//�ȴ�Ӧ�ó�����������
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
* @param [in] ReadQueue �����о��, pReadQueuekָ������п��ƿ��ָ�룬CH �����������
* @return  �߳̽�������0
* @note   ������ʼ�������У���Ҫ��ɶ�����ָ��ĳ�ʼ��
*/

bool   SockPool::InitalReadQueue(HANDLE ReadQueue, PM &pReadQueue, HANDLE &CH)
{
	pReadQueue = (PM)MapViewOfFile(ReadQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<ӳ������п��ƿ鵽����
	pReadQueue->reader = _getpid();
	CH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pReadQueue->writer);
	DuplicateHandle(CH, pReadQueue->Head, SH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);
	if (pReadQueue->Cur == NULL)
		return PrintLog(_T("�����нڵ��ʼ��ʧ��"),false);
	return true;
}

/**
* @author ACM2012
* @param [in] WriteQueue �����о��, pWriteQueuekָ������п��ƿ��ָ�룬CH �����������
* @return  �߳̽�������0
* @note    ������ʼ��д���У�д������Э��ջ�������󲿷ֳ�ʼ���ڱ�����ɣ����������ֻ��ʼ����ָ��Ͷ�����id
*/

bool   SockPool::InitalWriteQueue(HANDLE WriteQueue, PM &pWriteQueue)
{
	pWriteQueue = (PM)MapViewOfFile(WriteQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<ӳ��д���п��ƿ鵽����
	if (pWriteQueue == NULL)
		return PrintLog(_T("д���ƿ�ӳ��ʧ��"),false);
	pWriteQueue->hid = 0;   ///<��ʼ��д���ƿ����
	pWriteQueue->cid = 0;
	pWriteQueue->writer = _getpid();
	pWriteQueue->Cur = NULL;

	HANDLE EmptyNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);///<����һ���սڵ�
	PN pNode = (PN)MapViewOfFile(EmptyNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));///<ӳ��ڵ㵽����
	memset(pNode, 0, sizeof(Node));
	UnmapViewOfFile(pNode);

	DuplicateHandle(SH, EmptyNode, SH, &pWriteQueue->Tail, NULL, true, DUPLICATE_SAME_ACCESS); ///<�սڵ��������
	DuplicateHandle(SH, EmptyNode, SH, &pWriteQueue->Head, NULL, true, DUPLICATE_SAME_ACCESS);
	CloseHandle(EmptyNode);
	if (!(pWriteQueue->Tail || pWriteQueue->Head))
		return PrintLog(_T("���ƹ�����ʼ��ʧ��"),false);
	return true;
}

/**
* @author ACM2012
* @param [in] psock ����㷢�͵ı��Ķνṹָ��
* @note    ������㴫�͵����ݿ������׽��ֻ�����
*/

void    SockPool::SendToApp(void *psock)
{
	portin        tempsrc;
	unsigned short nPort;
	transstruct *pmysock = (transstruct *)psock;
	memcpy(tempsrc.srcip, pmysock->srcip, 20); //����Դ�˿�Դ��ַĿ�Ķ˿��ҵ�ͨ�Ŷ˿�
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
* @param [in] SockMark �׽���Ψһ��ʾ��
* @note    ע���׽���ʱ���ô˺������ͷŷ�����׽��ֵ���Դ
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