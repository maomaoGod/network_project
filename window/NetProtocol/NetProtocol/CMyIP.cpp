/**@file
*@brief
*@author ACM2012
*@date 2015.4.18
*@version <0.1>
*@note
*定义CMyIP类的文件
*/
#pragma once
#include "stdafx.h"
#include "CMyIP.h"
#include "TransLayer.h"
#include "TransLayerTools.h"

/******************************************************
*
*			  222.20.101.215
*             /				\
*            /				 \
*           /			      \
*  192.168.44.1                222.20.101.58
*			\					   /
*			 \				      /
*             \				     /
*       222.20.101.43----222.20.101.54
********************************************************/

/*192.168.191.2, 192.168.191.3, 192.168.191.4, 192.168.191.5, 192.168.191.8 */
unsigned int IpNum[IPN] = { 0, 3232241409, 3232241411, 3232241414, 3232241415, 3232284420 };
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
	_offset = 0;
	sip = IpNum[1];
	dip = IpNum[5];
	info = new Route_info;
	LsData = new LS_data;
	DvData = new DV_data;
	if (Routing_select)
	{
		info->edgenum = 2;
		info->node = 3;
		info->sid = 1;
		info->E[0].u = 1;
		info->E[0].v = 2;
		info->E[1].u = 1;
		info->E[1].v = 3;
		/*
		info->edgenum = 2;
		info->node = 3;
		info->sid = 2;
		info->did = 5;
		info->E[0].u = 2;
		info->E[0].v = 1;
		info->E[1].u = 2;
		info->E[1].v = 5;
		*/
		/*
		info->edgenum = 2;
		info->node = 3;
		info->sid = 3;
		info->did = 5;
		info->E[0].u = 3;
		info->E[0].v = 1;
		info->E[1].u = 3;
		info->E[1].v = 4;
		*/
		/*
		info->edgenum = 2;
		info->node = 3;
		info->sid = 4;
		info->did = 5;
		info->E[0].u = 4;
		info->E[0].v = 3;
		info->E[1].u = 4;
		info->E[1].v = 5;
		*/
		/*
		info->edgenum = 2;
		info->node = 3;
		info->sid = 5;
		info->did = 5;
		info->E[0].u = 5;
		info->E[0].v = 2;
		info->E[1].u = 5;
		info->E[1].v = 4;
		*/
	}
	else
	{
		info->edgenum = 5;
		info->node = 5;
		info->sid = 1;
		info->did = 5;
		info->E[0].u = 1;
		info->E[0].v = 2;
		info->E[1].u = 1;
		info->E[1].v = 3;
		info->E[2].u = 2;
		info->E[2].v = 5;
		info->E[3].u = 3;
		info->E[3].v = 4;
		info->E[4].u = 4;
		info->E[4].v = 5;
	}
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
* @param [in] wparam表示传输层传过来的数据包结构指针, lparam表示传输层传过来的参数.
* @return 返回FALSE说明发送到链路层的消息失败，返回TRUE说明发送成功
* @note 函数功能主要是将Msg结构和IP地址等信息分片组装成IP_msg发送给链路层
*/
BOOL CMyIP::IP2Link(WPARAM wparam, LPARAM lparam)
{
	///< 将运输层送来的Msg结构和IP地址插入到IP_msg结构中,
	///< 如果信息超过容量就进行分片处理, 
	///< 调用链路层的发送函数如果发送失败 return FALSE;
	///< 否则 return TRUE;
	if (end_connect)
	{
		int offset = 0, ident = 0;
		ident++;
		IP_data = (struct Msg*)wparam;
		MyIP = new ip_message;
		NetoLink = new IP_Msg;

		MyIP->ih_version = 4;
		MyIP->ip_hdr_length = 20;
		MyIP->ih_sever = 0;

		MyIP->ih_ident = ident;


		MyIP->ih_TTL = 100;
		MyIP->ih_protl = IP_data->protocol;

		MyIP->ih_saddr = IP_data->sip;
		MyIP->ih_daddr = IP_data->dip;                              
		
		NetoLink->ih_saddr = IP_data->sip;
		NetoLink->ih_daddr = iIP;

		while (IP_data->datelen - 8 * offset > MSS)
		{
			MyIP->ih_flags = 1;
			MyIP->ih_offset = offset;
			offset = offset + MSS / 8;
			MyIP->ih_data_len = MSS + 20;
			memcpy(MyIP->data, IP_data->data, MSS);
			MyIP->ip_checksum = iphdrmakesum(MyIP);
			NetoLink->data = (char *)MyIP;
			NetoLink->ih_len = MSS + 20;
			(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)NetoLink, lparam);
		}

		MyIP->ih_flags = 0;
		MyIP->ih_offset = offset;
		MyIP->ih_data_len = IP_data->datelen - 8 * offset + 20;
		memcpy(MyIP->data, IP_data->data, IP_data->datelen - 8 * offset);
		MyIP->ip_checksum = iphdrmakesum(MyIP);
		NetoLink->data = (char *)MyIP;
		NetoLink->ih_len = IP_data->datelen - 8 * offset + 20;
		(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)NetoLink, lparam);
	}
	else
	{
		MyIP = (struct ip_message*)wparam;
		if (MyIP->ip_checksum != iphdrmakesum(MyIP))
		{

		}
		else if (MyIP->ih_protl == PROTOCOL_TCP)
		{
			tcp_message* mytcp = (struct tcp_message*)MyIP->data;
			if ((MyIP->ih_saddr & 0xffffff00) - (sip & 0xffffff00) == 0)
			{
				In_NAT* Nat = new In_NAT;
				Nat->SIP = MyIP->ih_saddr;
				Nat->DIP = MyIP->ih_daddr;
				Nat->Sport = mytcp->tcp_src_port;
				Nat->Dport = mytcp->tcp_dst_port;
				Nat = InToOut.NAT(Nat);
				mytcp->tcp_src_port = Nat->Sport;
				mytcp->tcp_dst_port = Nat->Dport;
				memcpy(MyIP->data, &mytcp, strlen(MyIP->data));
				MyIP->ih_saddr = Nat->SIP;
				MyIP->ih_daddr = Nat->DIP;
				MyIP->ip_checksum = iphdrmakesum(MyIP);
			}
		}
		else if (MyIP->ih_protl == PROTOCOL_UDP)
		{
			udp_message* myudp = (struct udp_message*)MyIP->data;
			if ((MyIP->ih_saddr & 0xffffff00) - (sip & 0xffffff00) == 0)
			{
				In_NAT* Nat = new In_NAT;
				Nat->SIP = MyIP->ih_saddr;
				Nat->DIP = MyIP->ih_daddr;
				Nat->Sport = myudp->udp_src_port;
				Nat->Dport = myudp->udp_dst_port;
				Nat = InToOut.NAT(Nat);
				myudp->udp_src_port = Nat->Sport;
				myudp->udp_dst_port = Nat->Dport;
				memcpy(MyIP->data, &myudp, strlen(MyIP->data));
				MyIP->ih_saddr = Nat->SIP;
				MyIP->ih_daddr = Nat->DIP;
				MyIP->ip_checksum = iphdrmakesum(MyIP);
			}
		}
		NetoLink = new IP_Msg;
		NetoLink->data = (char *)MyIP;
		NetoLink->ih_len = MyIP->ih_data_len;
		NetoLink->ih_saddr = sip;
			
		if (MyIP->ih_daddr == oIP)
		{
			NetoLink->ih_daddr = MyIP->ih_daddr;
			(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)NetoLink, lparam);
		}
		else
		{
			NetoLink->ih_daddr = MyIP->ih_daddr;
			(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)NetoLink, lparam);
			/*
			if (Routing_select)
			{
			Route2LS(info, LsData);
			NetoLink->ih_daddr = Num2IP(Route.LS(LsData));
			}
			else
			{
			Route2DV(info, DvData);
			NetoLink->ih_daddr = Num2IP(Route.DV(DvData));
			}*/
		}
	}
	
	return 0;
}

