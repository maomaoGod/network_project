/** @file
 * @brief udp and tcp checksum
 * @author ACM1201
 * @date 2015/05/02
 * @version 0.1
 *
 */
#pragma once
#include "stdafx.h"
#include "TransLayerTools.h"

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
    sum = 0;
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
    if ((padding&1)==1){
        padd=1;
        buff[len_tcp]=0;
    }
    ///<initialize sum to zero
    sum = 0;
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
    sum = udpmakesum(len_udp, src_port, dest_port, padding, buff)-checksum;
    if (sum == 0x0000)
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
    sum = tcpmakesum(len_tcp, src_port, dest_port, padding, buff)-checksum;
    if (sum == 0x0000)
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
			ip_number = ip_number * 256 + ip_seg_number;
			ip_seg_number = 0;
		}
		else
		{
			ip_seg_number = ip_seg_number*10+ip[i]-48;
		}
	}
	ip_number = ip_number * 256 + ip_seg_number;
	return ip_number;
}

unsigned int IP_chars2uint(char ip[])
{
	// 将ip从字符串转为unsigned int
	unsigned int ip_number = 0;
	unsigned int ip_seg_number = 0;
	for (int i = 0; i < 15 && ip[i]; ++i)
	{
		if (ip[i] == '.')
		{
			ip_number = ip_number * 256 + ip_seg_number;
			ip_seg_number = 0;
		}
		else
		{
			ip_seg_number = ip_seg_number * 10 + ip[i] - 48;
		}
	}
	ip_number = ip_number * 256 + ip_seg_number;
	return ip_number;
}

void IP_uint2chars(char ip[], unsigned int ip_int)
{
	int ptr = 0;
	unsigned int segment;
	char temp[20];	// ipv4
	for (int i = 0; i < 4; ++i)	// ipv4
	{
		segment = ip_int%256;
		ip_int /= 256;
		if (segment)
		{
			while (segment)
			{
				temp[ptr++] = segment%10+48;
				segment /= 10;
			}
		}
		else
		{
			temp[ptr++] = '0';
		}

		if (i != 3)
		{
			temp[ptr++] = '.';
		}
	}

	for (int i = 0; i < ptr; ++i)
	{
		ip[i] = temp[ptr-i-1];
	}
	ip[ptr] = '\0';
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
			ip_number = ip_number*256+ip_seg_number;
		}
		else
		{
			ip_seg_number = ip_seg_number*10+ip[i]-48;
		}
	}
	ip_number = ip_number*256+ip_seg_number;
	return ip_number;
}

unsigned int port_string2uint(string port)
{
	unsigned int port_number = 0;
	for (unsigned int i = 0; i < port.length(); ++i)
	{
		port_number = port_number*10+port[i]-48;
	}
	return port_number;
}

// 超时重发
//void post()
//{
//    if(get_data == 1/*从上层收到数据*/)
//    {
//        //检查下一个可用于该分组的序号;
//        if(next_no >= pwndl && next_no <= pwndr/*该序号在发送方窗口内*/)
//        {
//            _send();//打包数据并发送;
//        }
//        else
//        {
//            _cache();//将数据缓存;
//            _return();//将其返回给上层;
//        }
//    }
//    if(timeout == 1/*超时*/)
//    {
//        _resend();//重发超时分组;
//    }
//    if(get_ack == 1 && ack_no >= pwndl && ack_no <= pwndr/*收到ACK && 该分组序号在窗口内*/)
//    {
//        _mark();//发送方将该被确认的分组标记为已接收;
//        if(ack_no == send_base/*该分组的序号 == send_base*/)
//        {
//            //将窗口基序号向前移动到具有最小序号的未确认分组处;
//            do
//            {
//                send_base = ack_no;
//                ack--;
//            }
//            while(ack_no 未确认)
//        }
//        if(窗口移动了 && 有序号落在窗口内的未发送分组)
//        {
//            _send();////发送这些分组;
//        }
//    }
//}
//
//void get()
//{
//    if(rvc_flag == 1 && no >=rcv_base && no <= rcv_base+N-1/*分组被正确接收 && 序号 >= rcv_base && 序号 <= rcv_base+N-1*/)
//    {
//        _resend();//回送一个选择ACK分组给发送方;
//        if(check_flag == 1/*该分组之前未收到*/)
//        {
//            _cache();//缓存该分组;
//        }
//        if(no == rvc_base/*该分组序号 == 接收窗口的基序号*/)
//        {
//            将该分组以及之前缓存的序号连续的分组交付给上层;
//            接收窗口按向上交付的分组的数量向前移动;
//        }
//    }
//    if(rvc_flag == 1 && no >=rcv_base - N && no <= rcv_base-1/*分组被正确收到 && 序号 >= rcv_base - N && 序号 <= rcv_base-1*/)
//    {
//        _resend();//回送一个ACK;
//    }
//}

// 握手
//int ShakeHands()
//{
//    client_isn = random();  //client
//    serve_isn = random(); //serve
//    connect1();
//    connect2();
//    connect3();
//    disconnect1();
//    disconnect2();
//    disconnect3();
//    disconnect4();
//    //wait 30s
//    close();
//    return 0;
//}
//
//void connect1()    //client to serve 生成请求连接报文段
//{
//
//    SYN = 1;
//    no = client_isn;
//
//}
//
//void connect2()   //serve to client 生成允许连接报文段
//{
//    //分配TCP缓存和变量
//    SYN = 1;
//    confirm_no = client_isn + 1;
//    no = serve_isn;
//
//}
//
//void connect3()  //client to serve 生成确认允许连接报文段
//{
//     //分配TCP缓存和变量
//     SYN = 0;
//     confirm_no = serve_isn + 1;
//     no = client_isn + 1;
//}
//
//void disconnect1()  //client to serve 生成终止报文段
//{
//     FIN = 1;
//}
//
//void disconnect2()  //serve to client 确认
//{
//    //生成ACK
//}
//
//void disconnect3()  //serve to client 生成终止报文段
//{
//    FIN = 1;
//}
//
//void disconnect4()  //client to serve 确认
//{
//    //生成ACK
//}
//
//void close()
//{
//    //断开连接
//}
//
//int random()
//{
//    int rand;
//    //生成一个随机数
//    return rand;
//}

// 拥塞控制
//int ctrl()
//{
//    if(ack_flag == 1)
//    {
//        count ++;
//    }
//    else count = 0;
//    if(cwnd <= Threshold) //慢启动
//    {
//        slow_start();
//    }
//    else
//    {
//        control();
//    }
//    return 0;
//}
//
//void slow_start()
//{
//    cwnd += MSS;
//}
//
//void control()
//{
//    if(timenew - timeold > RTT)
//    {
//        Threshold = cwnd/2;
//        cwnd = MSS;
//    }
//    else if(count >= 3)
//    {
//        Threshold = cwnd/2;
//        cwnd =  Threshold;
//    }
//    else
//    {
//        cwnd = cwnd + MSS*(MSS/cwnd);
//    }
//}