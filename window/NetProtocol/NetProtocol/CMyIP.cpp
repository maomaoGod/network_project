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
	IP_HEADER.ih_len = strlen(IP_data->data);
	IP_HEADER.ih_version = 4;
	IP_HEADER.ih_sport = IP_data->ih_sport;
	IP_HEADER.ih_dport = IP_data->ih_dport;
	MyIP->iphdr = &IP_HEADER;
	memcpy(MyIP->data, IP_data->data, IP_data->datelen - 8 * offset);
	(AfxGetApp()->m_pMainWnd)->SendMessage(LINKSEND, (WPARAM)MyIP, lparam);
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
	int ident = 1;
	MyIP = (struct IP_Msg*)wparam;

	if (MyIP->iphdr->ih_ident == ident)
	{
		if (MyIP->iphdr->ih_offset * 8 == _offset)
		{
			if (MyIP->iphdr->ih_flags){
				memcpy(IP_data->data + _offset, MyIP->data, MAXSIZE);
				_offset = _offset + MAXSIZE;
			}
			else{
				memcpy(IP_data->data + _offset, MyIP->data, MyIP->iphdr->ih_len - _offset);
				_offset = 0;
				IP_data->sip = MyIP->iphdr->ih_saddr;
				IP_data->dip = MyIP->iphdr->ih_daddr;
				IP_data->ih_sport = MyIP->iphdr->ih_sport;
				IP_data->ih_dport = MyIP->iphdr->ih_dport;
				IP_data->protocol = MyIP->iphdr->ih_protl;
				IP_data->datelen = MyIP->iphdr->ih_len;
				(AfxGetApp()->m_pMainWnd)->SendMessage(TRANSTOAPP, (WPARAM)IP_data, lparam);
			}
		}
	}

	return 0;
}