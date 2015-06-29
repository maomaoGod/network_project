/**@file
*@brief
*@author ACM2012
*@date 2015/4/18
*@version <0.1>
*@note
*声明MyIP类的头文件
*/
#pragma once
#include "NetProtocol.h"
#include "CRouting.h"
#include "CNAT.h"


/**
*@class  <CMyIP>
*@brief  基于IP协议与客户端实现可靠传输与不可靠传输的类
*@author ACM2012
*@note
*基于IP协议实现选路与转发, 选路算法包括链路状态选路算法和距离向量选路算法, 通过选路算法构造转发表
*/
class CMyIP
{
public:
	Msg *IP_data;		  ///< 定义IP报文数据
	ip_message *MyIP;     ///< 定义IP数据报格式
	IP_Msg *NetoLink;     ///< 定义IP报文
	int _offset;		  ///< 定义偏移量
	Route_info *info;     ///< 路由信息结构
	LS_data *LsData;	  ///< LS算法的输入结构
	DV_data *DvData;      ///< DV算法的输入结构
	unsigned int sip;    
	unsigned int dip;

public:
	CRouting Route;       ///< 调用路由选择协议
	CNAT     InToOut;     ///< 调用NAT协议

public:
	/** @brief CMyIP类的构造函数 */
	CMyIP();
	/** @brief CMyIP类的析构函数 */
	~CMyIP();
	/** @brief 从网络层传送数据到传输层的函数 */
	Bool IP2Trans(WPARAM wparam, LPARAM lparam);
	/** @brief 从网络层传送数据到链路层的函数 */
	BOOL IP2Link(WPARAM wparam, LPARAM lparam);
	/** @brief 从链路层得到用于选路的信息 */
	BOOL RecvMsg(WPARAM wparam, LPARAM lparam);
	/** @brief 用于发送用于选路的信息 */
	BOOL SendMsg(WPARAM wparam, LPARAM lparam);
	/** @brief 从数组的下标得到对应的IP */
	unsigned int Num2IP(int num);
	/** @brief 从IP得到对应的数组下标 */
	BOOL IP2Num(unsigned int IP);
	/** @brief 计算IP数据报的首部检验和 */
	unsigned short iphdrmakesum(ip_message *ip);
	/** @brief 从路由信息结构中提取到LS_data信息结构 */
	void Route2LS(Route_info *info, LS_data *LsData);
	/** @brief 从路由信息结构中提取到DV_data信息结构 */
	void Route2DV(Route_info *info, DV_data *DvData);
};

