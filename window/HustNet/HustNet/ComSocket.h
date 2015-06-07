#pragma once

#define  SOCKCONNECT             200
#define  SOCKBIND                     201
#define  SOCKLISTEN                 202
#define  SOCKSEND                   203
#define  SOCKSENDTO               204
#define  SOCKRECEIVE              205
#define  SOCKRECEIVEFROM    206
#define  SOCKCLOSE                 207
#define  SOCKACCEPT               208


/**
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

typedef struct Node
{
	unsigned int FuncID;///< �׽�����Ϣ����
	unsigned int SockMark;///< �׽���Ψһ��ʶ
	unsigned int AcceptSockMark;///<ָ���׽��ֱ�ʶ
	unsigned short  srcport;    ///<Դ�˿ں�
	unsigned short  dstport;    ///<Ŀ�Ķ˿ں�
	char srcip[20];                 ///<Դip
	char dstip[20];                 ///<Ŀ��ip
	unsigned short  bindport;  ///<�󶨶˿ں�
	unsigned int  DataLen;     ///<���ݳ���
	HANDLE Data;               ///<���ݾ��
	HANDLE Next;              ///<��һ���ڵ���
}*PN;

struct regstruct{
	int    SockMark;            ///<�׽���Ψһ��ʶ
	TCHAR ReadQueueName[20];///<���������ṹ��
	TCHAR WriteQueueName[20];///<д�������ṹ��
};

typedef struct ConnQueue
{
	char srcip[20];   ///<Դ�˿ں�
	char dstip[20];   ///<Ŀ�Ķ˿ں�
	unsigned short srcport; ///<
	unsigned short dstport; ///<
}*PC;

class CComSocket
{
public:
	  CComSocket();
	~CComSocket();
private:
      bool flag;
public:
	 static int SockMark;
	 static bool Isfirst;
	 static HANDLE  MFile,Csemaphore, Psemaphore, Ssemaphore;  //ע���ļ���ͬ���ź���
	 static regstruct *preg;                                      
	 void  GetSockMark(int &sockmark,regstruct &myreg);
protected:
	bool  PrintLog(LPCTSTR, bool);
	void  char2Tchar(LPCTSTR dst, LPSTR src, int maxlen);
	void  Tchar2char(LPSTR dst, LPCTSTR src);
};

