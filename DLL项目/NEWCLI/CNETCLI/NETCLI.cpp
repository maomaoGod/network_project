#include "stdafx.h"
#include "NETCLI.h"

NETCLI::NETCLI(void)
{
	_offset = 0;
}

NETCLI::~NETCLI(void)
{

}

BOOL NETCLI::IP2Link(WPARAM wparam, LPARAM lparam)
{
	struct NETMsg *IP_data = NULL;
	struct NETip_message *MyIP = new NETip_message;
	struct NETIP_Msg *NetoLink = new NETIP_Msg;
	int offset = 0, ident = 0;
	ident++;
	IP_data = (struct NETMsg*)wparam;

	MyIP->ih_version = 4;
	MyIP->ip_hdr_length = 20;
	MyIP->ih_sever = 0;

	MyIP->ih_ident = ident;


	MyIP->ih_TTL = 100;
	MyIP->ih_protl = IP_data->protocol;

	MyIP->ih_saddr = IP_data->sip;
	MyIP->ih_daddr = IP_data->dip;

	NetoLink->ih_saddr = IP_data->sip;
	NetoLink->ih_daddr = IP_data->dip;

	while (IP_data->datelen - 8 * offset > ipMSS)
	{
		MyIP->ih_flags = 1;
		MyIP->ih_offset = offset;
		offset = offset + ipMSS / 8;
		MyIP->ih_data_len = ipMSS + 20;
		memcpy(MyIP->data, IP_data->data, ipMSS);
		MyIP->ip_checksum = iphdrmakesum(MyIP);
		NetoLink->data = (char *)MyIP;
		NetoLink->ih_len = ipMSS + 20;
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

	return 0;
}

/**
* @author ACM2012
* @param [in] wparam��ʾ����㴫���������ݰ��ṹָ��, lparam��ʾ����㴫�����Ĳ���.
* @return ����FALSE˵��û�з��ͳɹ�������TRUE˵�����ͳɹ�
* @note ��������:����Ƕ�ϵͳ����·�㷢�͹�������Ϣ�ֽ��Msg�ṹ, ���͸������
*				 �����·��������·�㷢�͹�������Ϣֱ�ӻ���IP2Link
*/
BOOL NETCLI::IP2Trans(WPARAM wparam, LPARAM lparam)
{
	static int count;
	TCHAR buf[10];
	struct NETip_message *MyIP;

	int ident = 1;
	MyIP = (struct NETip_message*)wparam;
	struct NETMsg *local_IP_data = new NETMsg;
	if (MyIP->ip_checksum != iphdrmakesum(MyIP))
	{
		return 1;
	}
	else if (MyIP->ih_ident == ident)
	{
		if (MyIP->ih_offset * 8 == _offset)
		{
			if (MyIP->ih_flags){
				memcpy(local_IP_data->data + _offset, MyIP->data, ipMSS);
				_offset = _offset + ipMSS;
			}
			else{
				_itot_s(count++, buf, 10);
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

	return 0;
}

/**
* @author ACM2012
* @param [in] ip���ݱ��ײ�
* @return �ײ������
* @note ����������Ҫ�Ǵ�IP����IP���ݱ����ײ������
*/
unsigned short NETCLI::iphdrmakesum(NETip_message *ip)
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

CNETCLI* CreateCliPtr()
{
	return new NETCLI();
}

void DeleteCliPtr(CNETCLI* t)
{
	if (NULL != t)
		delete t;
}