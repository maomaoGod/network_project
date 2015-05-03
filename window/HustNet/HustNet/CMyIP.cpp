/**@file
*@brief
*@author ACM2012
*@date 2015.4.18
*@version <0.1>
*@note
*����CMyIP����ļ�
*/
#include "stdafx.h"
#include "CMyIP.h"
/**
* @author ACM2012
* @param
* @exception
* @return ��
* @note
* CMyIP��Ĺ��캯�� 
* @remarks
*/
CMyIP::CMyIP()
{
}
/**
* @author ACM2012
* @param
* @return ��
* @note
* CMyIP�����������
* @remarks
*/
CMyIP::~CMyIP()
{
}
/**
* @author ACM2012
* @return ��
* @note
* ��ʼ������, ѡ��Ĭ�ϵ�ַ
*/
void CMyIP::Init()
{
	///< ��ʼ���ͻ��˵�ַ, Ĭ�ϱ��ص�ַ
	///< ��ʼ��IP����
	///< ��ʼ����װ��TCP/UDPЭ���ͷ��������
}

/**
* @author ACM2012
* @param [in] ��
* @return ����FALSE˵��û�н��յ���·�����Ϣ������TRUE˵�����յ���·�㷢������Ϣ
* @note
* ����������Ҫ����·�㷢�͹�������Ϣ�ֽ��IP_msg�ṹ, ���Ұ����Msg�ṹ, ׼�����͸������
*/
BOOL CMyIP::RecvMsg()
{
	///< ������·�㷢�͵����ݽ��а���õ����ĳ����Լ�ƫ��, �Ƚ�ƫ�����Ƿ���ڱ��ĳ���
	///< �����ַ�Ƭȱʧ���߼���ͳ����� return FALSE;
	///< ���������ݳɹ����� ���������ļ���ͼ��, ��û�д���
	///< ��IP_msg�ṹ�����Msg�ṹ

}
/**
* @author ACM2012
* @param [in] ��
* @return ����FALSE˵������ʧ�ܣ�����TRUE˵�����ͳɹ�
* @note ����������Ҫ�ǽ�Msg�ṹ��IP��ַ����Ϣ��Ƭ��װ��IP_msg���͸���·��
*/
BOOL CMyIP::SendMsg()
{
	///< �������������Msg�ṹ��IP��ַ���뵽IP_msg�ṹ��,
	///< �����Ϣ���������ͽ��з�Ƭ����, 
	///< ������·��ķ��ͺ����������ʧ�� return FALSE;
	///< ���� return TRUE;
}

