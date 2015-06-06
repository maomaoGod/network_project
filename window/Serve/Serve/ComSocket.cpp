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
		MFile = OpenFileMapping(FILE_MAP_WRITE, FALSE, _T("NetProtocolListen"));//打开连接文件
		if (NULL == MFile){
			AfxMessageBox(_T("打开连接文件失败"));
			return;
		}
		preg = (regstruct*)MapViewOfFile(MFile, FILE_MAP_WRITE, 0, 0, sizeof(regstruct));//映射连接文件到本地
		Csemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolCsemaphore"));//打开应用程序信号量
		Psemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolPsemaphore"));//打开协议服务程序信号量
		Ssemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, _T("NetProtocolCreateSuccess"));
		if (!(preg||Csemaphore||Psemaphore)){
			AfxMessageBox(_T("打开协议服务程序信号量失败"));
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
	for (int index = 6; index >=0 ; index--)//将套接字标志加入共享内存名
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

void  CComSocket::Tchar2char(LPSTR dst, LPCTSTR src)//Tchar 转换为char 
{
	int iLength = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, src, -1, dst, iLength, NULL, NULL);
}

/**
* @author ACM2012
* @note   辅助函数，数据传输使用char数据，应用程序使用TCHAR编码数据。本函数将char转换为tchar
*/
void  CComSocket::char2Tchar(LPCTSTR dst, LPSTR src, int maxlen) //char转换为Tchar
{
	MultiByteToWideChar(CP_ACP, 0, src, -1, (LPWSTR)dst, maxlen);
}

/**
* @author ACM2012
* @note   辅助函数，应用程序使用TCHAR编码数据，数据传输使用char数据。本函数将tchar转换为char
*/

bool   CComSocket::PrintLog(LPCTSTR ErrorCause, bool returnvalue)
{
	AfxMessageBox(ErrorCause);
	return returnvalue;
}