/**
* @author ACM2012
* @param [in] wparam表示传输层传过来的数据包结构指针, lparam表示传输层传过来的参数.
* @return 返回FALSE说明没有发送成功，返回TRUE说明发送成功
* @note 函数功能主要是链路层发送过来的信息分解成Msg结构, 发送给运输层
*/
Bool CMyIP::IP2Trans(WPARAM wparam, LPARAM lparam)
{
	///< 根据链路层发送的数据进行剥离得到报文长度以及偏移, 比较偏移量是否等于报文长度
	///< 若发现分片缺失或者检验和出错则 return FALSE;
	///< 若是则数据成功接收 进行少量的检验和检查, 若没有错误
	///< 则将IP_msg结构剥离出Msg结构
	if (end_connect)
	{
		int ident = 1;
		MyIP = (struct ip_message*)wparam;
		struct Msg *local_IP_data = new Msg;
		if (MyIP->ip_checksum != iphdrmakesum(MyIP))
		{
			
		}
		else if (MyIP->ih_ident == ident)
		{
			if (MyIP->ih_offset * 8 == _offset)
			{
				if (MyIP->ih_flags){
					memcpy(local_IP_data->data + _offset, MyIP->data, MSS);
					_offset = _offset + MSS;
				}
				else{
					memcpy(local_IP_data->data + _offset, MyIP->data, MyIP->ih_data_len - 20 - _offset);
					_offset = 0;
					local_IP_data->sip = MyIP->ih_saddr;
					local_IP_data->dip = MyIP->ih_daddr;
					local_IP_data->protocol = MyIP->ih_protl;
					local_IP_data->datelen = MyIP->ih_data_len - 20;
					(AfxGetApp()->m_pMainWnd)->SendMessage(TRANSTOAPP, (WPARAM)local_IP_data, lparam);
				}
			}
		}
	}
	else
		(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOLINK, wparam, lparam);

	return 0;
}

