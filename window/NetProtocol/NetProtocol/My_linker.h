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
#define  FRAMESIZE 128
#define table_size 10
#define IP_SIZE sizeof(struct _iphdr)
#define MAC_ADDR_SIZE 3


#pragma pack(1)  //按一个字节内存对齐
#define ETH_ARP         0x0806  //以太网帧类型表示后面数据的类型，对于ARP请求或应答来说，该字段的值为x0806
#define ARP_HARDWARE    1  //硬件类型字段值为表示以太网地址
#define ETH_IP          0x0800  //协议类型字段表示要映射的协议地址类型值为x0800表示IP地址
#define ARP_REQUEST     1   //ARP请求
#define ARP_REPLY       2      //ARP应答
#define HOSTNUM         255   //主机数量

typedef unsigned char Byte;

/**
*@class <CMlinker>
*@brief 链路层数据段帧结构体
*@author ACM2012
*@note
*链路层的发送帧数据部分结构，采用标准以太网帧结构
*/
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
		if (total_seq_num != it.total_seq_num) return  false;
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
*@brief 广播帧帧结构定义
*@author ACM2012
*@note
* 此处是广播帧结构的定义
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

/**
*@class <CMlinker>
*@brief 帧结构定义
*@author ACM2012
*@note
*帧结构的定义，采用以太网标准帧结构
*/
struct EthernetHeader
{
    u_char DestMAC[6];    //目的MAC地址 6字节
    u_char SourMAC[6];   //源MAC地址 6字节
    u_short EthType;         //上一层协议类型，如0x0800代表上一层是IP协议，0x0806为arp  2字节
};

/**
*@class <CMlinker>
*@brief ARP帧结构定义
*@author ACM2012
*@note
*链路层ARP帧结构的定义，采用以太网标准ARP包结构
*/
struct Arpheader {
	unsigned short HardwareType; /**@brief 硬件类型*/ 
	unsigned short ProtocolType; /**@brief 协议类型*/
	unsigned char HardwareAddLen; /**@brief 硬件地址长度*/
	unsigned char ProtocolAddLen; /**@brief 协议地址长度*/
	unsigned short OperationField; /**@brief 操作字段*/
	unsigned char SourceMacAdd[6]; /**@brief 源mac地址*/
	unsigned long SourceIpAdd; /**@brief 源ip地址*/
	unsigned char DestMacAdd[6]; /**@brief 目的mac地址*/
	unsigned long DestIpAdd; /**@brief 目的ip地址*/
};

/**
*@class <CMlinker>
*@brief 链路层类
*@author ACM2012
*@note
*链路层类，其中包含链路层所涉及的各类函数以及各类变量
*/
class my_linker
{
private:
	
	/**
	*@class <CMlinker>
	*@brief 数据片段
	*@author ACM2012
	*@note
	*接收的帧会临时用这个结构体来保存。
	*/
	struct Data_Segment
	{
		Byte data[128];
		unsigned short length;
	};

	static const int maxlength = 100000;
	char **msg;/**@brief 数据报*/ 			 
	Data_Segment *buffer;/**@brief 缓存*/ 
	int bp;/**@brief 指向buffer的指针，表示这一位置可用*/ 
	int **data_pointer;/**@brief 指针，表示每个数据报的每个帧指向的buffer中的位置*/ 
	int *left;/**@brief 每个数据报还剩的帧的个数*/ 
	void get_adapter();/**@brief 获取适配器的方法，私有*/ 

public:

	unsigned short mac_src[3];/**@brief 本机MAC地址*/ 
	unsigned short mac_des[3];/**@brief 目的MAC地址*/ 
	pcap_t * adapterHandle;/**@brief 适配器句柄*/ 
	unsigned int transIP[table_size];
	unsigned short transmac[table_size][3];
	
	/**
	* @author ACM2012
	* @note
	* my_linker类的构造函数
	* @remarks
	*/
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
			AfxMessageBox(_T("创建抓包线程失败!"));
	}
	
	/**
	* @author ACM2012
	* @note
	* my_linker类的析构函数
	* @remarks
	*/
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
	
	/**
	* @author ACM2012
	* @note
	* my_linker类的初始化函数，同构造函数
	* @remarks
	*/
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
	char * combine(const u_char *);/**@brief 接收端通过接收到的帧结构组装网络层数据报的函数*/ 
	int send_by_frame(IP_Msg *, pcap_t *, unsigned short, unsigned short);/**@brief 发送网络层传来的数据报的函数*/ 
	int CSMA_CD_send(IP_Msg *, pcap_t *, unsigned short, unsigned short);/**@brief 发送帧，加上了碰撞检测*/ 
	void GetSelfMac(char*, unsigned short *);/**@brief 获得自身MAC地址的函数*/ 
	int ArpGetMacFromIp(pcap_t *adhandle, const char *ip_addr, unsigned char *ip_mac);

	u_char* BuildArpPacket(unsigned short *, unsigned int, unsigned int);/**@brief 构造ARP包的函数*/ 
	bool check(const u_char *);/**@brief 构造ARP包的函数用于判断接收的帧是否是广播帧*/
	unsigned short crc16(unsigned char *, int);/**@brief 计算CRC检验和的函数*/ 
	unsigned short checkCrc16(unsigned char *, int);/**@brief 通过检验和检验接收的帧是否正确的函数*/ 
	int pppEncode(unsigned char * buf, int len);/**@brief ppp数据帧编码函数*/ 
	int pppDecode(unsigned char * buf, int len);/**@brief ppp数据帧解码函数*/ 
	void send_broadcast(pcap_t  *adapterHandle, unsigned int src_IP, unsigned int dst_IP);/**@brief 发送广播帧*/ 
	bool get_mac(pcap_t  *adapterHandle);/**@brief 获取MAC地址*/ 
	bool transtable(unsigned int IP);
	void packcap();/**@brief 循环接收帧的函数*/ 
	void Link2IP(WPARAM wparam);/**@brief 调用combine合并帧并提交上一层的模块*/ 
	static DWORD WINAPI NewPackThread(LPVOID lParam);/**@brief 一个线程，在构造函数中调用，它将用于执行packcap方法*/
	unsigned int getIP();/**@brief 获取IP地址*/ 
};
