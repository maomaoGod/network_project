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
*@class UDP���Ķνṹ
*@brief ���UDP���ĶεĽṹ��
*@author ACM1201
*@note
*UDP���Ķλ����ṹ
*/
struct udp_message
{
	unsigned udp_src_port : 16;	///< Դ�˿ں�
	unsigned udp_dst_port : 16;	///< Ŀ�Ķ˿ں�
	unsigned udp_msg_length : 16;	///< ���ĳ���
	unsigned udp_checksum : 16;	///< �����
	CString udp_app_data;	///< ����
};

/**
*@class TCP���Ķνṹ
*@brief ���TCP���ĶεĽṹ��
*@author ACM1201
*@note
*TCP���Ķλ����ṹ
*/
struct tcp_message
{
	unsigned tcp_src_port : 16;	///< Դ�˿ں�
	unsigned tcp_dst_port : 16;	///< Ŀ�Ķ˿ں�
	unsigned tcp_seq_number : 32;	///< ���
	unsigned tcp_ack_number : 32;	///< ȷ�Ϻţ����ڴ�����һ����ţ�
	unsigned tcp_hdr_length : 4;	///< �ײ����ȣ�ͨ��Ϊ20Byte��
	unsigned tcp_reserved : 6;	///< �����ֶΣ���δʹ�ã�
	unsigned tcp_urg : 1;	///< ������־����δʹ�ã�
	unsigned tcp_ack : 1;	///< ȷ�ϱ�־
	unsigned tcp_psh : 1;	///< �����Ͻ���־����δʹ�ã�
	unsigned tcp_rst : 1;	///< �������ӽ�������
	unsigned tcp_syn : 1;	///< �������ӽ�������
	unsigned tcp_fin : 1;	///< �������ӽ�������
	unsigned tcp_rcv_window : 16;	///< ���մ���
	unsigned tcp_checksum : 16;	///< �����
	unsigned tcp_urg_ptr : 16;	///< ��������ָ�루��δʹ�ã�
	CString tcp_opts_and_app_data;	///< ѡ���ֶΣ�ͨ��û�У���ͨ���ײ�����Ϊ20Byte��������
};

/**
*@class TCP���ķ���״̬�ṹ��
*@brief ���TCP���Ķεķ���״̬�Ľṹ��
*@author ACM1201
*@note
*TCP���Ķη���״̬�����ṹ
*/
struct tcp_state
{
	unsigned int tcp_ack_counter;	///< TCP���Ķη��ͺ�ack���ĵļ���
	unsigned int tcp_rcv_window;	///< TCP���ĶεĽ��ܴ���
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

unsigned int IP_string2uint(string ip);

unsigned int port_string2uint(string port);