/**
* @author ACM2012
* @param [in] wparam表示传输层传过来的数据包结构指针, lparam表示传输层传过来的参数.
* @return 返回FALSE说明没有发送成功，返回TRUE说明发送成功
* @note 函数功能主要是链路层发送过来的信息得到选路的数据
*/
BOOL CMyIP::RecvMsg(WPARAM wparam, LPARAM lparam)
{
	///< 根据链路层发送的数据进行剥离得到源节点的连接信息
	///< 根据源节点与目的节点的连接将info结构进行更新

	MyIP = (struct ip_message*)wparam;
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
* @note 函数功能选路的数据的选路算法得到下一个目的IP
*/
BOOL CMyIP::SendMsg(WPARAM wparam, LPARAM lparam)
{
	///< 通过选择两种选路算法, 得到下一个节点的IP
	/*
	int test = 1;
	if (test)
	{
		MyIP = new ip_message;
		Route_info *in;
		in = new Route_info;
		in->sid = 2;
		in->did = 1;
		in->node = 3;
		in->edgenum = 2;
		in->E[0].u = 2;
		in->E[0].v = 1;
		in->E[1].u = 2;
		in->E[1].v = 5;
		MyIP->data = (char *)in;

		MyIP->ih_version = 4;
		MyIP->ip_hdr_length = 20;
		MyIP->ih_sever = 0;
		MyIP->ih_data_len = MSS + 20;

		MyIP->ih_ident = 0;
		MyIP->ih_flags = 0;
		MyIP->ih_offset = 0;

		MyIP->ih_TTL = 100;
		MyIP->ih_protl = 17;
		MyIP->ip_checksum = 0;

		int m = IP2Num(sip);
		for (int i = 0; i < info->edgenum; i++)
		{
			if (info->E[i].u == m)
			{
				MyIP->ih_daddr = Num2IP(info->E[i].v);
				(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOTRANS, (WPARAM)MyIP, lparam);
			}
			else if (info->E[i].v == m)
			{
				MyIP->ih_daddr = Num2IP(info->E[i].u);
				(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOTRANS, (WPARAM)MyIP, lparam);
			}
		}
	}
	*/
	MyIP = new ip_message;
	memcpy(MyIP->data, info, MSS);

	MyIP->ih_version = 4;
	MyIP->ip_hdr_length = 20;
	MyIP->ih_sever = 0;
	MyIP->ih_data_len = MSS + 20;

	MyIP->ih_ident = 0;
	MyIP->ih_flags = 0;
	MyIP->ih_offset = 0;

	MyIP->ih_TTL = 100;
	MyIP->ih_protl = 17;
	MyIP->ip_checksum = 0;

	MyIP->ih_saddr = sip;

	NetoLink->ih_saddr = sip;
	NetoLink->ih_len = MSS + 20;

	int m = IP2Num(sip);
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
unsigned int CMyIP::Num2IP(int num)
{
	return IpNum[num];
}

/**
* @author ACM2012
* @param [in] IP
* @return 对应的数组下标
* @note 函数功能主要是从IP得到对应的数组下标
*/
BOOL CMyIP::IP2Num(unsigned int IP)
{
	for (int i = 1; i <= IPN; i++)
	{
		if (IpNum[i] == IP)
			return i;
	}

	return 0;
}

/** @brief 计算IP数据报的首部检验和 */
unsigned short CMyIP::iphdrmakesum(ip_message *ip)
{
	unsigned int sum = 0;
	sum += ((ip->ih_version & 0xf000) + (ip->ip_hdr_length & 0x0f00) + (ip->ih_sever & 0x00ff));
	sum += (ip->ih_data_len & 0xffff);
	sum += (ip->ih_ident & 0xffff); 
	sum += ((ip->ih_flags & 0xe000) + (ip->ih_offset & 0x1fff));
	sum += ((ip->ih_TTL & 0xff00) + (ip->ih_protl & 0x00ff));
	sum += ((ip->ih_saddr & 0xffff0000) >> 8 + (ip->ih_saddr & 0x0000ffff));
	sum += ((ip->ih_daddr & 0xffff0000) >> 8 + (ip->ih_daddr & 0x0000ffff));
	sum = ((sum & 0xffff0000) >> 8 + (sum & 0x0000ffff));
	sum = ~sum;
	return sum;
}

/** @brief 从路由信息结构中提取到LS_data信息结构 */
void CMyIP::Route2LS(Route_info *info, LS_data *LsData)  
{
	LsData->LsData.node = info->node;
	LsData->LsData.sid = info->sid;
	LsData->LsData.did = info->did;
	for (int i = 1; i < IPN; i++)
	{
		for (int j = i+1; j < IPN; j++)
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

/** @brief 从路由信息结构中提取到DV_data信息结构 */
void CMyIP::Route2DV(Route_info *info, DV_data *DvData)
{
	DvData->DvData.node = info->node;
	DvData->DvData.sid = info->sid;
	DvData->DvData.did = info->did;
	for (int i = 0; i < info->edgenum; i++)
		DvData->E[i] = info->E[i];
	DvData->edgenum = info->edgenum;
}

/*
BOOL CMyIP::IP2Link(WPARAM wparam, LPARAM lparam)
{
	///< 将运输层送来的Msg结构和IP地址插入到IP_msg结构中,
	///< 如果信息超过容量就进行分片处理, 
	///< 调用链路层的发送函数如果发送失败 return FALSE;
	///< 否则 return TRUE;
	MyIP = new IP_Msg;
	int offset = 0, ident = 0;
	ident++;
	IP_data = (struct Msg*)wparam;
	while (IP_data->datelen - 8 * offset > MAXSIZE)
	{
		IP_HEADER.ih_protl = IP_data->protocol;
		IP_HEADER.ih_saddr = IP_data->sip;
		IP_HEADER.ih_daddr = IP_data->dip;
		IP_HEADER.ih_flags = 1;
		IP_HEADER.ih_ident = ident;
		IP_HEADER.ih_offset = offset;
		IP_HEADER.ih_len = strlen(IP_data->data);
		offset = offset + MAXSIZE / 8;
		IP_HEADER.ih_version = 4;
		IP_HEADER.ih_sport = IP_data->ih_sport;
		IP_HEADER.ih_dport = IP_data->ih_dport;
		MyIP->iphdr = &IP_HEADER;
		memcpy(MyIP->data, IP_data->data, MAXSIZE);
		(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)MyIP, lparam);
	}
	IP_HEADER.ih_protl = IP_data->protocol;
	IP_HEADER.ih_saddr = IP_data->sip;
	IP_HEADER.ih_daddr = IP_data->dip;
	IP_HEADER.ih_flags = 0;
	IP_HEADER.ih_ident = ident;
	IP_HEADER.ih_offset = offset;
	IP_HEADER.ih_len = IP_data->datelen;
	IP_HEADER.ih_version = 4;
	IP_HEADER.ih_sport = IP_data->ih_sport;
	IP_HEADER.ih_dport = IP_data->ih_dport;
	MyIP->iphdr = &IP_HEADER;
	memcpy(MyIP->data, IP_data->data, IP_data->datelen - 8 * offset);
	(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)MyIP, lparam);
	return 0;
}

Bool CMyIP::IP2Trans(WPARAM wparam, LPARAM lparam)
{
	///< 根据链路层发送的数据进行剥离得到报文长度以及偏移, 比较偏移量是否等于报文长度
	///< 若发现分片缺失或者检验和出错则 return FALSE;
	///< 若是则数据成功接收 进行少量的检验和检查, 若没有错误
	///< 则将IP_msg结构剥离出Msg结构
	int ident = 1;
	MyIP = (struct IP_Msg*)wparam;
	struct Msg *local_IP_data= new Msg;
	if (MyIP->iphdr->ih_ident == ident)
	{
		if (MyIP->iphdr->ih_offset * 8 == _offset)
		{
			if (MyIP->iphdr->ih_flags){
				memcpy(local_IP_data->data + _offset, MyIP->data, MAXSIZE);
				_offset = _offset + MAXSIZE;
			}
			else{
				memcpy(local_IP_data->data + _offset, MyIP->data, MyIP->iphdr->ih_len - _offset);
				_offset = 0;
				local_IP_data->sip = MyIP->iphdr->ih_saddr;
				local_IP_data->dip = MyIP->iphdr->ih_daddr;
				local_IP_data->ih_sport = MyIP->iphdr->ih_sport;
				local_IP_data->ih_dport = MyIP->iphdr->ih_dport;
				local_IP_data->protocol = MyIP->iphdr->ih_protl;
				local_IP_data->datelen = MyIP->iphdr->ih_len;
				(AfxGetApp()->m_pMainWnd)->SendMessage(TRANSTOAPP, (WPARAM)local_IP_data, lparam);
			}
		}
	}
	return 0;
}
*/