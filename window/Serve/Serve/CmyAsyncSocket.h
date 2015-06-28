#pragma once
#include "ComSocket.h"

/**
*@class  <CmySocket>
*@brief  自实现异步套接字
*@author ACM2012
*@note 本类和协议栈中的套接字池合作使用,完成对MFC提供的CAsyncSocket类的重写，实现套接字监听，数据收发，连接转交等功能
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
	bool    flag;          //创建是否成功标志
	int      LastError;   //错误代号
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
	/** @brief 套接字监听 */
	bool Listen();
	/** @brief  发送UDP报文段到指定IP*/
	int   SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress);
	/** @brief  接受UDP报文段并保存发送方端口和IP */
	int   ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags = 0);
	/** @brief 套接字绑定到某一个特定的端口 */
	bool Bind(UINT nSocketPort);
	/** @brief 创建套接字 */
	bool Create();
	/** @brief 创建套接字同时绑定端口 */
	bool Create(UINT nHostPort);
	/** @brief TCP接受报文段 */
	int    Receive(void* lpBuf, int nBufLen);
	/** @brief  TCP接受连接请求并将连接转交给新建套接字*/
	bool  Accept(CmyAsyncSocket& rConnectedSocket);
	/** @brief  关闭套接字 */
	void  Close();
	/** @brief TCP发送应用层数据 */
	int    Send(const void* lpBuf, int nBufLen);
	/** @brief TCP请求连接*/
	bool  Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
	/** @brief 通知套接字可接收数据*/
	virtual void  OnReceive(int nErrorCode);
	/** @brief 通知套接字有请求到来*/
	virtual void  OnAccept(int nErrorCode);
	/** @brief  通知套接字对方已关闭*/
	virtual void  OnClose(int nErrorCode);
	/** @brief 通知套接字可以发送数据*/
	virtual void  OnSend(int nErrorCode);
	/** @brief  通知套接字连接成功 */
	virtual void  OnConnect(int nErrorCode);
	/** @brief 返回最后的一个错误*/
	int    GetLastError();
};

