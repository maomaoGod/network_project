/**@file
*@brief
*@author ACM2012
*@date 2015/4/18
*@version <0.1>
*@note
*声明MyIP类的头文件
*/
typedef unsigned char Byte;
typedef unsigned short Ushort;
typedef int Bool;
#define MAXSIZE 512

/**
*@class <_iphdr>
*@brief 存放ip首部的结构体
*@author ACM2012
*@note
*自己定义的IP报文首部
*/
typedef struct _iphdr //定义IP首部 
{
	unsigned short ih_version;  ///< 标识了数据包的IP版本号,一共4位, 0100表示IPv4, 0110表示IPv6
	unsigned short ih_len;	    ///< 16位数据报长度
	unsigned short ih_ident;	///< 数据报的16位标识
	unsigned short ih_flags;    ///< 数据报的标志
	unsigned short ih_offset;	///< 数据报的片偏移
	unsigned short ih_TTL;		///< 数据报的寿命
	unsigned short ih_protl;	///< 数据报的协议
	unsigned short ih_sum;		///< 16位首部检验和
	unsigned int ih_saddr;		///< 32位源IP
	unsigned int ih_daddr;		///< 32位目的IP
} IP_HEADER;

typedef struct Msg{
		Byte *data;			///<数据
		int data_len;
		Byte *head;			///<数据头
		int head_len;
};

typedef struct IP_msg{
	IP_HEADER *iphdr;    ///< 指向ip首部的指针
	Msg msg;             ///< 封装的TCP/UDP协议的头部及数据
};


/**
*@class  <CMyIP>
*@brief  基于IP协议与客户端实现可靠传输与不可靠传输的类
*@author ACM2012
*@note
*基于IP协议实现选路与转发, 选路算法包括链路状态选路算法和距离向量选路算法, 通过选路算法构造转发表
*/
class CMyIP
{
public:
	Byte IP;		  ///< 客户端地址
	IP_msg *buffer;   ///< 定义IP报文
	Msg msg;          ///< 定义封装的TCP/UDP协议的头部及数据
public:
	/** @brief CMyIP类的构造函数 */
	CMyIP();
	/** @brief CMyIP类的析构函数 */
	~CMyIP();
	/** @brief 初始化CMyIP类的函数 */
	void Init();

	/** @brief 从链路层接收数据的函数 */
	Bool RecvMsg();
	
	/** @brief 往链路层发送数据的函数 */
	BOOL SendMsg();

	/** @brief 从运输层接收数据及IP地址的函数 */
	Bool nettran(Msg msg, Byte IP);

	/** @brief 发送数据到运输层的函数 */
	Bool netrevc(Msg &msg);

	/** @brief 计算待发送数据的检验和 */
	unsigned short GetChecksum(IP_msg *buffer);

	/** @brief 根据菜单提供的选项选择选路算法 */
	void select_algorithm(int mode);

	/** @brief n为路由器个数，v为源节点，*dist为随着算法进行本次迭代，从源节点到目的v的最低费用路径的费用,
	 *	*prev为从源节点v沿着当前最低费用路径的前一节点,
	 *  c[maxnum][maxnum]为从节点x到节点y的链路费用矩阵,
	 *  如果x和y不是直连的，则 c(x,y)=maxint
	 *  链路状态选路算法的实现.
	 */
	void Dijkstra(int n, int v, int *dist, int *prev, int c[MAXSIZE][MAXSIZE]);

	/** @brief  n为节点数，dis[x][y]表示节点x到y的距离， *D[x]表示节点x的距离向量， 
	  * V[x][y]=1表示节点x到y有一条边， cost[x][y]表示节点x到y的距离
	  * 距离向量算法的实现
	  */
	void Bellman_Ford(int n, int *D[x], int dis[n][n], int V[n][n], int cost[n][n]);

	/** @brief 根据所选的选路算法，找出从原地址v到目的地址u的最低费用路径，即根据首部值u，确定一个到v的转发表 */
	void searchPath(int *prev, int v, int u);
};