/**
* @author ACM2012
* @param [in] <buffer> unsigned short *���ͣ�ָ��Ҫ�������͵����ݵĵ�һ���ֽ�
* @return ���ؼ�����ɵļ����
* @note
*/
unsigned short CMyIP::GetChecksum(IP_msg *buffer)
{
	unsigned long sum = 0; ///< ����һ���޷��ų�����  
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

/** @brief ���ݲ˵��ṩ��ѡ��ѡ��ѡ·�㷨 */
void CMyIP::select_algorithm(int mode)
{
	if (mode == SELECT_D) ///< �ڲ˵���ѡȡ��·״̬�㷨
		Dijkstra(n, 1, dist, prev, c);
	else    ///< �ڲ˵���ѡȡ���������㷨
		Bellman_Ford(n, 1, dist, prev, c);
}

/** @brief nΪ·����������vΪԴ�ڵ㣬*distΪ�����㷨���б��ε�������Դ�ڵ㵽Ŀ��v����ͷ���·���ķ���,
*	*prevΪ��Դ�ڵ�v���ŵ�ǰ��ͷ���·����ǰһ�ڵ�,
*   c[MAXSIZE][MAXSIZE]Ϊ�ӽڵ�x���ڵ�y����·���þ���,
*   ���x��y����ֱ���ģ��� c(x,y)=maxint
*   ��·״̬ѡ·�㷨��ʵ��.
*/
void CMyIP::Dijkstra(int n, int v, int *dist, int *prev, int c[MAXSIZE][MAXSIZE])
{

	bool s[maxnum];///< s���������ж�i�ڵ��Ƿ��Ѱ���ͷ��õ������ֵΪ1������ֵΪ
	///< ���ȳ�ʼ��
	for (int i = 1; i <= n; ++i)
	{
		dist[i] = c[v][i];///< ��Դ�ڵ㵽�����ڵ�ķ��ø���ֵ

		s[i] = 0;  ///< ����ʼ��ʱ���нڵ㶼��ֵΪû�а���ͷ��õ���
		///< �ж�Դ�ڵ�������ڵ��Ƿ�Ϊֱ���ģ�������ýڵ��ǰ�ڵ�Ϊ0������ΪԴ�ڵ�
		if (dist[i] == maxint)
			prev[i] = 0;
		else
			prev[i] = v;
	}
	dist[v] = 0;///< ���Դ�ڵ�ķ��ú����ܵ���ڵ�ĸ�ֵ
	s[v] = 1;///<  Դ�ڵ��Ѱ���ͷ��õ���
	///<  ���ν�δ����S���ϵĽ���У�ȡdist[]��Сֵ�Ľ�㣬������S�У�һ��S����������V�ж��㣬dist�ͼ�¼�˴�Դ�㵽������������֮������·������
	for (int i = 2; i <= n; ++i)
	{
		int tmp = maxint;
		int u = v;
		///<  �ҳ���ǰδʹ�õĵ�j��dist[j]��Сֵ
		for (int j = 1; j <= n; ++j)
		if ((!s[j]) && dist[j]<tmp)
		{
			///<  u���浱ǰ�ڽӵ��з�����С�ĵ�ĺ���
			u = j;
			tmp = dist[j];
		}
		s[u] = 1;///<  ��ʾu���Ѵ���S������
		///< ����j����dist
		for (int j = 1; j <= n; ++j)
		if ((!s[j]) && c[u][j]<maxint)
		{
			int newdist = dist[u] + c[u][j];
			///<  ����µķ��ñ�ԭ��С�������µķ���
			if (newdist < dist[j])
			{
				dist[j] = newdist;
				prev[j] = u;///<  ��u��ǰ�ڵ㸳Ϊj
			}
		}
	}
}

/** @brief  nΪ�ڵ�����dis[x][y]��ʾ�ڵ�x��y�ľ��룬 *D[x]��ʾ�ڵ�x�ľ���������
  * V[x][y]=1��ʾ�ڵ�x��y��һ���ߣ� cost[x][y]��ʾ�ڵ�x��y�ľ���
  * ���������㷨��ʵ��
  */
void CMyIP::Bellman_Ford(int n, int *D[x], int dis[n][n], int V[n][n], int cost[n][n])
{
	int dis[n][n]��///<  dis[x][y]��ʾ�ڵ�x��y�ľ���
		int D[n][n];      ///<  *D[x]��ʾ�ڵ�x�ľ�������
	int V[n][n];  ///<  V[x][y]=1��ʾ�ڵ�x��y��һ����
	int cost[n][n]; ///<  cost[x][y]��ʾ�ڵ�x��y�ľ���
	define MAX = 999999;
	void send(D[x], y); ///< send��*D[x]��y����ʾ��ڵ�y����x�Ľڵ�����
	int wait();   ///< wait()���ڼ���Ǹ��ڵ���յ����µľ�������

	///< ��ʼ��
	for (int x = 0; x<n; x++)
	{
		for (int y = 0; y<n; n++)
		{
			if (V[x][y] == 1) ///<  ���yΪx���ھ�
				dis[x][y] = c[x][y];  ///<  dis[x][y]=cost[x][y]
			else
				dis[x][y] = MAX;  ///< ���y��Ϊx���ھӣ�dis[x][y]Ϊ�����
			D[x][y] = dis[x][y]; ///<  ��ʼ����������ΪD[x][y]=dis[x][y]
		}
		for (int y = 0; y<n; n++)
			send(*D[x], y);    ///< ��x�ľ����������͵�ÿ���ھӽڵ�
	}

	///< ��ص��ڵ�x���յ����µľ�������
	while��(x = wait())�� = 0��
	{
		int change = 0;
		int dis1[n][n];
		for (int y = 0; y<n; n++)
		{
			dis1[x][y] = min(cost[x][y] + D[x][y]);
			if (dis[x][y] != dis[x][y])
				change = 1;
			dis[x][y] = min(cost[x][y] + D[x][y]); D[x][y] = dis[x][y]; ///< ����dis[x][y]��D[x][y]

		}
		if (change)
		for (int y = 0; y<n; n++)
			send(*D[x], y);
	}
}

/** @brief ������ѡ��ѡ·�㷨���ҳ���ԭ��ַv��Ŀ�ĵ�ַu����ͷ���·�����������ײ�ֵu��ȷ��һ����v��ת���� */
void CMyIP::searchPath(int *prev, int v, int u)
{
	int que[maxnum];///< ת����
	int tot = 1;    ///< ���ڻ��ݲ���¼�Ѿ����ݵĽڵ���
	que[tot] = u;   ///< Ŀ�ĵ�ַu
	tot++;          ///< �Ѵ�Ŀ�ĵ�ַ��ǰ�ҵ�����
	int tmp = prev[u];
	///< ��Ŀ�ĵ�ַ������ǰ���ݣ�ֱ���ҵ�ԭ��ַΪֹ��ÿ�ξ�ǰ�󽻻����Եó�����ԭ��ַ��Ŀ�ĵ�ַ��ת����
	while (tmp != v)
	{
		que[tot] = tmp;
		tot++;
		tmp = prev[tmp];
	}
	que[tot] = v;
	///< �����ԭ��ַ��Ŀ�ĵ�ַ��ת����
	for (int i = tot; i >= 1; --i)
	if (i != 1)
		cout << que[i] << " -> ";
	else
		cout << que[i] << endl;
}