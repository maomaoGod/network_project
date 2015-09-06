#include "CRoute.h"

extern "C" __declspec(dllexport) CRoute* CreateRoutePtr();

extern "C" __declspec(dllexport) void DeleteRoutePtr(CRoute* t);



/**
*@class  <CNetProtocolApp>
*@brief  协议栈应用程序类
*@author ACM2012
*@note 本程序为MFC向导生成类，主要完成应用程序的初始化工作
*/

class Route : public CRoute
{
public:
	NAT_translation_table *p = NULL;
	unsigned int iIP,oIP;
	struct Route_info *info;
	struct LS_data *LsData;
	struct DV_data *DvData;
	int LD;
	int IPN;

public:
	/** @brief 路由类的构造函数 */
	Route();
	/** @brief 路由类的析构函数 */
	~Route();
	/** @brief 选择选路协议函数 */
	void RS(int Routing_Select);
	/** @brief 选择发送路由信息还是数据信息函数 */
	int Datainfo(WPARAM wparam, LPARAM lparam);
	/** @brief 获取本地IP的函数 */
	unsigned int getLocalIP();
	/** @brief 获取外网IP的函数 */
	unsigned int getPublicIP();
	/** @brief 输入源IP,源端口,目的IP,目的端口的地址,用NAT协议修改 */
	In_NAT* NAT(In_NAT* Nat, int &flag);
	/** @brief 计算IP数据报的首部检验和 */
	unsigned short iphdrmakesum(Routeip_message *ip);
	/** @brief 从网络层传送数据到链路层的函数 */
	int IP2Link(WPARAM wparam, LPARAM lparam);
	

public:
	/** @brief 从链路层得到用于选路的信息 */
	int  RecvMsg(WPARAM wparam, LPARAM lparam);
	/** @brief 用于发送用于选路的信息 */
	int  SendMsg(WPARAM wparam, LPARAM lparam);
	/** @brief 从数组的下标得到对应的IP */
	unsigned int Num2IP(int num);
	/** @brief 从IP得到对应的数组下标 */
	int  IP2Num(unsigned int IP);

	/** @brief 从路由信息结构中提取到LS_data信息结构 */
	void Route2LS(Route_info *info, LS_data *LsData);
	/** @brief 从路由信息结构中提取到DV_data信息结构 */
	void Route2DV(Route_info *info, DV_data *DvData);
	/** @brief 堆栈选择下一条路,返回下一个IP地址 */
	int  searchPath(int *pre, int v, int u);
	/** @brief n为节点数，v为初始节点，dist为初始节点到各个节点的距离, pre为前节点序列 */
	void Dijkstra(int n, int v, int *dist, int *pre, int c[maxnum][maxnum]);
	/** @brief 求解全局路径的最短路径 */
	int  LS(LS_data *LSdata);
	/** @brief n为节点数，edgenum为边数，v为初始节点，dist为初始节点到各个节点的距离, pre为前节点序列 */
	void Bellman_Ford(int n, int edgenum, int v, int *dist, int *pre, Edge edge[N]);
	/** @brief 求解局部路径的最短路径 */
	int  DV(DV_data *DVdata);
};