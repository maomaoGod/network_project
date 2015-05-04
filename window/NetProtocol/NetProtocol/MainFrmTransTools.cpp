/** @file
 * @brief udp and tcp checksum
 * @author sunyiwei
 * @date 2015/05/02
 * @version 0.1
 *
 */

#include "stdafx.h"
#include "NetProtocol.h"
#include "MainFrmTransTools.h"
#include <string>

using namespace std;

/** @udpmakesum
 * @brief make the sum of udp datagram
*
 */
u16 udpmakesum(u16 len_udp, u16 src_port,u16 dest_port, bool padding, u16 *buff)
{
    u16 prot_udp=17;
    u16 padd=0;
    u16 word16;
    u32 sum;
    int i;
    ///< Find out if the length of data is even or odd number. If odd,
    ///< add a padding byte = 0 at the end of packet
    if ((padding&1)==1){
        padd=1;
        buff[len_udp]=0;
    }
    ///<initialize sum to zero
    sum=0;
    ///< make 16 bit words out of every two adjacent 8 bit words and
    ///< calculate the sum of all 16 bit words
    for (i=0; i<len_udp+padd; i=i+2){
        word16 = ((buff[i]<<8)&0xFF00)+(buff[i+1]&0xFF);
        sum = sum + (unsigned long)word16;
    }
    ///< add the UDP pseudo header which contains the IP source and destinationn addresses
    word16 = ((src_port<<8)&0xFF00)+(dest_port&0xFF);
    sum = sum+word16;

    ///< the protocol number and the length of the UDP packet
    sum = sum + prot_udp + len_udp;

    ///< keep only the last 16 bits of the 32 bit calculated sum and add the carries
        while (sum>>16)
        {
        sum = (sum & 0xFFFF)+(sum >> 16);
        }
    ///< Take the one's complement of sum
    sum = ~sum;
    return ((u16) sum);
}

/** @tcpmakesum
 * @brief make the sum of tcp datagram
*
 */
u16 tcpmakesum(u16 len_tcp, u16 src_port,u16 dest_port, bool padding, u16 *buff)
{
    u16 prot_tcp=6;
    u16 padd=0;
    u16 word16;
    u32 sum;
    int i;
    ///< Find out if the length of data is even or odd number. If odd,
    ///< add a padding byte = 0 at the end of packet
    if (padding&1==1){
        padd=1;
        buff[len_tcp]=0;
    }
    ///<initialize sum to zero
    sum=0;
    ///< make 16 bit words out of every two adjacent 8 bit words and
    ///< calculate the sum of all 16 vit words
    for (i=0;i<len_tcp+padd;i=i+2){
        word16 =((buff[i]<<8)&0xFF00)+(buff[i+1]&0xFF);
        sum = sum + (unsigned long)word16;
    }
    ///< add the TCP pseudo header which contains:
    ///< the IP source and destinationn addresses,
    word16 = ((src_port<<8)&0xFF00)+(dest_port&0xFF);
    sum = sum+word16;

    ///< the protocol number and the length of the TCP packet
    sum = sum + prot_tcp + len_tcp;

    ///< keep only the last 16 bits of the 32 bit calculated sum and add the carries
        while (sum>>16)
        sum = (sum & 0xFFFF)+(sum >> 16);

    ///< Take the one's complement of sum
    sum = ~sum;
    return ((u16) sum);
}

/** @udpcheck
 * @brief check the ckecksum of udp
*
 */
bool udpcheck(u16 len_udp, u16 src_port,u16 dest_port, bool padding, u16 *buff, u16 checksum)
 {
    u16 sum;///<sum of data and checksum
    sum = udpmakesum(len_udp, src_port, dest_port, padding, buff)+checksum;
    if (sum = 0xffff)
        return 1;
    else
        return 0;
 }

/** @tcpcheck
 * @brief check the ckecksum of tcp
*
 */
bool tcpcheck(u16 len_tcp, u16 src_port,u16 dest_port, bool padding, u16 *buff, u16 checksum)
 {
    u16 sum;///<sum of data and checksum
    sum = tcpmakesum(len_tcp, src_port, dest_port, padding, buff)+checksum;
    if (sum = 0xffff)
        return 1;
    else
        return 0;
 }


unsigned int getIP()
{
	WSADATA wsaData;
	char name[155];
	char *ip;
	PHOSTENT hostinfo; 
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)
	{ 
		if(gethostname (name, sizeof(name)) == 0)
		{ 
			if((hostinfo = gethostbyname(name)) != NULL) 
			{
				// 这些就是获得IP的函数
				ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			} 
		} 
		WSACleanup();
	}
	// 将ip从字符串转为unsigned int
	unsigned int ip_number = 0;
	unsigned int ip_seg_number = 0;
	for (int i = 0; i < 15 && ip[i]; ++i)
	{
		if (ip[i] == '.')
		{
			ip_number += ip_seg_number;
		}
		else
		{
			ip_seg_number = ip_seg_number*10+ip[i]-48;
		}
	}
	ip_number += ip_seg_number;
	return ip_number;
}

unsigned int IP_string2uint(string ip)
{
	// 将ip从字符串转为unsigned int
	unsigned int ip_number = 0;
	unsigned int ip_seg_number = 0;
	for (int i = 0; i < ip.length(); ++i)
	{
		if (ip[i] == '.')
		{
			ip_number += ip_seg_number;
		}
		else
		{
			ip_seg_number = ip_seg_number*10+ip[i]-48;
		}
	}
	ip_number += ip_seg_number;
	return ip_number;
}

unsigned int port_string2uint(string port)
{
	unsigned int port_number = 0;
	for (int i = 0; i < port.length(); ++i)
	{
		port_number = port_number*10+port[i]-48;
	}
	return port_number;
}