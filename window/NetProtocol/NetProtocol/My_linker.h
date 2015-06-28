/**
*@file
*@brief 链路层协议
*@author ACM2012
*@date 2015/06/06
*@version 1.1
*@note
* 实现链路层的基本功能，包括以太网的碰撞协议，arp协议，arp缓存栈，发送接收模块，ppp协议等。同时实现了多种链路层容错检验算法。
*/
#pragma once
#include "pcap.h"
#include "NetProtocol.h"
#include "conio.h" 
#include "packet32.h" 
#include "ntddndis.h"
#define FRAMESIZE 128
#define table_size 10
#define IP_SIZE sizeof(struct _iphdr)
#define MAC_ADDR_SIZE 3
typedef unsigned char Byte;

struct Frame{
	unsigned short MAC_des[MAC_ADDR_SIZE];           /**@brief MAC_dst MAC目标地址*/ 
	unsigned short MAC_src[MAC_ADDR_SIZE];           /**@brief MAC_src MAC源地址*/ 
	unsigned short total_seq_num;                    /**@brief 帧的总个数*/
	unsigned short datagram_num;                     /**@brief 数据报序号*/
	unsigned short seq;                              /**@brief 帧序号 */
	unsigned short length;                           /**@brief 当前帧数据的长度 */
	Byte data[FRAMESIZE];                            /**@brief 网络层数据 */
	unsigned short CRC;                              /**@brief CRC16生成结果 */
	
	/**
	*@brief 链路层帧验证
	*@author ACM2012
	*@note
	* 用户输入一个帧，由于这两个帧是结构体，进行比较时不能直接比较，
	* 这里重定义了等号表示，来进行帧结构是否相等的比较
	* 
	*/
	bool operator == (const Frame &it) const         
	{
		for (int i = 0; i < 3; ++i)
		{
			if (MAC_des[i] != it.MAC_des[i]) return false;
			if (MAC_src[i] != it.MAC_src[i]) return false;
		}
		if (total_seq_num != it.total_seq_num) return false;
		if (datagram_num != it.datagram_num) return false;
		if (seq != it.seq) return false;
		if (length != it.length) return false;
		for (int i = 0; i < length;++i)
			if (data[i] != it.data[i]) return false;
		return true;
	}
	
	/**
	*@brief 链路层帧不等验证
	*@author ACM2012
	*@note
	* 用户输入一个帧，由于这两个帧是结构体，进行比较时不能直接比较，
	* 这里重定义了等号表示，来进行帧结构是否不相等的比较
	* 
	*/
	bool operator != (const Frame &it) const
	{
		return !((*this) == it);
	}
};

/**
*@brief 帧结构定义
*@author ACM2012
*@note
* 此处是帧结构的帧头的定义
*/
struct Broadcast_frame
{
	unsigned short MAC_des[3];           /**@brief MAC_dst MAC目标地址*/
	unsigned short MAC_src[3];           /**@brief MAC_src MAC源地址*/
	unsigned short type;                /**@brief 类型定义 */
	unsigned int IP_dst;				 /**@brief 目标IP地址*/
	unsigned int IP_src;				 /**@brief 源IP地址*/
	unsigned short CRC;
};

class my_linker
{
private:

	struct Data_Segment
	{
		Byte data[128];
		unsigned short length;
	};

	static const int maxlength = 100000;
	char **msg;						 //数据报
	Data_Segment *buffer;
	int bp;
	int **data_pointer;
	int *left;						 //每个数据报还剩多少帧
	void get_adapter();

public:

	//本机MAC地址
	unsigned short mac_src[3];

	//目的MAC地址
	unsigned short mac_des[3];
	pcap_t * adapterHandle;
	unsigned int transIP[table_size];
	unsigned short transmac[table_size][3];
	my_linker()
	{
		msg = new char*[maxlength];
		buffer = new Data_Segment[maxlength];
		bp = 0;
		data_pointer = new int*[maxlength];
		left = new int[maxlength];
		for (int i = 0; i < maxlength; ++i)
		{
			data_pointer[i] = NULL;
			msg[i] = NULL;
		}
		get_adapter();
		if (!CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewPackThread, (LPVOID) this, NULL, NULL))
			AfxMessageBox(_T("创建抓包线程失败！"));
	}
	~my_linker()
	{
		for (int i = 0; i < maxlength; ++i)
			if (msg[i]!=NULL)
			{
				delete[] msg[i];
			}
		delete[] msg;
		delete[] buffer;
		for (int i = 0; i < maxlength; ++i)
			delete[] data_pointer[i];
		delete[] data_pointer;
		delete[] left;
	}
	inline void initialize()
	{
		msg = new char*[maxlength];
		buffer = new Data_Segment[maxlength];
		bp = 0;
		data_pointer = new int*[maxlength];
		left = new int[maxlength];
		for (int i = 0; i < maxlength; ++i)
		{
			data_pointer[i] = NULL;
			msg[i] = NULL;
		}
		get_adapter();
	}
	char * combine(const u_char *);
	int send_by_frame(IP_Msg *, pcap_t *, unsigned short, unsigned short);
	void GetSelfMac(char*, unsigned short *);
	u_char* BuildArpPacket(unsigned short *, unsigned int, unsigned int);
	bool check(const u_char *);
	unsigned short crc16(unsigned char *, int);
	unsigned short checkCrc16(unsigned char *, int);
	int pppEncode(unsigned char * buf, int len);
	int pppDecode(unsigned char * buf, int len);
	void send_broadcast(pcap_t  *adapterHandle, unsigned int src_IP, unsigned int dst_IP);
	bool get_mac(pcap_t  *adapterHandle);
	bool transtable(unsigned int IP);
	void packcap();
	void Link2IP(WPARAM wparam);
	static DWORD WINAPI NewPackThread(LPVOID lParam);
	unsigned int getIP();
};
