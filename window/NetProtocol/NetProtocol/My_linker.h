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

struct Frame_data{
	struct _iphdr IP;                  // ����㴫����IP
	Byte data[FRAMESIZE];                    // ���������
	bool operator == (const Frame_data &it) const
	{
		if (IP != it.IP) return false;
		for (int i = 0; i < FRAMESIZE; ++i)
		if (data[i] != it.data[i])
			return false;
		return true;
	}
	bool operator != (const  Frame_data &it) const
	{
		return !((*this) == it);
	}
};

struct Frame{
	unsigned short MAC_des[MAC_ADDR_SIZE];           // MAC_dst MACĿ���ַ
	unsigned short MAC_src[MAC_ADDR_SIZE];           // MAC_src MACԴ��ַ
	unsigned short total_seq_num;        // ֡���ܸ���
	unsigned short datagram_num;         // ���ݱ����
	unsigned short seq;                  // ֡���
	unsigned short length;               // ��ǰ֡���ݵĳ���
	struct Frame_data frame_data;        // ֡���ݽṹ
	unsigned short CRC;
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
		if (frame_data != it.frame_data) return false;
		return true;
	}
	bool operator != (const Frame &it) const
	{
		return !((*this) == it);
	}
};

struct Broadcast_frame
{
	unsigned short MAC_des[3];           // MAC_dst MACĿ���ַ
	unsigned short MAC_src[3];           // MAC_src MACԴ��ַ
	unsigned short type;
	unsigned int IP_dst;				 // Ŀ��IP��ַ
	unsigned int IP_src;				 // ԴIP��ַ
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
	IP_Msg *ip_msg;
	Data_Segment *buffer;
	int bp;
	int **data_pointer;
	int *left;					 //ÿ�����ݱ���ʣ����֡

public:

	//����MAC��ַ
	unsigned short mac_src[3];

	//Ŀ��MAC��ַ
	unsigned short mac_des[3];
	static pcap_t * adapterHandle;
	unsigned int transIP[table_size];
	unsigned short transmac[table_size][3];
	my_linker()
	{
		ip_msg = new IP_Msg[maxlength];
		buffer = new Data_Segment[maxlength];
		bp = 0;
		data_pointer = new int*[maxlength];
		left = new int[maxlength];
		for (int i = 0; i < maxlength; ++i)
		{
			data_pointer[i] = NULL;
			ip_msg->iphdr = NULL;
		}
	}
	~my_linker()
	{
		delete[] ip_msg;
		delete[] buffer;
		for (int i = 0; i < maxlength; ++i)
			delete[] data_pointer[i];
		delete[] data_pointer;
		delete[] left;
	}
	inline void initialize()
	{
		ip_msg = new IP_Msg[maxlength];
		buffer = new Data_Segment[maxlength];
		bp = 0;
		data_pointer = new int*[maxlength];
		left = new int[maxlength];
		for (int i = 0; i < maxlength; ++i)
		{
			data_pointer[i] = NULL;
			ip_msg->iphdr = NULL;
		}
	}
	pcap_t * get_adapter();
	IP_Msg * combine(const u_char *);
	int send_by_frame(IP_Msg *, pcap_t *, unsigned short);
	void GetSelfMac(char*, unsigned short *);
	u_char* BuildArpPacket(unsigned short *, unsigned int, unsigned int);
	bool check(const u_char *);
	unsigned short crc16(unsigned char *, int);
	unsigned short checkCrc16(unsigned char *, int);
	int pppEncode(unsigned char * buf, int len);
	int pppDecode(unsigned char * buf, int len);
	void send_broadcast(pcap_t  *adapterHandle, unsigned int src_IP, unsigned int dst_IP);
	void get_mac(pcap_t  *adapterHandle);
	bool transtable(unsigned int IP);
	unsigned int getIP();
};
