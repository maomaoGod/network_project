#pragma once

#include "Serve.h"

class Cmysocket
{

public:
	sockstruct mysock;
public:
	/** ���캯��
	*/
	Cmysocket();
	/** ��������
	*/
	~Cmysocket();
	/** Tcp������
	*/
public:
	int Send(const void* lpBuf, int nBufLen);
	/** Udp������
	*/

	bool Bind(UINT nSocketPort);

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
	/**  ��Ϣ����ʱ����
	*/
	void  OnReceive(int nErrorCode);

	void  char2Tchar(LPSTR src, LPCTSTR dst,int maxlen);

	void  Tchar2char(LPCTSTR src, LPSTR dst);
};
