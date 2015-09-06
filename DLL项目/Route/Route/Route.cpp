#include "stdafx.h"
#include "Route.h"

unsigned int IpNum[N] = {0};

/*
 * NAT协议模块
 */


/**
* @author ACM2012
* @param
* @exception
* @return 无
* @note 路由类的构造函数
*/
Route::Route()
{
	iIP = getLocalIP();
	oIP = getPublicIP();
	info = new Route_info;               ///<定义路由信息
	LsData = new LS_data;				 ///<定义LS算法的路由信息	
	DvData = new DV_data;				 ///<定义DV算法的路由信息
	IPN = 1;
}

/**
* @author ACM2012
* @param
* @exception
* @return 无
* @note 路由类的析构函数
*/
Route::~Route()
{

}

void Route::RS(int Routing_Select)
{
	LD = Routing_Select;
}

int Route::Datainfo(WPARAM wparam, LPARAM lparam)
{
	Routeip_message *Routest = (struct Routeip_message*)wparam;
	if (Routest->ih_ident == data_info)
	{
		if (!IP2Link(wparam, lparam))
			return true;
		else
		{
			AfxMessageBox(_T("Data convey Net to Link have errors.\n\n"));
			return false;
		}
	}
	else
	{
		if (!RecvMsg(wparam, lparam))
			return true;
		else
		{
			AfxMessageBox(_T("Route convey Net to Link have errors.\n\n"));
			return false;
		}
	}
}

/**
* @author ACM2012
* @param
* @exception
* @return 无符号整形表示的本地IP地址
* @note 函数
*/
unsigned int Route::getLocalIP()
{
	WSADATA wsaData;
	char name[155];
	char *ip;
	PHOSTENT hostinfo;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)
	{
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				// 这些就是获得IP的函数
				ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			}
		}
		WSACleanup();
	}
	// 将ip从字符串转为unsigned int
	unsigned int ip_number = 0;
	unsigned int ip_seg_number = 0;
	for (int i = 0; i < 15 && ip[i]; ++i)
	{
		if (ip[i] == '.')
		{
			ip_number = ip_number * 256 + ip_seg_number;
			ip_seg_number = 0;
		}
		else
		{
			ip_seg_number = ip_seg_number * 10 + ip[i] - 48;
		}
	}
	ip_number = ip_number * 256 + ip_seg_number;
	return ip_number;
}

/**
* @author ACM2012
* @param
* @exception
* @return 无符号整形表示的外网IP地址
* @note 函数
*/
unsigned int Route::getPublicIP()
{
	char buf[MAX_PATH] = { 0 };
	char chTempIp[128];
	char chIP[64];
	URLDownloadToFile(NULL, _T("http://www.ip138.com/ip2city.asp"), _T("1.txt"), 0, NULL);

	FILE *fp;
	int t = fopen_s(&fp, "1.txt", "r");

	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_SET);
		fread(buf, 1, 256, fp);
		fclose(fp);
		char* iIndex = strstr(buf, "[");

		if (iIndex)
		{
			sprintf_s(chTempIp, "%s", iIndex);
			int nBuflen = strlen(chTempIp);

			for (int i = 0; i<nBuflen; i++)
			{
				chIP[i] = chTempIp[i + 1];

				if (chTempIp[i] == ']')
				{
					chIP[i - 1] = '\0';
					DeleteFile(_T("1.txt"));
					continue;
				}

			}
		}
	}
	// 将ip从字符串转为unsigned int
	unsigned int ip_number = 0;
	unsigned int ip_seg_number = 0;
	for (int i = 0; i < 15 && chIP[i]; ++i)
	{
		if (chIP[i] == '.')
		{
			ip_number = ip_number * 256 + ip_seg_number;
			ip_seg_number = 0;
		}
		else
		{
			ip_seg_number = ip_seg_number * 10 + chIP[i] - 48;
		}
	}
	ip_number = ip_number * 256 + ip_seg_number;
	return ip_number;
}

