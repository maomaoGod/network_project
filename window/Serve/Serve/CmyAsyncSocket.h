#pragma once
#include "ComSocket.h"

/**
*@class  <CmyAsyncSocket>
*@brief  ��ʵ���첽�׽��֣�ʵ�ִ�����Ӧ�ò�֮���ͨ��
*@author ACM2012
*@note �����Э��ջ�е��׽��ֳغ���ʹ��,��ɶ�MFC�ṩ��CmyAsyncSocket�����д��ʵ���׽��ּ�����
* �����շ�������ת���ȹ���
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
	/** @brief ��ʼ��д���л���*/
	bool  InitalWriteQueue(regstruct &);
	/** @brief  ��ʼ�������л���*/
	bool  InitalReadQueue(regstruct &);
	/** @brief  �Ӵ�д�������Ƴ��Ѷ��ڵ�*/
	void   RemoveRead();
	/** @brief �������ݵ�����β��*/
	bool   AddToTail(HANDLE NewNode);
	/** @brief �׽��ֵȴ��׽����¼�����*/
	bool  WaitForSockEvent(unsigned int SOCKEVENT);
	/** @brief ��ʼ���׽����н�������*/
	int     RcvBegin(void* lpBuf, int nBufLen);
	/** @brief �����첽�׽����߳�*/
	static DWORD WINAPI NewGetSockEventThread(LPVOID);
	/** @brief ѭ���ȴ��׽���ʱ�䵽��*/
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

