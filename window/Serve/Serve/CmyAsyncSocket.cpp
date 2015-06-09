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

	if (InitalWriteQueue(myreg) == false){                      ///<��ʼ��д����
		AfxMessageBox(_T("��ʼ��д����ʧ��"));
		return;
	}

	if (InitalReadQueue(myreg) == false){                      ///<��ʼ��������
		AfxMessageBox(_T("��ʼ��������ʧ��"));
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
* @note   ӳ��д���п��ƿ鵽���̣�����ʼ�����ƿ��е�ֵ��������д�ڵ������д���Ľ���ID�����ƿ��о���ĳ�ֵ��Ϊ��֤���ж�дһ���ԣ�����ͷ���ڵ�
*/
bool   CmyAsyncSocket::InitalWriteQueue(regstruct &myreg)
{

	WaitForSingleObject(Wsemaphore, INFINITE);  ///<�����ź����ȴ���һ���׽���ע�����

	GetSockMark(SockMark, myreg);                    ///<��ȡ��д�������й���ģ���������׽��ֱ�ʶ

	WriteQueue = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Manager), myreg.WriteQueueName);///<����д����ģ��
	pWriteQueue = (PM)MapViewOfFile(WriteQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<ӳ��д���п��ƿ鵽����	
	if (pWriteQueue == NULL)
		return PrintLog(_T("д���ƿ�ӳ��ʧ��"), false);
	pWriteQueue->writer = _getpid();
	pWriteQueue->hid = 0;   ///<��ʼ��д���ƿ����
	pWriteQueue->cid = 0;
	pWriteQueue->Cur = NULL;
	HANDLE EmptyNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);///<����һ���սڵ�
	PN pNode = (PN)MapViewOfFile(EmptyNode, FILE_MAP_WRITE, 0, 0, sizeof(Node));///<ӳ��ڵ㵽����
	memset(pNode, 0, sizeof(Node));
	CH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pWriteQueue->writer);
	if (CH == NULL)
		return PrintLog(_T("��Э����̾��ʧ��"), false);
	DuplicateHandle(CH, EmptyNode, CH, &pWriteQueue->Tail, NULL, true, DUPLICATE_SAME_ACCESS);  ///<���ƿ�ͷ��ָ��սڵ�
	DuplicateHandle(CH, EmptyNode, CH, &pWriteQueue->Head, NULL, true, DUPLICATE_SAME_ACCESS);///<���ƿ�β��ָ��սڵ�
	UnmapViewOfFile(pNode);///<��ʼ��Ϊ��
	CloseHandle(EmptyNode);///<�رտսڵ���,��ʱ�ڵ�����ü���Ϊ2��,�ʲ��ᱻ�ͷ�

	ReleaseSemaphore(Dsemaphore, 1, NULL);    ///<��Э��ջע���׽���
	return true;
}

/**
* @author ACM2012
* @note   ӳ������п��ƿ鵽���̣�����ʼ�����ƿ��е�ֵ��������������ID
*/

bool   CmyAsyncSocket::InitalReadQueue(regstruct &myreg)
{
	WaitForSingleObject(Rsemaphore, INFINITE);     ///<�ȴ�ע�����

	ReadQueue = OpenFileMapping(FILE_MAP_WRITE, FALSE, myreg.ReadQueueName);///<�򿪶�����ģ��
	if (!ReadQueue){
		AfxMessageBox(_T("�򿪹����ڴ�ʧ��"));
		return false;
	}
	pReadQueue = (PM)MapViewOfFile(ReadQueue, FILE_MAP_WRITE, 0, 0, sizeof(Manager));///<ӳ������п��ƿ鵽����
	if (pReadQueue == NULL)
		return PrintLog(_T("������ӳ��ʧ��"), false);
	pReadQueue->reader = _getpid();
	SH = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pReadQueue->writer);
	DuplicateHandle(SH, pReadQueue->Head, CH, &pReadQueue->Cur, NULL, true, DUPLICATE_SAME_ACCESS);///<��ʼ�����ڵ��ֵ
	if (pReadQueue->Cur == NULL)
		return PrintLog(_T("�����нڵ��ʼ��ʧ��"), false);
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
* @note   ӳ��д���п��ƿ鵽���̣�����ʼ�����ƿ��е�ֵ��������д�ڵ������д���Ľ���ID�����ƿ��о���ĳ�ֵ��Ϊ��֤���ж�дһ���ԣ�����ͷ���ڵ�
*/

