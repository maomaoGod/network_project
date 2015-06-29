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


#define SOCK_WRONG              1000
#define SOCK_NOT_UDP            1001
#define SOCK_NOT_TCP             1002
#define SOCK_NOT_LISTEN        1003
#define SOCK_IS_CLOSED         1004
#define SOCK_LS_FAILED           1005

/**
* @class <Manager>
* @brief   ��д�������ṹ��ʵ�ֲ�ͬ���̶�ͬһ��д���еĹ���
* @author ACM2012
* @note   ��д�������ṹ������ṹΪ���̹���ṹ,ʵ�ֶ���̶�ͬһ����Ĺ���
* @param ���������id, writerд�������id,hid ��ǰ����ͷ��� ,cid ��ǰ��������� ��
* Head����ͷ�ڵ�����Cur��ǰ���ڵ�����Tail����β�ڵ���
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
* @class <regstruct>
* @brief   �׽���ע��ṹ�壬�׽���ע��ʦ����������ṹ��ʵ�ֽ��̼�ͨ��
* @author ACM2012
* @note   �������׽���ʱ��Ҫ�������׽��ֳ��н���ע�ᡣ�׽��ֳ�ά�����׽���ע��ṹ��
*�������׽���ʱ���׽������ע��ṹ���׽��ֳؽ��յ������ɻ�ȡ���ṹ��Ϣ��������Ӧ��Դ
*���ȴ���һ���׽���ע�ᡣ
* @param �ṹ������Ҫ����Դip,Դ�˿ڣ�Ŀ��IP��Ŀ�Ķ˿ڣ��Լ���Ӧ�Ŀ�����Ϣ
*/
struct regstruct{
	int    SockMark;            ///<�׽���Ψһ��ʶ
	TCHAR ReadQueueName[20];///<���������ṹ��
	TCHAR WriteQueueName[20];///<д�������ṹ��
};

/**
* @class <CComSocket>
* @brief   �׽��ֵĻ��࣬��Ҫ����׽��ֵĸ�������
* @author ACM2012
* @note   CComSocketΪͬ���׽��ֺ��첽�׽��ֻ��࣬��Ҫ�����CSocket��CAsyncSocket��
*�ĸ��������������׽���ע����׽��ִ����������Ӧ�ò㴫������ݴ���ɱ�׼��д���нڵ�
*/

class CComSocket
{
public:
	  CComSocket();
	~CComSocket();
private:
     static bool Isfirst; 
	 static int  SockNum;
protected:
	 /** @brief  �׽��ֵ�ǰ״̬*/
     enum  { INIT_FLAG, LISTEN_FLAG, TCP_FLAG, UDP_FLAG, CLOSE_FLAG, DIE_FLAG } sockstate;
	 char srcip[20], dstip[20], csrcip[20], cdstip[20];
	 unsigned short srcport,dstport,csrcport,cdstport;
	 unsigned int SockMark;
	 regstruct myreg;
public:
	 static HANDLE  MFile,Wsemaphore, Rsemaphore, Dsemaphore;  //ע���ļ���ͬ���ź���
	 static regstruct *preg;
	 /** @brief �׽���ע�Ტ��ȡΨһ ��ʶ��*/
	 void        GetSockMark(unsigned int &sockmark,regstruct &myreg);
	 /** @brief ���Ӧ�ò㴫���������ݳɱ�׼�ṹ*/
	 HANDLE PackNode(unsigned int function);
	 /** @brief ���Ӧ�ò㴫���������ݳɱ�׼�ṹ */
	 HANDLE PackNode(unsigned int function, UINT nSocketPort);
	 /** @brief ���Ӧ�ò㴫���������ݳɱ�׼�ṹ */
	 HANDLE PackNode(unsigned int function, const void* lpBuf, int nBufLen);
	 /** @brief ���Ӧ�ò㴫���������ݳɱ�׼�ṹ */
	 HANDLE PackNode(unsigned int function, LPCTSTR lpszHostAddress, UINT nHostPort);
	 /** @brief ���Ӧ�ò㴫���������ݳɱ�׼�ṹ */
	 HANDLE PackNode(unsigned int function, CComSocket &rConnectedSocket);
	 /** @brief ���Ӧ�ò㴫���������ݳɱ�׼�ṹ */
	 HANDLE PackNode(unsigned int function, const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress);
	 /** @brief ����쳣��Ϣ���ı�*/
	 bool  PrintLog(LPCTSTR, bool);
	 /** @brief ��������char������תΪtchar */
	 void  char2Tchar(LPCTSTR dst, LPSTR src, int maxlen);
	 /** @brief  ��������tchar������ת��Ϊchar*/
	 void  Tchar2char(LPSTR dst, LPCTSTR src);
};

