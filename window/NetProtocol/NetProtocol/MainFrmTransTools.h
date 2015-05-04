/** @file
 * @brief udp tcp checksum
 * @author sunyiwei
 * @date 2015/05/02
 * @version 0.1
 *
 */

#include "stdafx.h"
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")

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

unsigned int IP_string2uint(string ip);

unsigned int port_string2uint(string port);