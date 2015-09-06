#pragma once

#include <map>
using namespace std;
/**
*@class  <SockPool>
*@brief  �׽��ֹ�����
*@author ACM2012
*@note
*�׽��ֹ���ģ��,���������׽���ע��������,���ṩЭ��ջ���׽��ֵ����ӽӿڣ�
*ʵ��������Ӧ�ó����Э��ջ����Ľ���ͨ��
*/

#define  TRANSTOIP             WM_USER+1000
#define  IPTOLINK                 WM_USER+1001
#define  LINKSEND               WM_USER+1002
#define  TRANSTOAPP         WM_USER+1003
#define  APPTOTRANS	       WM_USER+1004
#define  IPTOTRANS             WM_USER+1005
#define  LINKTOIP                 WM_USER+1006
#define  APPSEND                WM_USER+1007
#define SOCKSTATEUPDATE WM_USER+1008

#define  SOCKCONNECT             200
#define  SOCKBIND                     201
#define  SOCKLISTEN                 202
#define  SOCKSEND                   203
#define  SOCKSENDTO               204
#define  SOCKRECEIVE              205
#define  SOCKRECEIVEFROM    206
#define  SOCKCLOSE                 207
#define  SOCKACCEPT               208
#define  SOCKDESTORY             209


/**
* @class <Manager>
* @brief   ��д�������ṹ��ʵ�ֲ�ͬ���̶�ͬһ��д���еĹ���
* @author ACM2012
* @note   ��д�������ṹ������ṹΪ���̹���ṹ,ʵ�ֶ���̶�ͬһ����Ĺ���
* @param reader ���������id, writerд�������id,hid ��ǰ����ͷ��� ,cid ��ǰ��������� ��Head����ͷ�ڵ�����Cur��ǰ���ڵ�����Tail����β�ڵ���
*/
typedef struct Manager
{
	int reader;  ///<���������id
	int writer;   ///<д�������id
	unsigned int hid; ///<��ǰ����ͷ���
	unsigned int cid;///<��ǰ���������
	HANDLE Head;///<����ͷ�ڵ���
	HANDLE Cur;///<��ǰ���ڵ���
	HANDLE Tail;///<����β�ڵ���
} *PM;

/**
* @class <transstruct>
* @brief   �׽��ֺʹ�������ݴ���ṹ
* @author ACM2012
* @note   �׽��ֺʹ�������ݴ���ṹ����Ҫ��Ӧ�ò����ݴ���������
* @param �ṹ������Ҫ����Դip,Դ�˿ڣ�Ŀ��IP��Ŀ�Ķ˿ڣ��Լ���Ӧ�Ŀ�����Ϣ
*/

struct transstruct {
	unsigned short  dstport;   //Ŀ�Ķ˿ں�
	unsigned short  srcport;   //Դ�˿ں�
	unsigned short  function;     //�󶨶˿ں�
	int    datalength;             //���ݳ���
	char srcip[20];                //ԭ��ַip
	char dstip[20];                //Ŀ���ַip
	char *data;                     //����
};

/**
* @class <Node>
* @brief   ����ͨ�Žڵ�ṹ�����̼�ͨ�ž�ʹ�ñ�׼��Node�ṹ
* @author ACM2012
* @note   ��д�������ṹ������ṹΪ���̹���ṹ,ʵ�ֶ���̶�ͬһ����Ĺ���
* @param �ṹ������Ҫ����Դip,Դ�˿ڣ�Ŀ��IP��Ŀ�Ķ˿ڣ��Լ���Ӧ�Ŀ�����Ϣ
*/
typedef struct Node
{
	unsigned int FuncID;              ///< �׽�����Ϣ����
	unsigned int AcceptSockMark;///<ָ���׽��ֱ�ʶ
	unsigned short  srcport;    ///<Դ�˿ں�
	unsigned short  dstport;    ///<Ŀ�Ķ˿ں�
	char srcip[20];                 ///<Դip
	char dstip[20];                 ///<Ŀ��ip
	unsigned short  bindport;  ///<�󶨶˿ں�
	unsigned int  DataLen;     ///<���ݳ���
	HANDLE Data;                ///<���ݾ��
	HANDLE Next;                ///<��һ���ڵ���
}*PN;

/**
*@class <regstruct>
* @brief   �׽���ע��ṹ�壬�׽���ע��ʦ����������ṹ��ʵ�ֽ��̼�ͨ��
* @author ACM2012
* @note   �������׽���ʱ��Ҫ�������׽��ֳ��н���ע�ᡣ�׽��ֳ�ά�����׽���ע��ṹ��
*�������׽���ʱ���׽������ע��ṹ���׽��ֳؽ��յ������ɻ�ȡ���ṹ��Ϣ��������Ӧ��Դ
*���ȴ���һ���׽���ע�ᡣ
* @param �ṹ������Ҫ����Դip, Դ�˿ڣ�Ŀ��IP��Ŀ�Ķ˿ڣ��Լ���Ӧ�Ŀ�����Ϣ
*/

