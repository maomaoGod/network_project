/**@file
*@brief
*@author ACM2012
*@date 2015.4.18
*@version <0.1>
*@note
*定义CMyIP类的文件
*/
#include "stdafx.h"
#include "CMyIP.h"
/**
* @author ACM2012
* @param
* @exception
* @return 无
* @note
* CMyIP类的构造函数 
* @remarks
*/
CMyIP::CMyIP()
{
}
/**
* @author ACM2012
* @param
* @return 无
* @note
* CMyIP类的析构函数
* @remarks
*/
CMyIP::~CMyIP()
{
}
/**
* @author ACM2012
* @return 无
* @note
* 初始化函数, 选择默认地址
*/
void CMyIP::Init()
{
	///< 初始化客户端地址, 默认本地地址
	///< 初始化IP报文
	///< 初始化封装的TCP/UDP协议的头部及数据
}

/**
* @author ACM2012
* @param [in] 无
* @return 返回FALSE说明没有接收到链路层的消息，返回TRUE说明接收到链路层发来的消息
* @note
* 函数功能主要是链路层发送过来的信息分解成IP_msg结构, 并且剥离出Msg结构, 准备发送给运输层
*/
BOOL CMyIP::RecvMsg()
{
	///< 根据链路层发送的数据进行剥离得到报文长度以及偏移, 比较偏移量是否等于报文长度
	///< 若发现分片缺失或者检验和出错则 return FALSE;
	///< 若是则数据成功接收 进行少量的检验和检查, 若没有错误
	///< 则将IP_msg结构剥离出Msg结构

}
/**
* @author ACM2012
* @param [in] 无
* @return 返回FALSE说明发送失败，返回TRUE说明发送成功
* @note 函数功能主要是将Msg结构和IP地址等信息分片组装成IP_msg发送给链路层
*/
BOOL CMyIP::SendMsg()
{
	///< 将运输层送来的Msg结构和IP地址插入到IP_msg结构中,
	///< 如果信息超过容量就进行分片处理, 
	///< 调用链路层的发送函数如果发送失败 return FALSE;
	///< 否则 return TRUE;
}

/**
* @author ACM2012
* @param [in] <buffer> unsigned short *类型，指向要计算检验和的数据的第一个字节
* @return 返回计算完成的检验和
* @note
*/
unsigned short CMyIP::GetChecksum(IP_msg *buffer)
{
	unsigned long sum = 0; ///< 定义一个无符号长整型  
	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}
	if (size)
	{
		cksum += *(UCHAR*)buffer;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (unsigned short)(~cksum);
}

/** @brief 根据菜单提供的选项选择选路算法 */
void CMyIP::select_algorithm(int mode)
{
	if (mode == SELECT_D) ///< 在菜单上选取链路状态算法
		Dijkstra(n, 1, dist, prev, c);
	else    ///< 在菜单上选取距离向量算法
		Bellman_Ford(n, 1, dist, prev, c);
}

/** @brief n为路由器个数，v为源节点，*dist为随着算法进行本次迭代，从源节点到目的v的最低费用路径的费用,
*	*prev为从源节点v沿着当前最低费用路径的前一节点,
*   c[MAXSIZE][MAXSIZE]为从节点x到节点y的链路费用矩阵,
*   如果x和y不是直连的，则 c(x,y)=maxint
*   链路状态选路算法的实现.
*/
void CMyIP::Dijkstra(int n, int v, int *dist, int *prev, int c[MAXSIZE][MAXSIZE])
{

	bool s[maxnum];///< s集合用于判断i节点是否已按最低费用到达，是则赋值为1，否则赋值为
	///< 首先初始化
	for (int i = 1; i <= n; ++i)
	{
		dist[i] = c[v][i];///< 对源节点到各个节点的费用赋初值

		s[i] = 0;  ///< 即初始化时所有节点都赋值为没有按最低费用到达
		///< 判断源节点跟其他节点是否为直连的，不是则该节点的前节点为0，是则为源节点
		if (dist[i] == maxint)
			prev[i] = 0;
		else
			prev[i] = v;
	}
	dist[v] = 0;///< 完成源节点的费用和所能到达节点的赋值
	s[v] = 1;///<  源节点已按最低费用到达
	///<  依次将未放入S集合的结点中，取dist[]最小值的结点，放入结合S中，一旦S包含了所有V中顶点，dist就记录了从源点到所有其他顶点之间的最短路径长度
	for (int i = 2; i <= n; ++i)
	{
		int tmp = maxint;
		int u = v;
		///<  找出当前未使用的点j的dist[j]最小值
		for (int j = 1; j <= n; ++j)
		if ((!s[j]) && dist[j]<tmp)
		{
			///<  u保存当前邻接点中费用最小的点的号码
			u = j;
			tmp = dist[j];
		}
		s[u] = 1;///<  表示u点已存入S集合中
		///< 根据j更新dist
		for (int j = 1; j <= n; ++j)
		if ((!s[j]) && c[u][j]<maxint)
		{
			int newdist = dist[u] + c[u][j];
			///<  如果新的费用比原来小，采用新的费用
			if (newdist < dist[j])
			{
				dist[j] = newdist;
				prev[j] = u;///<  把u的前节点赋为j
			}
		}
	}
}