/**
* @author ACM2012
* @param [in] 输入NAT的结构
* @return 返回更新后NAT结构
* @note 函数功能:主要是内部IP与端口对应起来
*/
In_NAT* Route::NAT(In_NAT* Nat, int &flag)
{
	NAT_translation_table *p1 = new NAT_translation_table;
	NAT_translation_table *p2 = new NAT_translation_table;
	flag = 0;
	if (Nat->DIP == oIP)               ///<  外网访问内网，根据端口号改写目的IP和目的端口
	{
		p1 = p;
		while (p1 != NULL)
		{
			if (p1->WAN_port == Nat->Dport)    ///< 
			{
				Nat->DIP = p1->LAN_IP;
				Nat->Dport = p1->LAN_port;
				break;
			}
			p1 = p1->next_p;
		}
		if (NULL == p1)
			flag = 1;
	}
	else                      ///<  内网访问外网，改写源IP和源端口，并添加NAT转换表项
	{
		p1 = p;
		if (p != NULL)
		{
			while (p1->next_p != NULL){
				if ((p1->LAN_IP == Nat->SIP) && (p1->LAN_port == Nat->Sport)) ///< 已存在表项，不添加
				{
					Nat->Sport = p1->WAN_port;
					break;
				}
				p1 = p1->next_p;
			}
			if (p1->next_p == NULL){
				if ((p1->LAN_IP == Nat->SIP) && (p1->LAN_port == Nat->Sport))
					Nat->Sport = p1->WAN_port;
				else{
					p2 = new NAT_translation_table;
					p2->WAN_IP = oIP;
					p2->WAN_port = p1->WAN_port + 1;
					p2->LAN_IP = Nat->SIP;
					p2->LAN_port = Nat->Sport;
					p2->next_p = NULL;
					p1->next_p = p2;
					Nat->Sport = p2->WAN_port;
				}
			}
		}
		else
		{
			p2 = new NAT_translation_table;
			p2->WAN_IP = oIP;
			p2->WAN_port = 80 + 1;
			p2->LAN_IP = Nat->SIP;
			p2->LAN_port = Nat->Sport;
			p2->next_p = NULL;
			p = p2;
			Nat->Sport = p->WAN_port;
		}
		Nat->SIP = oIP;
	}
	return Nat;
}

/**
* @author ACM2012
* @param [in] ip数据报首部
* @return 首部检验和
* @note 函数功能主要是从IP计算IP数据报的首部检验和
*/
unsigned short Route::iphdrmakesum(Routeip_message *ip)
{
	unsigned int sum = 0;
	sum += ((ip->ih_version & 0xf000) + (ip->ip_hdr_length & 0x0f00) + (ip->ih_sever & 0x00ff));
	sum += (ip->ih_data_len & 0xffff);
	sum += (ip->ih_ident & 0xffff);
	sum += ((ip->ih_flags & 0xe000) + (ip->ih_offset & 0x1fff));
	sum += ((ip->ih_TTL & 0xff00) + (ip->ih_protl & 0x00ff));
	sum += (((ip->ih_saddr & 0xffff0000) >> 8) + (ip->ih_saddr & 0x0000ffff));
	sum += (((ip->ih_daddr & 0xffff0000) >> 8) + (ip->ih_daddr & 0x0000ffff));
	sum = (((sum & 0xffff0000) >> 8) + (sum & 0x0000ffff));
	sum = ~sum;
	return sum;
}