struct regstruct{
	int    SockMark;            ///<�׽���Ψһ��ʶ
	TCHAR ReadQueueName[40];///<���������ṹ��
	TCHAR WriteQueueName[40];///<д�������ṹ��
	TCHAR CDestoryName[40];
	TCHAR SDestoryName[40];
};

/**
* @class <Para>
* @brief   ��д���д���Ĳ����ṹ��
* @author ACM2012
* @note   �׽��ֺʹ�������ݴ���ṹ����Ҫ��Ӧ�ò����ݴ���������
* @param �ṹ������Ҫ����Դ�׽��ֱ�ʶ������ָ�룬��д���ƿ�Ͷ�д����ָ��
*/
struct Para
{
	void *pthis;
	unsigned int SockMark;
	HANDLE CH;
	HANDLE Queue;
	HANDLE Destory;
	PM pQueue;
};

/**
* @class <portin>
* @brief   �Զ���˿�ӳ��ṹ�壬ʵ���׽��ֶ˿ڵ�ӳ��
* @author ACM2012
* @note   �Զ���˿�ӳ��ṹ�壬ʵ���׽��ֶ˿ڵ�ӳ�䣬�ڳ�����Ŀ�Ķ˿ڲ����Ǵ�����Ϣ����ʵ�˿ڣ�
* ������Ҫ��ĳ��ӳ��
* @param �ṹ������Ҫ����Դip,Դ�˿ڣ�Ŀ�Ķ˿�
*/

struct portin    //�õ�Ŀ�Ķ˿ڵ����ݽṹ
{
	char srcip[20];
	unsigned short srcport;
	unsigned short dstport;
	bool operator <(const portin & other) const
	{
		if (strcmp(srcip, other.srcip) < 0)
			return true;
		else if (strcmp(srcip, other.srcip) >0)
			return false;
		else {
			if (srcport < other.srcport)
				return true;
			else if (srcport>other.srcport)
				return false;
			else {
				if (dstport < other.dstport)
					return true;
				else if (dstport>other.dstport)
					return false;
				return false;
			}
		}
	}
};

class SockPool
{
public:
	SockPool();
	bool Create(CString user);
	~SockPool();
public:
	void   SendToApp(void *);
	int     sockconnum;
private:
	int nPort = 0;
	map <portin, unsigned int> PortIn2ScokMark;
	map <unsigned short, unsigned short> Port2PortOut;
	map <unsigned int, transstruct *> SockMark2SockStruct;
	map <unsigned int, unsigned short>  SockMark2Port;
	map <unsigned short, unsigned int>  Port2SockMark;
	map <unsigned int, CEvent *>SockMark2WEvent;
	map <unsigned int, CEvent *>SockMark2REvent;
	map <unsigned int, bool> SockMark2ReadState;
	map <unsigned int, bool> SockMark2WriteState;
	map <unsigned int, bool> SockMark2State;
	map  <unsigned int, HANDLE> SockMark2ReadThread;
	map <unsigned int, HANDLE> SockMark2WriteThread;
private:
	/** @brief �׽���ע��ͬ���ź���*/
	HANDLE Wsemaphore, Rsemaphore, Dsemaphore;
	/** @brief �׽����ļ����*/
	HANDLE MFile;
	HANDLE SH;
	regstruct *preg;
private:
	
	/** @brief �׽���ע���߳�*/
	static DWORD WINAPI NewConnThread(LPVOID lParam);
	/** @brief �׽��ֶ������߳� */
	static DWORD WINAPI NewReadThread(LPVOID lParam);
	/** @brief �׽���д�����߳� */
	static DWORD WINAPI NewWriteThread(LPVOID lParam);
	/** @brief  ��ʼ��������*/
	bool   InitalReadQueue(HANDLE, PM &, HANDLE &);
	/** @brief ��ʼ��д���� */
	bool   InitalWriteQueue(HANDLE, PM &);
	/** @brief �쳣�������*/
	bool   PrintLog(LPCTSTR, bool);
	/** @brief ���д�������ѱ���ȡ�ڵ� */
	void   ClearNode(PM &pWriteQueue);
	/** @brief ��ӽڵ㵽д����β�� */
   bool   AddToTail(PM &, HANDLE);
	/** @brief  �׽���ע�����Ӵ����� */
	void   Connect();
	/** @brief ��ʼ����д�̲߳������� */
	void   InitalThreadPara(Para &, HANDLE, HANDLE, PM, unsigned int,HANDLE &);
	/** @brief  ���������ת��Ϊ���нڵ�ṹ*/
	bool   SockDataToNode(PN, unsigned int);
	/** @brief �׽��ַ�����Դ*/
	void   AllocResource(unsigned int SockMark);
	/** @brief �������̴߳����� */
	void   ReadSock(HANDLE,unsigned int,HANDLE, PM,HANDLE &);
	/** @brief д�����̴߳����� */
	void   WriteSock(HANDLE,unsigned int,HANDLE, PM,HANDLE &);
	/** @brief �׽��ֹرպ���*/
	bool  CloseSock(unsigned int);
	bool  state = true;
};
