/**@file
*@brief
*@author ACM2012
*@date 2015/4/18
*@version <0.1>
*@note
*����MyIP���ͷ�ļ�
*/
typedef unsigned char Byte;
typedef unsigned short Ushort;
typedef int Bool;
#define MAXSIZE 512

/**
*@class <_iphdr>
*@brief ���ip�ײ��Ľṹ��
*@author ACM2012
*@note
*�Լ������IP�����ײ�
*/
typedef struct _iphdr //����IP�ײ� 
{
	unsigned short ih_version;  ///< ��ʶ�����ݰ���IP�汾��,һ��4λ, 0100��ʾIPv4, 0110��ʾIPv6
	unsigned short ih_len;	    ///< 16λ���ݱ�����
	unsigned short ih_ident;	///< ���ݱ���16λ��ʶ
	unsigned short ih_flags;    ///< ���ݱ��ı�־
	unsigned short ih_offset;	///< ���ݱ���Ƭƫ��
	unsigned short ih_TTL;		///< ���ݱ�������
	unsigned short ih_protl;	///< ���ݱ���Э��
	unsigned short ih_sum;		///< 16λ�ײ������
	unsigned int ih_saddr;		///< 32λԴIP
	unsigned int ih_daddr;		///< 32λĿ��IP
} IP_HEADER;

typedef struct Msg{
		Byte *data;			///<����
		int data_len;
		Byte *head;			///<����ͷ
		int head_len;
};

typedef struct IP_msg{
	IP_HEADER *iphdr;    ///< ָ��ip�ײ���ָ��
	Msg msg;             ///< ��װ��TCP/UDPЭ���ͷ��������
};


/**
*@class  <CMyIP>
*@brief  ����IPЭ����ͻ���ʵ�ֿɿ������벻�ɿ��������
*@author ACM2012
*@note
*����IPЭ��ʵ��ѡ·��ת��, ѡ·�㷨������·״̬ѡ·�㷨�;�������ѡ·�㷨, ͨ��ѡ·�㷨����ת����
*/
class CMyIP
{
public:
	Byte IP;		  ///< �ͻ��˵�ַ
	IP_msg *buffer;   ///< ����IP����
	Msg msg;          ///< �����װ��TCP/UDPЭ���ͷ��������
public:
	/** @brief CMyIP��Ĺ��캯�� */
	CMyIP();
	/** @brief CMyIP����������� */
	~CMyIP();
	/** @brief ��ʼ��CMyIP��ĺ��� */
	void Init();

	/** @brief ����·��������ݵĺ��� */
	Bool RecvMsg();
	
	/** @brief ����·�㷢�����ݵĺ��� */
	BOOL SendMsg();

	/** @brief �������������ݼ�IP��ַ�ĺ��� */
	Bool nettran(Msg msg, Byte IP);

	/** @brief �������ݵ������ĺ��� */
	Bool netrevc(Msg &msg);

	/** @brief ������������ݵļ���� */
	unsigned short GetChecksum(IP_msg *buffer);

	/** @brief ���ݲ˵��ṩ��ѡ��ѡ��ѡ·�㷨 */
	void select_algorithm(int mode);

	/** @brief nΪ·����������vΪԴ�ڵ㣬*distΪ�����㷨���б��ε�������Դ�ڵ㵽Ŀ��v����ͷ���·���ķ���,
	 *	*prevΪ��Դ�ڵ�v���ŵ�ǰ��ͷ���·����ǰһ�ڵ�,
	 *  c[maxnum][maxnum]Ϊ�ӽڵ�x���ڵ�y����·���þ���,
	 *  ���x��y����ֱ���ģ��� c(x,y)=maxint
	 *  ��·״̬ѡ·�㷨��ʵ��.
	 */
	void Dijkstra(int n, int v, int *dist, int *prev, int c[MAXSIZE][MAXSIZE]);

	/** @brief  nΪ�ڵ�����dis[x][y]��ʾ�ڵ�x��y�ľ��룬 *D[x]��ʾ�ڵ�x�ľ��������� 
	  * V[x][y]=1��ʾ�ڵ�x��y��һ���ߣ� cost[x][y]��ʾ�ڵ�x��y�ľ���
	  * ���������㷨��ʵ��
	  */
	void Bellman_Ford(int n, int *D[x], int dis[n][n], int V[n][n], int cost[n][n]);

	/** @brief ������ѡ��ѡ·�㷨���ҳ���ԭ��ַv��Ŀ�ĵ�ַu����ͷ���·�����������ײ�ֵu��ȷ��һ����v��ת���� */
	void searchPath(int *prev, int v, int u);
};

