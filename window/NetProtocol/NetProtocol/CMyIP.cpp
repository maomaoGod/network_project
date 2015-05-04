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
* @param [in] 无
* @return 返回FALSE说明发送到链路层的消息失败，返回TRUE说明发送成功
* @note 函数功能主要是将Msg结构和IP地址等信息分片组装成IP_msg发送给链路层
*/
BOOL CMyIP::IP2Link(WPARAM wparam, LPARAM lparam)
{
	///< 将运输层送来的Msg结构和IP地址插入到IP_msg结构中,
	///< 如果信息超过容量就进行分片处理, 
	///< 调用链路层的发送函数如果发送失败 return FALSE;
	///< 否则 return TRUE;
	int offset = 0, ident = 0;
	ident++;
	while (strlen(IP_data.data) - 8 * offset > MAXSIZE)
	{
		MyIP.iphdr->ih_saddr = IP_data.sip;
		MyIP.iphdr->ih_daddr = IP_data.dip;
		MyIP.iphdr->ih_flags = 1;
		MyIP.iphdr->ih_ident = ident;
		MyIP.iphdr->ih_offset = offset;
		MyIP.iphdr->ih_len = strlen(IP_data.data) + 20;
		offset = offset + MAXSIZE / 8;
		MyIP.iphdr->ih_version = 4;
		strncpy_s(MyIP.data, MAXSIZE, IP_data.data, MAXSIZE);
		(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOLINK, wparam, lparam);
	}
	MyIP.iphdr->ih_saddr = IP_data.sip;
	MyIP.iphdr->ih_daddr = IP_data.dip;
	MyIP.iphdr->ih_flags = 0;
	MyIP.iphdr->ih_ident = ident;
	MyIP.iphdr->ih_offset = offset;
	MyIP.iphdr->ih_len = strlen(IP_data.data) + 20;
	MyIP.iphdr->ih_version = 4;
	strncpy_s(MyIP.data, strlen(IP_data.data) - offset * 8, IP_data.data, strlen(IP_data.data) - offset * 8);
	(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOLINK, wparam, lparam);

	return 0;
}

/**
* @author ACM2012
* @param [in] 无
* @return 返回FALSE说明没有发送成功，返回TRUE说明发送成功
* @note 函数功能主要是链路层发送过来的信息分解成Msg结构, 发送给运输层
*/
Bool CMyIP::IP2Trans(WPARAM wparam, LPARAM lparam)
{
	///< 根据链路层发送的数据进行剥离得到报文长度以及偏移, 比较偏移量是否等于报文长度
	///< 若发现分片缺失或者检验和出错则 return FALSE;
	///< 若是则数据成功接收 进行少量的检验和检查, 若没有错误
	///< 则将IP_msg结构剥离出Msg结构
	int offset = 0, ident = 0;
	ident++;
	while (MyIP.iphdr->ih_len - offset > 0)
	{
		if (MyIP.iphdr->ih_ident == ident)
		{
			if (MyIP.iphdr->ih_offset * 8 == offset)
			{
				if (MyIP.iphdr->ih_flags){
					strncpy_s(IP_data.data + offset, MAXSIZE, MyIP.data, MAXSIZE);
					offset = offset + MAXSIZE;
					IP_data.sip = MyIP.iphdr->ih_saddr;
					IP_data.dip = MyIP.iphdr->ih_daddr;
					(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOTRANS, wparam, lparam);
				}
				else{
					strncpy_s(IP_data.data + offset, MAXSIZE, MyIP.data, MyIP.iphdr->ih_len - offset);
					offset = MyIP.iphdr->ih_len;
					IP_data.sip = MyIP.iphdr->ih_saddr;
					IP_data.dip = MyIP.iphdr->ih_daddr;
					(AfxGetApp()->m_pMainWnd)->SendMessage(IPTOTRANS, wparam, lparam);
				}
			}
		}
	}

	return 0;
}

