/**@file
*@brief
*@author ACM2012
*@date 2015.4.18
*@version <0.1>
*@note
*定义CRouting类的文件
*/
#include "stdafx.h"
#include "CRouting.h"


extern int Routing_select;
extern int end_connect;


/**
* @author ACM2012
* @param
* @exception
* @return 无
* @note
* CRouting类的构造函数
* @remarks
*/
CRouting::CRouting()
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
CRouting::~CRouting()
{
}

/**
* @author ACM2012
* @param [in] 初始节点, 目的节点, 之间相连节点的堆栈
* @return 当前节点应该向下传的一个节点
* @note 主要是寻找下一个节点
*/
BOOL CRouting::searchPath(int *pre, int v, int u)
{
	int que[maxnum];
	int tot = 1;
	que[tot] = u;
	tot++;
 	int tmp = pre[u];
	while (tmp != v)
	{
		que[tot] = tmp;
		tot++;
		tmp = pre[tmp];
	}
	que[tot] = v;
	return que[tot - 1];
}

/**
* @author ACM2012
* @param [in] 初始节点, 目的节点, 当前节点到源节点的最短路径长度, 当前节点的前一个节点, 记录图的两节点间路径长度
* @return 无
* @note 全局搜索最优的下一个节点
*/
void CRouting::Dijkstra(int n, int v, int *dist, int *pre, int c[maxnum][maxnum])
{
	bool s[maxnum];    // 判断是否已存入该点到S集合中
	for (int i = 1; i <= n; ++i)
	{
		dist[i] = c[v][i];
		s[i] = 0;     // 初始都未用过该点
		if (dist[i] == maxint)
			pre[i] = 0;
		else
			pre[i] = v;
	}
	dist[v] = 0;
	s[v] = 1;

	// 依次将未放入S集合的结点中，取dist[]最小值的结点，放入结合S中
	// 一旦S包含了所有V中顶点，dist就记录了从源点到所有其他顶点之间的最短路径长度
	for (int i = 2; i <= n; ++i)
	{
		int tmp = maxint;
		int u = v;
		// 找出当前未使用的点j的dist[j]最小值
		for (int j = 1; j <= n; ++j)
		if ((!s[j]) && dist[j]<tmp)
		{
			u = j;              // u保存当前邻接点中距离最小的点的号码
			tmp = dist[j];
		}
		s[u] = 1;    // 表示u点已存入S集合中

		// 更新dist
		for (int j = 1; j <= n; ++j)
		if ((!s[j]) && c[u][j]<maxint)
		{
			int newdist = dist[u] + c[u][j];
			if (newdist < dist[j])
			{
				dist[j] = newdist;
				pre[j] = u;
			}
		}
	}
}

BOOL CRouting::LS(LS_data *LSdata)
{
	Dijkstra(LSdata->LsData.node, LSdata->LsData.sid, LSdata->LsData.dist, LSdata->LsData.pre, LSdata->c);
	return searchPath(LSdata->LsData.pre, LSdata->LsData.sid, LSdata->LsData.did);
}

//n为节点数，edge为边数，v为初始节点，dist为初始节点到各个节点的距离, pre为前节点序列
void CRouting::Bellman_Ford(int n, int edgenum, int v, int *dist, int *pre, Edge edge[N])
{
	pre[v] = v;
	for (int i = 1; i <= n; ++i) //初始化
		dist[i] = (i == v ? 0 : MAX);

	for (int j = 0; j < edgenum; ++j)
		if (dist[edge[j].v] > dist[edge[j].u] + 1) //松弛（顺序一定不能反~）
		{
			dist[edge[j].v] = dist[edge[j].u] + 1;
			pre[edge[j].v] = edge[j].u;
		}
}

BOOL CRouting::DV(DV_data *DVdata)
{
	Bellman_Ford(DVdata->DvData.node, DVdata->edgenum, DVdata->DvData.sid, DVdata->DvData.dist, DVdata->DvData.pre, DVdata->E);
	return searchPath(DVdata->DvData.pre, DVdata->DvData.sid, DVdata->DvData.did);
}
