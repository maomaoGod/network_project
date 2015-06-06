#include "stdafx.h"
#include "ComSocket.h"

int    CComSocket::SockMark = _getpid()*100;
bool  CComSocket::Isfirst = true;
HANDLE CComSocket::Csemaphore = NULL;
HANDLE CComSocket::Ssemaphore = NULL;
HANDLE CComSocket::Psemaphore = NULL;
HANDLE CComSocket::MFile = NULL;
regstruct *CComSocket::preg = NULL;

CComSocket::CComSocket()
{
	if (Isfirst){
		MFile = OpenFileMapping(FILE_MAP_WRITE, FALSE, _T("NetProtocolListen"));//�������ļ�
		if (NULL == MFile){
			AfxMessageBox(_T("�������ļ�ʧ��"));
			return;
		}
		preg = (regstruct*)MapViewOfFile(MFile, FILE_MAP_WRITE, 0, 0, sizeof(regstruct));//ӳ�������ļ�������
		Csemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolCsemaphore"));//��Ӧ�ó����ź���
		Psemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolPsemaphore"));//��Э���������ź���
		Ssemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolCreateSuccess"));
		if (!(preg||Csemaphore||Psemaphore)){
			AfxMessageBox(_T("��Э���������ź���ʧ��"));
			return;
		}
		memcpy(preg->ReadQueueName, _T("ReadQueue"),sizeof(_T("ReadQueue")));
		memcpy(preg->WriteQueueName, _T("WriteQueue"),sizeof(_T("WriteQueue")));
		Isfirst = false;
	}
}

CComSocket::~CComSocket()
{

}

void  CComSocket::GetSockMark(int &sockmark, regstruct &myreg)
{
	TCHAR Buf[7];
	int temp;
	SockMark = SockMark + 1;
	temp = SockMark;
	for (int index = 6; index >=0 ; index--)//���׽��ֱ�־���빲���ڴ���
	{
		Buf[index] = temp % 10 + _T('0');
		temp /= 10;
	}
	memcpy(preg->WriteQueueName+10, Buf, sizeof(Buf));
	memcpy(preg->ReadQueueName+9, Buf, sizeof(Buf));
	preg->SockMark = SockMark;
	sockmark = SockMark;
	memcpy(&myreg, preg, sizeof(regstruct));
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