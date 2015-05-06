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

	bool Bind(UINT nSocketPort);
};