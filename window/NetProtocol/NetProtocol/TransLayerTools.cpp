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
				// ��Щ���ǻ��IP�ĺ���
				ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			} 
		} 
		WSACleanup();
	}
	// ��ip���ַ���תΪunsigned int
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
	// ��ip���ַ���תΪunsigned int
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
	// ��ip���ַ���תΪunsigned int
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

// ��ʱ�ط�
//void post()
//{
//    if(get_data == 1/*���ϲ��յ�����*/)
//    {
//        //�����һ�������ڸ÷�������;
//        if(next_no >= pwndl && next_no <= pwndr/*������ڷ��ͷ�������*/)
//        {
//            _send();//������ݲ�����;
//        }
//        else
//        {
//            _cache();//�����ݻ���;
//            _return();//���䷵�ظ��ϲ�;
//        }
//    }
//    if(timeout == 1/*��ʱ*/)
//    {
//        _resend();//�ط���ʱ����;
//    }
//    if(get_ack == 1 && ack_no >= pwndl && ack_no <= pwndr/*�յ�ACK && �÷�������ڴ�����*/)
//    {
//        _mark();//���ͷ����ñ�ȷ�ϵķ�����Ϊ�ѽ���;
//        if(ack_no == send_base/*�÷������� == send_base*/)
//        {
//            //�����ڻ������ǰ�ƶ���������С��ŵ�δȷ�Ϸ��鴦;
//            do
//            {
//                send_base = ack_no;
//                ack--;
//            }
//            while(ack_no δȷ��)
//        }
//        if(�����ƶ��� && ��������ڴ����ڵ�δ���ͷ���)
//        {
//            _send();////������Щ����;
//        }
//    }
//}
//
//void get()
//{
//    if(rvc_flag == 1 && no >=rcv_base && no <= rcv_base+N-1/*���鱻��ȷ���� && ��� >= rcv_base && ��� <= rcv_base+N-1*/)
//    {
//        _resend();//����һ��ѡ��ACK��������ͷ�;
//        if(check_flag == 1/*�÷���֮ǰδ�յ�*/)
//        {
//            _cache();//����÷���;
//        }
//        if(no == rvc_base/*�÷������ == ���մ��ڵĻ����*/)
//        {
//            ���÷����Լ�֮ǰ�������������ķ��齻�����ϲ�;
//            ���մ��ڰ����Ͻ����ķ����������ǰ�ƶ�;
//        }
//    }
//    if(rvc_flag == 1 && no >=rcv_base - N && no <= rcv_base-1/*���鱻��ȷ�յ� && ��� >= rcv_base - N && ��� <= rcv_base-1*/)
//    {
//        _resend();//����һ��ACK;
//    }
//}

// ����
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
//void connect1()    //client to serve �����������ӱ��Ķ�
//{
//
//    SYN = 1;
//    no = client_isn;
//
//}
//
//void connect2()   //serve to client �����������ӱ��Ķ�
//{
//    //����TCP����ͱ���
//    SYN = 1;
//    confirm_no = client_isn + 1;
//    no = serve_isn;
//
//}
//
//void connect3()  //client to serve ����ȷ���������ӱ��Ķ�
//{
//     //����TCP����ͱ���
//     SYN = 0;
//     confirm_no = serve_isn + 1;
//     no = client_isn + 1;
//}
//
//void disconnect1()  //client to serve ������ֹ���Ķ�
//{
//     FIN = 1;
//}
//
//void disconnect2()  //serve to client ȷ��
//{
//    //����ACK
//}
//
//void disconnect3()  //serve to client ������ֹ���Ķ�
//{
//    FIN = 1;
//}
//
//void disconnect4()  //client to serve ȷ��
//{
//    //����ACK
//}
//
//void close()
//{
//    //�Ͽ�����
//}
//
//int random()
//{
//    int rand;
//    //����һ�������
//    return rand;
//}

// ӵ������
//int ctrl()
//{
//    if(ack_flag == 1)
//    {
//        count ++;
//    }
//    else count = 0;
//    if(cwnd <= Threshold) //������
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