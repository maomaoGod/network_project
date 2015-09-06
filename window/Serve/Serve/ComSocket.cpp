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
		MFile = OpenFileMapping(FILE_MAP_WRITE, FALSE, NowUser + _T("NetProtocolListen"));///<��ע�������ļ�
		if (NULL == MFile){
			AfxMessageBox(_T("�������ļ�ʧ��"));
			return;
		}
		preg = (regstruct*)MapViewOfFile(MFile, FILE_MAP_WRITE, 0, 0, sizeof(regstruct));///<ӳ��ע�������ļ�������
		Wsemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, NowUser + _T("NetProtocolWsemaphore"));///<��Ӧ�ó����ź���
		Rsemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, NowUser + _T("NetProtocolRsemaphore"));///<��Э���������ź���
		Dsemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, NowUser + _T("NetProtocolDsemaphore"));///<��Э�����ɹ��ź���
		if (!(preg||Wsemaphore||Rsemaphore)){    
			AfxMessageBox(_T("��Э���������ź���ʧ��"));
			return;
		}
		memset(preg->ReadQueueName, 0, sizeof(preg->ReadQueueName));
		memset(preg->WriteQueueName, 0, sizeof(preg->WriteQueueName));
		memset(preg->CDestoryName, 0, sizeof(preg->CDestoryName));
		memset(preg->SDestoryName, 0, sizeof(preg->SDestoryName));
		CString Tmp;
		Tmp = NowUser + _T("ReadQueue");
		memcpy(preg->ReadQueueName, Tmp.GetBuffer(), sizeof(TCHAR)*wcslen(Tmp.GetBuffer()));///<������ע���ļ���������
		Tmp = NowUser + _T("WriteQueue");
		memcpy(preg->WriteQueueName, Tmp.GetBuffer(), sizeof(TCHAR)*wcslen(Tmp.GetBuffer()));///<������ע���ļ�д������
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
* @param [in] sockmark���ص��׽���Ψһ��ʶ��
* @note    ���������׽���Ψһ��ʶ��������Ψһ��ʾ����ӵ���д�������У�����Ψһ�Ķ�д������
*/
void  CComSocket::GetSockMark(unsigned int &sockmark, regstruct &myreg)
{
	TCHAR Buf[7];
	int temp;
	int Wlen, Rlen, Clen, Dlen;
	SockNum = SockNum + 1;
	temp = SockNum;
	for (int index = 6; index >=0 ; index--)///<�׽��ֱ�־תΪ�̶���7λ�ַ���
	{
		Buf[index] = temp % 10 + _T('0');
		temp /= 10;
	}
	Wlen = wcslen(preg->WriteQueueName);
	Rlen = wcslen(preg->ReadQueueName);
	Clen = wcslen(preg->CDestoryName);
	Dlen = wcslen(preg->SDestoryName);
	memcpy(preg->WriteQueueName+Wlen, Buf, sizeof(Buf));///<�����׽��ֱ�־��д����
	memcpy(preg->ReadQueueName+Rlen, Buf, sizeof(Buf));///<�����׽��ֱ�־����������
	memcpy(preg->CDestoryName + Clen, Buf, sizeof(Buf));
	memcpy(preg->SDestoryName + Dlen, Buf, sizeof(Buf));
	preg->SockMark = SockNum;//ע���ļ���ֵ�׽��ֱ�־ 
	sockmark = SockNum;///<�����׽���Ψһ��ʾ��
	memcpy(&myreg, preg, sizeof(regstruct));//�����׽��ֹ��������
}

void  CComSocket::Tchar2char(LPSTR dst, LPCTSTR src)//Tchar ת��Ϊchar 
{
	int iLength = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, src, -1, dst, iLength, NULL, NULL);
}

/**
* @author ACM2012
* @note   �������������ݴ���ʹ��char���ݣ�Ӧ�ó���ʹ��TCHAR�������ݡ���������charת��Ϊtchar
*/
void  CComSocket::char2Tchar(LPCTSTR dst, LPSTR src, int maxlen) //charת��ΪTchar
{
	MultiByteToWideChar(CP_ACP, 0, src, -1, (LPWSTR)dst, maxlen);
}

/**
* @author ACM2012
* @note   ����������Ӧ�ó���ʹ��TCHAR�������ݣ����ݴ���ʹ��char���ݡ���������tcharת��Ϊchar
*/

bool   CComSocket::PrintLog(LPCTSTR ErrorCause, bool returnvalue)
{
	AfxMessageBox(ErrorCause);
	return returnvalue;
}

HANDLE CComSocket::PackNode(unsigned int function)
{
	HANDLE NewNode = CreateFileMapping(HANDLE(0xFFFFFFFF), NULL, PAGE_READWRITE, 0, sizeof(Node), NULL);//����һ���սڵ�
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