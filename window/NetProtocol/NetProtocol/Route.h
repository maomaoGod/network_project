#include "CRoute.h"

extern "C" __declspec(dllexport) CRoute* CreateRoutePtr();

extern "C" __declspec(dllexport) void DeleteRoutePtr(CRoute* t);



/**
*@class  <CNetProtocolApp>
*@brief  Э��ջӦ�ó�����
*@author ACM2012
*@note ������ΪMFC�������࣬��Ҫ���Ӧ�ó���ĳ�ʼ������
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
	/** @brief ·����Ĺ��캯�� */
	Route();
	/** @brief ·������������� */
	~Route();
	/** @brief ѡ��ѡ·Э�麯�� */
	void RS(int Routing_Select);
	/** @brief ѡ����·����Ϣ����������Ϣ���� */
	int Datainfo(WPARAM wparam, LPARAM lparam);
	/** @brief ��ȡ����IP�ĺ��� */
	unsigned int getLocalIP();
	/** @brief ��ȡ����IP�ĺ��� */
	unsigned int getPublicIP();
	/** @brief ����ԴIP,Դ�˿�,Ŀ��IP,Ŀ�Ķ˿ڵĵ�ַ,��NATЭ���޸� */
	In_NAT* NAT(In_NAT* Nat, int &flag);
	/** @brief ����IP���ݱ����ײ������ */
	unsigned short iphdrmakesum(Routeip_message *ip);
	/** @brief ������㴫�����ݵ���·��ĺ��� */
	int IP2Link(WPARAM wparam, LPARAM lparam);
	

public:
	/** @brief ����·��õ�����ѡ·����Ϣ */
	int  RecvMsg(WPARAM wparam, LPARAM lparam);
	/** @brief ���ڷ�������ѡ·����Ϣ */
	int  SendMsg(WPARAM wparam, LPARAM lparam);
	/** @brief ��������±�õ���Ӧ��IP */
	unsigned int Num2IP(int num);
	/** @brief ��IP�õ���Ӧ�������±� */
	int  IP2Num(unsigned int IP);

	/** @brief ��·����Ϣ�ṹ����ȡ��LS_data��Ϣ�ṹ */
	void Route2LS(Route_info *info, LS_data *LsData);
	/** @brief ��·����Ϣ�ṹ����ȡ��DV_data��Ϣ�ṹ */
	void Route2DV(Route_info *info, DV_data *DvData);
	/** @brief ��ջѡ����һ��·,������һ��IP��ַ */
	int  searchPath(int *pre, int v, int u);
	/** @brief nΪ�ڵ�����vΪ��ʼ�ڵ㣬distΪ��ʼ�ڵ㵽�����ڵ�ľ���, preΪǰ�ڵ����� */
	void Dijkstra(int n, int v, int *dist, int *pre, int c[maxnum][maxnum]);
	/** @brief ���ȫ��·�������·�� */
	int  LS(LS_data *LSdata);
	/** @brief nΪ�ڵ�����edgenumΪ������vΪ��ʼ�ڵ㣬distΪ��ʼ�ڵ㵽�����ڵ�ľ���, preΪǰ�ڵ����� */
	void Bellman_Ford(int n, int edgenum, int v, int *dist, int *pre, Edge edge[N]);
	/** @brief ���ֲ�·�������·�� */
	int  DV(DV_data *DVdata);
};