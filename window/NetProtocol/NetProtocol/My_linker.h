#pragma once
#include "pcap.h"
#include "NetProtocol.h"
#define FRAMESIZE 128
#define IP_SIZE sizeof(struct _iphdr)
#define MAC_ADDR_SIZE 3
typedef unsigned char Byte;



#define PPP_FRAME_FLAG  0x7e /* ��־�ַ� */
#define PPP_FRAME_ESC   0x7d /* ת���ַ� */
#define PPP_FRAME_ENC   0x20 /* �����ַ� */
#define BUF_LEN 1500

class my_linker
{
private:
	struct Frame_data{
		struct _iphdr IP;                  // ����㴫����IP
		Byte data[FRAMESIZE];                    // ���������
	};

	struct Frame{
		unsigned short MAC_des[MAC_ADDR_SIZE];           // MAC_dst MACĿ���ַ
		unsigned short MAC_src[MAC_ADDR_SIZE];           // MAC_src MACԴ��ַ
		unsigned short total_seq_num;        // ֡���ܸ���
		unsigned short datagram_num;         // ���ݱ����
		unsigned short seq;                  // ֡���
		unsigned short length;               // ��ǰ֡���ݵĳ���
		struct Frame_data frame_data;        // ֡���ݽṹ
	};

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
	my_linker(){}
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
	}
	pcap_t * get_adapter();
	IP_Msg * combine(const u_char *);
	int send_by_frame(struct IP_Msg *data_gram, pcap_t * adapterHandle, unsigned short i);
	int pppEncode(unsigned char * buf, int len);
	int pppDecode(unsigned char * buf, int len);

};
