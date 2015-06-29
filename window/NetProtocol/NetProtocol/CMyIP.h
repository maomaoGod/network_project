/**@file
*@brief
*@author ACM2012
*@date 2015/4/18
*@version <0.1>
*@note
*����MyIP���ͷ�ļ�
*/
#pragma once
#include "NetProtocol.h"
#include "CRouting.h"
#include "CNAT.h"


/**
*@class  <CMyIP>
*@brief  ����IPЭ����ͻ���ʵ�ֿɿ������벻�ɿ��������
*@author ACM2012
*@note
*����IPЭ��ʵ��ѡ·��ת��, ѡ·�㷨������·״̬ѡ·�㷨�;�������ѡ·�㷨, ͨ��ѡ·�㷨����ת����
*/
class CMyIP
{
public:
	Msg *IP_data;		  ///< ����IP��������
	ip_message *MyIP;     ///< ����IP���ݱ���ʽ
	IP_Msg *NetoLink;     ///< ����IP����
	int _offset;		  ///< ����ƫ����
	Route_info *info;     ///< ·����Ϣ�ṹ
	LS_data *LsData;	  ///< LS�㷨������ṹ
	DV_data *DvData;      ///< DV�㷨������ṹ
	unsigned int sip;    
	unsigned int dip;

public:
	CRouting Route;       ///< ����·��ѡ��Э��
	CNAT     InToOut;     ///< ����NATЭ��

public:
	/** @brief CMyIP��Ĺ��캯�� */
	CMyIP();
	/** @brief CMyIP����������� */
	~CMyIP();
	/** @brief ������㴫�����ݵ������ĺ��� */
	Bool IP2Trans(WPARAM wparam, LPARAM lparam);
	/** @brief ������㴫�����ݵ���·��ĺ��� */
	BOOL IP2Link(WPARAM wparam, LPARAM lparam);
	/** @brief ����·��õ�����ѡ·����Ϣ */
	BOOL RecvMsg(WPARAM wparam, LPARAM lparam);
	/** @brief ���ڷ�������ѡ·����Ϣ */
	BOOL SendMsg(WPARAM wparam, LPARAM lparam);
	/** @brief ��������±�õ���Ӧ��IP */
	unsigned int Num2IP(int num);
	/** @brief ��IP�õ���Ӧ�������±� */
	BOOL IP2Num(unsigned int IP);
	/** @brief ����IP���ݱ����ײ������ */
	unsigned short iphdrmakesum(ip_message *ip);
	/** @brief ��·����Ϣ�ṹ����ȡ��LS_data��Ϣ�ṹ */
	void Route2LS(Route_info *info, LS_data *LsData);
	/** @brief ��·����Ϣ�ṹ����ȡ��DV_data��Ϣ�ṹ */
	void Route2DV(Route_info *info, DV_data *DvData);
};

