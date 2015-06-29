#pragma once
#include "ComSocket.h"

/**
*@class  <CmySocket>
*@brief   ��ʵ��ͬ���׽��֣�ʵ�ִ�����Ӧ�ò�֮���ͨ��
*@author ACM2012
*@note   �����Э��ջ�е��׽��ֳغ���ʹ��,��ɶ�MFC�ṩ��CSocket�����д��ʵ���׽��ּ�����
* �����շ�������ת���ȹ���
*/


class  CmySocket: public CComSocket
{
public:
 	  CmySocket();
	~CmySocket();
private:
	HANDLE   ReadQueue,WriteQueue;
	HANDLE   SH, CH;
	PN pCur;
	Manager   *pReadQueue, *pWriteQueue;
private:
	bool    flag;           //�����Ƿ�ɹ���־
	int      LastError;   //�������
	bool   done=true;
	char  *pReadData;
	void  DestroySock();
	unsigned int ReadDataLen,DataLen;
	enum  { READBEGIN, READMID, READFIN } readstate;
private:
	/** @brief ��ʼ��д���л���*/
	bool  InitalWriteQueue(regstruct &);
	/** @brief  ��ʼ�������л���*/
	bool  InitalReadQueue(regstruct &);
	/** @brief  �Ӵ�д�������Ƴ��Ѷ��ڵ�*/
	void   RemoveRead();
	/** @brief ��ʼ���׽����н�������*/
	int    RcvBegin(void* lpBuf, int nBufLen);
	/** @brief �������ݵ�����β��*/
	bool   AddToTail(HANDLE NewNode);
	/** @brief �׽��ֵȴ��׽����¼����� */
	bool  WaitForSockEvent(unsigned int SOCKEVENT);
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
	bool  Accept(CmySocket& rConnectedSocket);
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

