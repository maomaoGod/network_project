#pragma once
#include "Serve.h"

class CmyAsyncSocket
{
public:
	ObjEvent    myEvent;
	bool flag;
	int SockMark;
public:
	/** ���캯��
	*/
	CmyAsyncSocket();
	/** ��������
	*/
	~CmyAsyncSocket();
	/** Tcp������
	*/
public:

	bool InitalEvent(regstruct *myreg);
	int Send(const void* lpBuf, int nBufLen);
	/** Udp������
	*/
	int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress);
	/** ��������
	*/
	int Receive(void* lpBuf, int nBufLen);
	/** ����TCP����
	*/
	bool  Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
	/** ������������
	*/
	bool Listen();
	/** ����socket
	*/
	bool Create();

	bool Create(UINT nHostPort);
	/**  ��Ϣ����ʱ����
	*/
	virtual void  OnReceive(int nErrorCode);

	virtual void  OnAccept(int nErrorCode);

	virtual void  OnClose(int nErrorCode);

	void Close();

	void  Accept(CmyAsyncSocket& rConnectedSocket);

	bool Bind(UINT nSocketPort);

	void  char2Tchar(LPSTR src, LPCTSTR dst,int maxlen);

	void  Tchar2char(LPCTSTR src, LPSTR dst);

	void GetSockMark(regstruct *preg, regstruct *myreg);

	static DWORD WINAPI NewThread(LPVOID lParam);

	void ReadSock();
};
