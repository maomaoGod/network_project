/** @file
 * @brief udp tcp checksum
 * @author ACM1201
 * @date 2015/05/02
 * @version 0.1
 *
 */
#pragma once
#include "stdafx.h"
#include "winsock2.h"
#include <string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

#define MSS 1024

/**
*@class UDP报文段结构
*@brief 存放UDP报文段的结构体
*@author ACM1201
*@note
*UDP报文段基本结构
*/
struct udp_message
{
	unsigned udp_src_port : 16;	///< 源端口号
	unsigned udp_dst_port : 16;	///< 目的端口号
	unsigned udp_msg_length : 16;	///< 报文长度
	unsigned udp_checksum : 16;	///< 检验和
	char udp_app_data[MSS];	///< 数据
};

/**
*@class TCP报文段结构
*@brief 存放TCP报文段的结构体
*@author ACM1201
*@note
*TCP报文段基本结构
*/
struct tcp_message
{
	unsigned tcp_src_port : 16;	///< 源端口号
	unsigned tcp_dst_port : 16;	///< 目的端口号
	unsigned tcp_seq_number : 32;	///< 序号
	unsigned tcp_ack_number : 32;	///< 确认号（所期待的下一个序号）
	unsigned tcp_hdr_length : 4;	///< 首部长度（通常为20Byte）
	unsigned tcp_reserved : 6;	///< 保留字段（并未使用）
	unsigned tcp_urg : 1;	///< 紧急标志（并未使用）
	unsigned tcp_ack : 1;	///< 确认标志
	unsigned tcp_psh : 1;	///< 立即上交标志（并未使用）
	unsigned tcp_rst : 1;	///< 用于连接建立与拆除（并未使用）
	unsigned tcp_syn : 1;	///< 用于连接建立与拆除
	unsigned tcp_fin : 1;	///< 用于连接建立与拆除
	unsigned tcp_rcv_window : 16;	///< 接收窗口
	unsigned tcp_checksum : 16;	///< 检验和
	unsigned tcp_urg_ptr : 16;	///< 紧急数据指针（并未使用）
	char tcp_opts_and_app_data[MSS];	///< 选项字段（通常没有，故通常首部长度为20Byte）和数据
};

/**
*u16 buff[] is an array containing all the octets in the UDP header and data
*u16 len_udp is the length (number of octets) of the UDP header and data
*padding is 1 if data has an even number of octets and 0 for an odd number
*u16 src_port[4] and u16 dest_port[4] are the IP source and destination address octets
 */
typedef unsigned short u16;
typedef unsigned long u32;

u16 udpmakesum(u16 len_udp, u16 src_port,u16 dest_port, bool padding, u16 *buff);

u16 tcpmakesum(u16 len_tcp, u16 src_port,u16 dest_port, bool padding, u16 *buff);

bool udpcheck(u16 len_udp, u16 src_port,u16 dest_port, bool padding, u16 *buff, u16 checksum);

bool tcpcheck(u16 len_udp, u16 src_port,u16 dest_port, bool padding, u16 *buff, u16 checksum);

unsigned int getIP();

unsigned int IP_chars2uint(char ip[]);

void IP_uint2chars(char ip[], unsigned int ip_int);

unsigned int IP_string2uint(string ip);

unsigned int port_string2uint(string port);