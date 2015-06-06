#include "stdafx.h"

#include "CmyAsyncSocket.h"


/**
* @author ACM2012
* @note  ͬ���׽��ֹ��캯��,��Э��ջע���׽���,����д���п��ƿ�ʹ򿪶����п��ƿ�
*/

CmyAsyncSocket::CmyAsyncSocket()
{
	flag = false;
	regstruct myreg;
	WaitForSingleObject(Csemaphore, INFINITE);  ///<�����ź����ȴ���һ���׽���ע�����
	GetSockMark(SockMark, myreg);                    ///<��ȡ��д�������й���ģ���������׽��ֱ�ʶ
	WriteQueue = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Manager), myreg.WriteQueueName);///<����д����ģ��
	if (InitalWriteQueue() == false){                       ///<ӳ��д���й���ģ�鵽���ز���ʼ��
		AfxMessageBox(_T("��ʼ��д����ʧ��"));
		return;
	}
	ReleaseSemaphore(Psemaphore, 1, NULL);    ///<��Э��ջע���׽���
	WaitForSingleObject(Ssemaphore, INFINITE);     ///<�ȴ�ע�����
	ReadQueue = OpenFileMapping(FILE_MAP_WRITE, FALSE, myreg.ReadQueueName);///<�򿪶�����ģ��
	if (!(WriteQueue || ReadQueue)){
		AfxMessageBox(_T("�򿪹����ڴ�ʧ��"));
		return;
	}
	if (InitalReadQueue() == false){                      ///<ӳ�������ģ�鵽����
		AfxMessageBox(_T("��ʼ��������ʧ��"));
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
* @note   ӳ��д���п��ƿ鵽���̣�����ʼ�����ƿ��е�ֵ��������д�ڵ������д���Ľ���ID�����ƿ��о���ĳ�ֵ��Ϊ��֤���ж�дһ���ԣ�����ͷ���ڵ�
*/

void CmyAsyncSocket::GetSockEvent()
{
	while (state)
	{
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//��ȡCurӳ���ڴ��
		while (pCur->Next == NULL);//���õȴ����ݵ���
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
	pWriteQueue = (PM)MapViewOfFile(WriteQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<ӳ��д���п��ƿ鵽����
	if (pWriteQueue == NULL)
		return PrintLog(_T("д���ƿ�ӳ��ʧ��"), false);
	pWriteQueue->hid = 0;   ///<��ʼ��д���ƿ����
	pWriteQueue->cid = 0;
	pWriteQueue->writer = _getpid();
	pWriteQueue->Cur = NULL;

	HANDLE EmptyNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);///<����һ���սڵ�
	PN pNode = (PN)MapViewOfFile(EmptyNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));///<ӳ��ڵ㵽����
	memset(pNode, 0, sizeof(Node));
	UnmapViewOfFile(pNode);

	CH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pWriteQueue->writer);
	if (CH == NULL)
		return PrintLog(_T("��Э����̾��ʧ��"), false);
	DuplicateHandle(CH, EmptyNode, CH, &pWriteQueue->Tail, NULL, true, DUPLICATE_SAME_ACCESS); ///<�սڵ��������
	DuplicateHandle(CH, EmptyNode, CH, &pWriteQueue->Head, NULL, true, DUPLICATE_SAME_ACCESS);
	CloseHandle(EmptyNode);
	if (!(pWriteQueue->Tail || pWriteQueue->Head))
		return PrintLog(_T("���ƹ�����ʼ��ʧ��"), false);
	return true;
}

/**
* @author ACM2012
* @note   ӳ������п��ƿ鵽���̣�����ʼ�����ƿ��е�ֵ��������������ID
*/

bool   CmyAsyncSocket::InitalReadQueue()
{
	pReadQueue = (PM)MapViewOfFile(ReadQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<ӳ������п��ƿ鵽����
	if (pReadQueue == NULL)
		return PrintLog(_T("������ӳ��ʧ��"), false);
	pWriteQueue->reader = pReadQueue->writer;
	pReadQueue->reader = _getpid();
	SH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pReadQueue->writer);
	DuplicateHandle(SH, pReadQueue->Head, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);///<��ʼ�����ڵ��ֵ
	if (pReadQueue->Cur == NULL)
		return PrintLog(_T("�����нڵ��ʼ��ʧ��"), false);
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
		return PrintLog(_T("��ӵ�д���нڵ�ʧ��"), false);
	return true;
}

bool CmyAsyncSocket::Listen()
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);//����һ���սڵ�\

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
* @param [in] lpBufΪӦ�ò㷢�������׵�ַ,nBufLenΪӦ�ò㷢�͵����ݳ���,nHostPortΪ���ݷ��͵�Ŀ�Ķ˿�,lpszHostAddressΪ���ݷ��͵�Ŀ��IP
* @return �������ݳɹ����͵Ĵ�С
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
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//��ȡCurӳ���ڴ��
		while (pCur->Next == NULL) //���õȴ����ݵ���
			Sleep(10000);
		CloseHandle(pReadQueue->Cur);//�ͷ�Cur
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

bool CmyAsyncSocket::Create(UINT nHostPort) //���ش���socket���󶨶˿�
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
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//��ȡCurӳ���ڴ��
		while (pCur->Next == NULL) //���õȴ����ݵ���
			Sleep(10000);
		CloseHandle(pReadQueue->Cur);//�ͷ�Cur
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
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);//����һ���սڵ�

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
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);//����һ���սڵ�

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

int CmyAsyncSocket::Send(const void* lpBuf, int nBufLen) //��������
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);//����һ���սڵ�

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