int Route::IP2Link(WPARAM wparam, LPARAM lparam)
{
	int nRetCode = 0;
	struct Routeip_message *MyIP = (struct Routeip_message*)wparam;
	struct RouteIP_Msg *NetoLink = new RouteIP_Msg;
	if (MyIP->ip_checksum != iphdrmakesum(MyIP))
	{
		return 1;
	}
	else if (MyIP->ih_protl == RoutePROTOCOL_TCP)
	{
		Routetcp_message* mytcp = (struct Routetcp_message*)MyIP->data;
		if (((MyIP->ih_saddr & 0xffffff00) - (iIP & 0xffffff00) == 0)||(MyIP->ih_daddr==oIP)){
			In_NAT* Nat = new In_NAT;
			Nat->SIP = MyIP->ih_saddr;
			Nat->DIP = MyIP->ih_daddr;
			Nat->Sport = mytcp->tcp_src_port;
			Nat->Dport = mytcp->tcp_dst_port;
			Nat = NAT(Nat,nRetCode);
			if (nRetCode)
				return 1;
			mytcp->tcp_src_port = Nat->Sport;
			mytcp->tcp_dst_port = Nat->Dport;
			memcpy(MyIP->data, &mytcp, strlen(MyIP->data));
			MyIP->ih_saddr = Nat->SIP;
			MyIP->ih_daddr = Nat->DIP;
			MyIP->ip_checksum = iphdrmakesum(MyIP);
			NetoLink->data = (char *)MyIP;
			NetoLink->ih_len = MyIP->ih_data_len;
			NetoLink->ih_saddr = MyIP->ih_saddr;
			NetoLink->ih_daddr = MyIP->ih_daddr;
			(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)NetoLink, lparam);
		}
		else{
			struct RouteIP_Msg *NetoLink = new RouteIP_Msg;
			NetoLink->data = (char *)MyIP;
			NetoLink->ih_len = MyIP->ih_data_len;
			NetoLink->ih_saddr = MyIP->ih_saddr;
			NetoLink->ih_daddr = MyIP->ih_daddr;
			if (LD)
			{
				info->sid = IP2Num(oIP);
				info->did = IP2Num(NetoLink->ih_daddr);
				Route2LS(info, LsData);
				NetoLink->ih_daddr = Num2IP(LS(LsData));
			}
			else
			{
				info->sid = IP2Num(oIP);
				info->did = IP2Num(NetoLink->ih_daddr);
				Route2DV(info, DvData);
				NetoLink->ih_daddr = Num2IP(DV(DvData));
			}
			(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)NetoLink, lparam);
		}
	}
	else if (MyIP->ih_protl == RoutePROTOCOL_UDP)
	{
		Routeudp_message* myudp = (struct Routeudp_message*)MyIP->data;
		if (((MyIP->ih_saddr & 0xffffff00) - (iIP & 0xffffff00) == 0) || (MyIP->ih_daddr == oIP)){
			In_NAT* Nat = new In_NAT;
			Nat->SIP = MyIP->ih_saddr;
			Nat->DIP = MyIP->ih_daddr;
			Nat->Sport = myudp->udp_src_port;
			Nat->Dport = myudp->udp_dst_port;
			Nat = NAT(Nat,nRetCode);
			if (nRetCode)
				return 1;
			myudp->udp_src_port = Nat->Sport;
			myudp->udp_dst_port = Nat->Dport;
			memcpy(MyIP->data, &myudp, strlen(MyIP->data));
			MyIP->ih_saddr = Nat->SIP;
			MyIP->ih_daddr = Nat->DIP;
			MyIP->ip_checksum = iphdrmakesum(MyIP);
			NetoLink->data = (char *)MyIP;
			NetoLink->ih_len = MyIP->ih_data_len;
			NetoLink->ih_saddr = MyIP->ih_saddr;
			NetoLink->ih_daddr = MyIP->ih_daddr;
			(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)NetoLink, lparam);
		}
		else{
			struct RouteIP_Msg *NetoLink = new RouteIP_Msg;
			NetoLink->data = (char *)MyIP;
			NetoLink->ih_len = MyIP->ih_data_len;
			NetoLink->ih_saddr = MyIP->ih_saddr;
			NetoLink->ih_daddr = MyIP->ih_daddr;
			if (LD)
			{
				info->sid = IP2Num(oIP);
				info->did = IP2Num(NetoLink->ih_daddr);
				Route2LS(info, LsData);
				NetoLink->ih_daddr = Num2IP(LS(LsData));
			}
			else
			{
				info->sid = IP2Num(oIP);
				info->did = IP2Num(NetoLink->ih_daddr);
				Route2DV(info, DvData);
				NetoLink->ih_daddr = Num2IP(DV(DvData));
			}
			(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)NetoLink, lparam);
		}
	}
	
	return 0;
}



/*
 * 选路协议模块
 */