/** @brief  n为节点数，dis[x][y]表示节点x到y的距离， *D[x]表示节点x的距离向量，
  * V[x][y]=1表示节点x到y有一条边， cost[x][y]表示节点x到y的距离
  * 距离向量算法的实现
  */
void CMyIP::Bellman_Ford(int n, int *D[x], int dis[n][n], int V[n][n], int cost[n][n])
{
	int dis[n][n]；///<  dis[x][y]表示节点x到y的距离
		int D[n][n];      ///<  *D[x]表示节点x的距离向量
	int V[n][n];  ///<  V[x][y]=1表示节点x到y有一条边
	int cost[n][n]; ///<  cost[x][y]表示节点x到y的距离
	define MAX = 999999;
	void send(D[x], y); ///< send（*D[x]，y）表示向节点y发送x的节点向量
	int wait();   ///< wait()用于监控那个节点接收到了新的距离向量

	///< 初始化
	for (int x = 0; x<n; x++)
	{
		for (int y = 0; y<n; n++)
		{
			if (V[x][y] == 1) ///<  如果y为x的邻居
				dis[x][y] = c[x][y];  ///<  dis[x][y]=cost[x][y]
			else
				dis[x][y] = MAX;  ///< 如果y不为x的邻居，dis[x][y]为无穷大
			D[x][y] = dis[x][y]; ///<  初始化距离向量为D[x][y]=dis[x][y]
		}
		for (int y = 0; y<n; n++)
			send(*D[x], y);    ///< 将x的距离向量发送到每个邻居节点
	}

	///< 监控到节点x接收到了新的距离向量
	while（(x = wait())！ = 0）
	{
		int change = 0;
		int dis1[n][n];
		for (int y = 0; y<n; n++)
		{
			dis1[x][y] = min(cost[x][y] + D[x][y]);
			if (dis[x][y] != dis[x][y])
				change = 1;
			dis[x][y] = min(cost[x][y] + D[x][y]); D[x][y] = dis[x][y]; ///< 更新dis[x][y]，D[x][y]

		}
		if (change)
		for (int y = 0; y<n; n++)
			send(*D[x], y);
	}
}

/** @brief 根据所选的选路算法，找出从原地址v到目的地址u的最低费用路径，即根据首部值u，确定一个到v的转发表 */
void CMyIP::searchPath(int *prev, int v, int u)
{
	int que[maxnum];///< 转发表
	int tot = 1;    ///< 用于回溯并记录已经回溯的节点数
	que[tot] = u;   ///< 目的地址u
	tot++;          ///< 已从目的地址向前找到回溯
	int tmp = prev[u];
	///< 从目的地址不断向前回溯，直到找到原地址为止，每次均前后交换，以得出按从原地址到目的地址的转发表
	while (tmp != v)
	{
		que[tot] = tmp;
		tot++;
		tmp = prev[tmp];
	}
	que[tot] = v;
	///< 输出从原地址到目的地址的转发表
	for (int i = tot; i >= 1; --i)
	if (i != 1)
		cout << que[i] << " -> ";
	else
		cout << que[i] << endl;
}