void CmyAsyncSocket::GetSockEvent()
{
	while (state)
	{
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//��ȡCurӳ���ڴ��
		while (pCur->Next == NULL)//���õȴ����ݵ���
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
	PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//��ȡCurӳ���ڴ��
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
* @param [in] NewNode�´����Ľڵ���
* @return  ���ڵ�ɹ���ӵ�д����β��������true�����򷵻�true
* @note    �������´����Ľڵ���ӵ�д����β�������������ȳ�����
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
		return PrintLog(_T("��ӵ�д���нڵ�ʧ��"), false);
	return true;
}

/**
* @author ACM2012
* @return  ��������true
* @note    ��������һ����������ڵ㣬�Ӷ�����ӵ�д����β��
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
* @param [in] lpBufΪӦ�ò㷢�������׵�ַ,nBufLenΪӦ�ò㷢�͵����ݳ���,nHostPortΪ���ݷ��͵�Ŀ�Ķ˿�,lpszHostAddressΪ���ݷ��͵�Ŀ��IP
* @return �������ݳɹ����͵Ĵ�С
* @note   UDP���ݷ��ͺ�����Ӧ�ò㽫ip�Ͷ˿ں��Լ�ָ�����͵�������Ϊ�������ݸ��������������칲��ڵ㣬����Ӧ��������;����
* �����µĹ����ڴ������������ݲ��ֿ����������ڴ档���ڵ���ӵ�д���к���򷵻أ����ݷ��͹��̽������漸�����
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
* @param [in] lpBufΪӦ�ò㷢�������׵�ַ,nBufLenΪӦ�ò㷢�͵����ݳ���,nHostPortΪ���ݷ��͵�Ŀ�Ķ˿�,lpszHostAddressΪ���ݷ��͵�Ŀ��IP
* @return �������ݳɹ����͵Ĵ�С
* @note   UDP���ݷ��ͺ�����Ӧ�ò㽫ip�Ͷ˿ں��Լ�ָ�����͵�������Ϊ�������ݸ��������������칲��ڵ㣬����Ӧ��������;����
* �����µĹ����ڴ������������ݲ��ֿ����������ڴ档���ڵ���ӵ�д���к���򷵻أ����ݷ��͹��̽������漸�����
*/

int    CmyAsyncSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags)
{
	if (done){
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//��ȡCurӳ���ڴ��
		while (pCur->Next == NULL) //���õȴ����ݵ���
			Sleep(100);
		CloseHandle(pReadQueue->Cur);//�ͷ�Cur
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
* @param [in] nHostPortΪ���ݷ��͵�Ŀ�Ķ˿�
* @return  ���ɹ��󶨶˿ڣ����򷵻�true
* @note    �ú������׽��ְ���һ���̶��Ķ˿ڡ�����ʹ��Bind����������������׽��֡�
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
* @return  ���ɹ������׽��֣����򷵻�true
* @note   �ú��������׽����Ƿ񴴽��ɹ���־
*/
bool CmyAsyncSocket::Create()
{
	return flag;
}

/**
* @author ACM2012
* @param [in] nHostPort �����׽���ʱ�󶨵Ķ˿�
* @return ���׽��ִ����ɹ��ҳɹ��󶨶˿ڣ���������true�����򷵻�false
* @note   �ú���ΪCreate�����غ����������л���Ҫʵ�ֶ˿ڵİ�
*/
bool CmyAsyncSocket::Create(UINT nHostPort) //���ش���socket���󶨶˿�
{
	if (true == Bind(nHostPort) && flag)
		return true;
	return false;
}

/**
* @author ACM2012
* @param [in] nErrorCode������
* @note  �ú����������յ�һ�����Ķ�
*/
void   CmyAsyncSocket::OnReceive(int nErrorCode)
{

}

/**
* @author ACM2012
* @param [in] lpBufΪӦ�ó���������ݱ��ĵĻ������׵�ַ��nBufLen���յ����ݴ�С
* @return �������ؽ��յ������ݱ���С����Ӧ�ó�����Ҫ�����ݴ�С�����������е�ǰ���ݣ�
�����������ݲ����ؿ��������ݴ�С�����򿽱�Ӧ�ó�����Ҫ�����ݴ�С
* @note   �����ӽ������ݶ����п������ݵ�Ӧ�ó���Ľ��ջ�������
*/
int  CmyAsyncSocket::Receive(void* lpBuf, int nBufLen)
{
	if (done){
		PN pCur = (PN)MapViewOfFile(pReadQueue->Cur, FILE_MAP_WRITE, 0, 0, sizeof(Node));//��ȡCurӳ���ڴ��
		while (pCur->Next == NULL) //���õȴ����ݵ���
			Sleep(100);
		CloseHandle(pReadQueue->Cur);//�ͷ�Cur
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
* @param [in] nErrorCode������
* @note    �ú����������յ���������
*/
void   CmyAsyncSocket::OnAccept(int nErrorCode)
{

}

/**
* @author ACM2012
* @param [in] rConnectedSocket ��������������׽����շ����ݵ����׽���
* @note   �����ڽ��յ�һ����������ʱ�����ã�Ӧ�ó�����øú��������յ������������׽���ת������һ���׽��ִ����������
Ӧ���׽��ּ���������������
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
* @param [in] nErrorCode������
* @note  �ú����������յ��Է��׽��ֹر���Ϣ
*/
void   CmyAsyncSocket::OnClose(int nErrorCode)
{

}

/**
* @author ACM2012
* @note  �ú������͹ر��׽��ֱ���
*/
void CmyAsyncSocket::Close()
{
	HANDLE NewNode = PackNode(SOCKCLOSE);
	AddToTail(NewNode);
	CloseHandle(NewNode);
}

/**
* @author ACM2012
* @note  �ú���������ǰ�ɷ������ݱ���
*/
void   CmyAsyncSocket::OnSend(int nErrorCode)
{

}

/**
* @author ACM2012
* @param [in] lpBufΪӦ�ó��������ݱ��ĵĻ������׵�ַ��nBufLenΪ���͵����ݴ�С
* @return  �ɹ����͵����ݴ�С
* @note    ���������͵����ݿ��������Ͷ�����
*/
int CmyAsyncSocket::Send(const void* lpBuf, int nBufLen) //��������
{
	HANDLE NewNode = PackNode(SOCKSEND, lpBuf, nBufLen);
	AddToTail(NewNode);
	CloseHandle(NewNode);
	return nBufLen;
}

/**
* @author ACM2012
* @param [in] nErrorCode������
* @note    �������յ�����Ϣ˵��Connect�������
*/

void   CmyAsyncSocket::OnConnect(int nErrorCode)
{

}

/**
* @author ACM2012
* @param [in] lpszHostAddress �������ӵ�Ŀ��IP��ַ��nHostPort�������ӵ�Ŀ�Ķ˿�
* @note    �����������ӱ���
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