/**
* @author ACM2012
* @param [in] wparam表示传输层传过来的数据包结构指针, lparam表示传输层传过来的参数.
* @return 返回FALSE说明没有发送成功，返回TRUE说明发送成功
* @note 函数功能主要是链路层发送过来的信息得到选路的数据
*/
int Route::RecvMsg(WPARAM wparam, LPARAM lparam)
{
	///< 根据链路层发送的数据进行剥离得到源节点的连接信息
	///< 根据源节点与目的节点的连接将info结构进行更新
	struct Routeip_message* MyIP = (struct Routeip_message*)wparam;
	Route_info *Local_data = (struct Route_info*)MyIP->data;
	int tempedge = 0;
	int tempnode = 0;
	int unode[maxnum];
	int vnode[maxnum];
	int j, k;
	for (int i = 0; i < Local_data->edgenum; i++)
	{
		for (j = 0; j < info->edgenum; j++)
		{
			if ((info->E[j].u == Local_data->E[i].u && info->E[j].v == Local_data->E[i].v) || (info->E[j].u == Local_data->E[i].v && info->E[j].v == Local_data->E[i].u))
				break;
		}
		if (j == info->edgenum)
		{
			unode[tempedge] = Local_data->E[i].u;
			vnode[tempedge] = Local_data->E[i].v;
			tempedge++;
			for (k = 0; k < info->edgenum; k++)
			{
				if (info->E[k].u == Local_data->E[i].u || info->E[k].v == Local_data->E[i].v || (info->E[k].u == Local_data->E[i].v || info->E[k].v == Local_data->E[i].u))
				{
					tempnode++;
					break;
				}
			}
			if (k == info->edgenum)
			{
				tempnode += 2;
			}
		}
	}
	info->node += tempnode;
	for (int i = 0; i < tempedge; i++)
	{
		info->E[info->edgenum].u = unode[i];
		info->E[info->edgenum++].v = vnode[i];
	}

	return 0;
}

/**
* @author ACM2012
* @param [in] wparam表示自己封装的数据包结构指针, lparam表示若干参数.
* @return 返回0说明发送成功，其他说明发送失败
* @note 函数功能向周围路由器发送路由信息
*/
int Route::SendMsg(WPARAM wparam, LPARAM lparam)
{
	///< 通过选择两种选路算法, 得到下一个节点的IP
	struct Routeip_message *MyIP = new Routeip_message;
	memcpy(MyIP->data, info, ipMSS);

	MyIP->ih_version = 4;
	MyIP->ip_hdr_length = 20;
	MyIP->ih_sever = 0;
	MyIP->ih_data_len = ipMSS + 20;

	MyIP->ih_ident = 0;
	MyIP->ih_flags = 0;
	MyIP->ih_offset = 0;

	MyIP->ih_TTL = 100;
	MyIP->ih_protl = RoutePROTOCOL_UDP;
	MyIP->ip_checksum = 0;

	MyIP->ih_saddr = oIP;

	struct RouteIP_Msg *NetoLink = new RouteIP_Msg;
	NetoLink->ih_saddr = oIP;
	NetoLink->ih_len = ipMSS + 20;

	int m = IP2Num(oIP);
	for (int i = 0; i < info->edgenum; i++)
	{
		if (info->E[i].u == m)
		{
			MyIP->ih_daddr = Num2IP(info->E[i].v);
			NetoLink->ih_daddr = MyIP->ih_daddr;
			NetoLink->data = (char *)MyIP;
			(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)NetoLink, lparam);
		}
		else if (info->E[i].v == m)
		{
			MyIP->ih_daddr = Num2IP(info->E[i].u);
			NetoLink->ih_daddr = MyIP->ih_daddr;
			NetoLink->data = (char *)MyIP;
			(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)NetoLink, lparam);
		}
	}
	return 0;
}

/**
* @author ACM2012
* @param [in] 数组下标
* @return 对应的IP
* @note 函数功能主要是从数组的下标得到对应的IP
*/
unsigned int Route::Num2IP(int num)
{
	if (num < IPN)
		return IpNum[num];
	return 1;
}

/**
* @author ACM2012
* @param [in] IP
* @return 对应的数组下标
* @note 函数功能主要是从IP得到对应的数组下标
*/
BOOL Route::IP2Num(unsigned int ip)
{
	int i;
	for (i = 1; i < IPN; i++)
	{
		if (IpNum[i] == ip)
			return i;
	}
	if (i == IPN)
		IpNum[IPN++] = ip;
	return 0;
}

