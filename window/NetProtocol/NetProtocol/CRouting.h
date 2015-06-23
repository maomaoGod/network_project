/**@file
*@brief
*@author ACM2012
*@date 2015/4/18
*@version <0.1>
*@note
*声明CRouting类的头文件
*/
#pragma once
#include "NetProtocol.h"

/**
*@class  <CRouting>
*@brief  实现选路算法, 包括LS全局搜索算法和DV局部搜索算法
*@author ACM2012
*@note
*选路算法包括链路状态选路算法和距离向量选路算法, 通过选路算法得到下一个转发的IP地址
*/
class CRouting
{
public:
	/** @brief CRouting类的构造函数 */
	CRouting();
	/** @brief CRouting类的析构函数 */
	~CRouting();
	/** @brief 堆栈选择下一条路,返回下一个IP地址 */
	BOOL searchPath(int *pre, int v, int u);
	/** @brief 求解全局路径的最短路径 */
	void Dijkstra(int n, int v, int *dist, int *pre, int c[maxnum][maxnum]);
	/** @brief 求解全局路径的最短路径 */
	BOOL LS(LS_data *LSdata);
	/** @brief n为节点数，edge为边数，v为初始节点，dist为初始节点到各个节点的距离, pre为前节点序列 */
	void Bellman_Ford(int n, int edgenum, int v, int *dist, int *pre, Edge edge[N]);
	/** @brief 求解局部路径的最短路径 */
	BOOL DV(DV_data *DVdata);
};

