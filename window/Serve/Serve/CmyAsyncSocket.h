#pragma once
#include "ComSocket.h"

/**
*@class  <CmySocket>
*@brief  ��ʵ���첽�׽���
*@author ACM2012
*@note �����Э��ջ�е��׽��ֳغ���ʹ��,��ɶ�MFC�ṩ��CAsyncSocket�����д��ʵ���׽��ּ����������շ�������ת���ȹ���
*/

class  CmyAsyncSocket : public CComSocket
{
public:
	CmyAsyncSocket();
	~CmyAsyncSocket();
private:
	HANDLE    ReadQueue, WriteQueue;
	HANDLE    SH, CH;
	PN   pCur;
	bool state = true;
	Manager   *pReadQueue, *pWriteQueue;
private:
	bool    flag;          //�����Ƿ�ɹ���־
	int      LastError;   //�������
	enum  {READBEGIN,READMID,READFIN} readstate;
	char  *pReadData;
	unsigned int ReadDataLen, DataLen;
private:
	bool  InitalWriteQueue(regstruct &);
	bool  InitalReadQueue(regstruct &);
	void   RemoveRead();
	bool   AddToTail(HANDLE NewNode);
	bool  WaitForSockEvent(unsigned int SOCKEVENT);
	int     RcvBegin(void* lpBuf, int nBufLen);
	static DWORD WINAPI NewGetSockEventThread(LPVOID);
	void   GetSockEvent();
public:
	/** @brief �׽��ּ��� */
	bool Listen();
	/** @brief  ����UDP���Ķε�ָ��IP*/
	int   SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress);
	/** @brief  ����UDP���Ķβ����淢�ͷ��˿ں�IP */
	int   ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags = 0);
	/** @brief �׽��ְ󶨵�ĳһ���ض��Ķ˿� */
	bool Bind(UINT nSocketPort);
	/** @brief �����׽��� */
	bool Create();
	/** @brief �����׽���ͬʱ�󶨶˿� */
	bool Create(UINT nHostPort);
	/** @brief TCP���ܱ��Ķ� */
	int    Receive(void* lpBuf, int nBufLen);
	/** @brief  TCP�����������󲢽�����ת�����½��׽���*/
	bool  Accept(CmyAsyncSocket& rConnectedSocket);
	/** @brief  �ر��׽��� */
	void  Close();
	/** @brief TCP����Ӧ�ò����� */
	int    Send(const void* lpBuf, int nBufLen);
	/** @brief TCP��������*/
	bool  Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
	/** @brief ֪ͨ�׽��ֿɽ�������*/
	virtual void  OnReceive(int nErrorCode);
	/** @brief ֪ͨ�׽�����������*/
	virtual void  OnAccept(int nErrorCode);
	/** @brief  ֪ͨ�׽��ֶԷ��ѹر�*/
	virtual void  OnClose(int nErrorCode);
	/** @brief ֪ͨ�׽��ֿ��Է�������*/
	virtual void  OnSend(int nErrorCode);
	/** @brief  ֪ͨ�׽������ӳɹ� */
	virtual void  OnConnect(int nErrorCode);
	/** @brief ��������һ������*/
	int    GetLastError();
};