/**
* @author ACM2012
* @param [in] 初始节点, 目的节点, 之间相连节点的堆栈
* @return 当前节点应该向下传的一个节点
* @note 主要是寻找下一个节点
*/
int  Route::searchPath(int *pre, int v, int u)
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
* @param [in] 初始节点, 初始节点, 当前节点到源节点的最短路径长度, 当前节点的前一个节点, 记录图的两节点间路径长度
* @return 无
* @note 全局搜索最优的下一个节点
*/
void Route::Dijkstra(int n, int v, int *dist, int *pre, int c[maxnum][maxnum])
{
	int  s[maxnum];    ///< 判断是否已存入该点到S集合中
	for (int i = 1; i <= n; ++i)
	{
		dist[i] = c[v][i];
		s[i] = 0;     ///< 初始都未用过该点
		if (dist[i] == maxint)
			pre[i] = 0;
		else
			pre[i] = v;
	}
	dist[v] = 0;
	s[v] = 1;

	///< 依次将未放入S集合的结点中，取dist[]最小值的结点，放入结合S中
	///< 一旦S包含了所有V中顶点，dist就记录了从源点到所有其他顶点之间的最短路径长度
	for (int i = 2; i <= n; ++i)
	{
		int tmp = maxint;
		int u = v;
		///< 找出当前未使用的点j的dist[j]最小值
		for (int j = 1; j <= n; ++j)
		if ((!s[j]) && dist[j]<tmp)
		{
			u = j;              ///< u保存当前邻接点中距离最小的点的号码
			tmp = dist[j];
		}
		s[u] = 1;    ///< 表示u点已存入S集合中

		///< 更新dist
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

/**
* @author ACM2012
* @param [in] LS算法的路由信息结构
* @return 无
* @note 全局搜索最优的下一个节点
*/
int  Route::LS(LS_data *LSdata)
{
	Dijkstra(LSdata->LsData.node, LSdata->LsData.sid, LSdata->LsData.dist, LSdata->LsData.pre, LSdata->c);
	return searchPath(LSdata->LsData.pre, LSdata->LsData.sid, LSdata->LsData.did);
}

/**
* @author ACM2012
* @param [in] 节点数, 边数, 初始节点, 当前节点到源节点的最短路径长度, 当前节点的前一个节点, 记录图中所有的边
* @return 无
* @note 全局搜索最优的下一个节点
*/
void Route::Bellman_Ford(int n, int edgenum, int v, int *dist, int *pre, Edge edge[N])
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

/**
* @author ACM2012
* @param [in] DV算法的路由信息结构
* @return 无
* @note 全局搜索最优的下一个节点
*/
int  Route::DV(DV_data *DVdata)
{
	Bellman_Ford(DVdata->DvData.node, DVdata->edgenum, DVdata->DvData.sid, DVdata->DvData.dist, DVdata->DvData.pre, DVdata->E);
	return searchPath(DVdata->DvData.pre, DVdata->DvData.sid, DVdata->DvData.did);
}

/**
* @author ACM2012
* @param [in] info路由信息结构, LsData LS算法路由信息结构
* @return void
* @note 函数功能主要是从从路由信息结构中提取到LS_data信息结构
*/
void Route::Route2LS(Route_info *info, LS_data *LsData)
{
	LsData->LsData.node = info->node;
	LsData->LsData.sid = info->sid;
	LsData->LsData.did = info->did;
	for (int i = 1; i < IPN; i++)
	{
		for (int j = i + 1; j < IPN; j++)
		{
			LsData->c[i][j] = maxint;
			LsData->c[j][i] = maxint;
		}
	}
	for (int i = 0; i < info->edgenum; i++)
	{
		LsData->c[info->E[i].u][info->E[i].v] = 1;
		LsData->c[info->E[i].v][info->E[i].u] = 1;
	}
}

/**
* @author ACM2012
* @param [in] info路由信息结构, DvData Dv算法路由信息结构
* @return void
* @note 函数功能主要是从从路由信息结构中提取到DV_data信息结构
*/
/** @brief 从路由信息结构中提取到DV_data信息结构 */
void Route::Route2DV(Route_info *info, DV_data *DvData)
{
	DvData->DvData.node = info->node;
	DvData->DvData.sid = info->sid;
	DvData->DvData.did = info->did;
	for (int i = 0; i < info->edgenum; i++)
		DvData->E[i] = info->E[i];
	DvData->edgenum = info->edgenum;
}

