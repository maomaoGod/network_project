#pragma once
#include "stdafx.h"
#include "my_linker.h"
#include "NetProtocol.h"


void my_linker::get_adapter()
{
	pcap_if_t * allAdapters;//适配器列表

	pcap_t           * adapterHandle;//适配器句柄

	pcap_if_t * adapter;

	char errorBuffer[PCAP_ERRBUF_SIZE];//错误信息缓冲区

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL,

		&allAdapters, errorBuffer) == -1)

	{//检索机器连接的所有网络适配器

		fprintf(stderr, "Error in pcap_findalldevs_ex function: %s\n", errorBuffer);

		return ;

	}

	if (allAdapters == NULL)

	{//不存在任何适配器

		printf("\nNo adapters found! Make sure WinPcap is installed.\n");

		return ;

	}

	int crtAdapter = 0;
	FILE *pfile;
	fopen_s(&pfile, "adapter.txt", "w+");
	for (adapter = allAdapters; adapter != NULL; adapter = adapter->next)

	{
		fprintf(pfile, "\n%d.%s ", ++crtAdapter, adapter->name);
		//printf("-- %s\n", adapter->description);
	}
	fclose(pfile);

	printf("\n");

	//选择要捕获数据包的适配器

	adapter = allAdapters;

	for (; ; )

	{
		bool ck = false;
		int retValue;
		struct pcap_pkthdr * packetHeader;
		const u_char       * packetData;
		adapterHandle = pcap_open(adapter->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errorBuffer);
		if ((retValue = pcap_next_ex(adapterHandle, &packetHeader, &packetData)) > 0)
		{
			ck = true;
		}
		if (ck) break;
		adapter = adapter->next;
		if (adapter == NULL) adapter = allAdapters;
	}
	// 打开指定适配器

	adapterHandle = pcap_open(adapter->name, // name of the adapter

		65536,         // portion of the packet to capture

		// 65536 guarantees that the whole 

		// packet will be captured

		PCAP_OPENFLAG_PROMISCUOUS, // promiscuous mode

		1000,             // read timeout - 1 millisecond

		NULL,          // authentication on the remote machine

		errorBuffer    // error buffer

		);

	GetSelfMac(adapter->name + 8, mac_src);

	if (adapterHandle == NULL)

	{//指定适配器打开失败

		fprintf(stderr, "\nUnable to open the adapter\n", adapter->name);

		// 释放适配器列表

		pcap_freealldevs(allAdapters);

		return ;

	}
	pcap_freealldevs(allAdapters);//释放适配器列表

	this->adapterHandle = adapterHandle;

}

/**
* @author ACM2012
* @return 返回值指示本次发送是否成功，返回值为0表示本次发送成功，返回值为1表示本次发送失败
* @note
* 该模块将实现数据报的拆帧工作和发送工作，帧结构参照以太网标准帧结构。发送帧基于Winpcap提供的
* 接口pcap_sendpacket(adapterHandle, // the adapter handle
		      packet, 	     // the packet
		      sizeof(frame)  // the length of the packet
* 拆帧完成后逐一调用上述函数进行发送
* @remarks
*/
int my_linker::send_by_frame(struct IP_Msg *data_gram, pcap_t * adapterHandle,
	unsigned short i,unsigned short len)
{
	int K = 0;/**@brief 数据报序号，用于接收端装帧*/
	srand(time(0));
	unsigned short seq = 0;/**@brief 帧序号*/
	unsigned short total_seq = (len + FRAMESIZE - 1) / FRAMESIZE;/**@brief 数据报拆分成帧的总个数*/
	unsigned short copy_size;/**@brief 帧大小*/
	unsigned short left_size = len;/**@brief len为网络层传给链路层的参数，表示网络层要发送总数据的大小。
				       /**left_size表示在发送过程中数据报剩余数据量的大小*/
	Byte *temp = (Byte *)(data_gram->data);/**@brief temp设置为指向待发送数据*/
	u_char *packet;/**@brief 数据报剩余数据量的大小*/
	struct pcap_pkthdr * packetHeader;/**@brief 指向包头的指针*/
	const u_char       * packetData;/**@brief 指向待发送数据的指针*/
	int retValue;/**@brief 指示网络适配器是否可用，若可用，则改值将会被设置为0，否则将会被设置为1*/
	
	/**@brief 当数据报还有剩余的字节没有发送时，则进入循环体*/
	while (left_size > 0)
	{
		struct Frame frame;/**@brief 声明一个待发送帧结构，但尚未填充数据*/
		unsigned short tempCRC;/**@brief 声明一个CRC冗余检测变量，但尚未计算出其值*/
		/**@brief 填充目的MAC地址和源MAC地址，均为6字节*/
		frame.MAC_des[0] = mac_des[0];
		frame.MAC_des[1] = mac_des[1];
		frame.MAC_des[2] = mac_des[2];
		frame.MAC_src[0] = mac_src[0];
		frame.MAC_src[1] = mac_src[1];
		frame.MAC_src[2] = mac_src[2];
		frame.total_seq_num = total_seq + 0xcccc;
		frame.seq = seq + 0xcccc;
		/**@brief 为当前待发送的帧结构填充序号字段i，用于接收端按序接收*/
		frame.datagram_num = i;
		/**@brief 填充帧结构数据字段的大小，由于最后一个帧往往不能填充满帧数据字段，因此进行以下min操作*/
		copy_size = min(FRAMESIZE, left_size);
		/**@brief 从网络层数据区复制数据到帧结构中*/
		memcpy(frame.data, temp, copy_size);
		frame.length = copy_size;/**@brief 填充帧结构数据长度字段*/
		temp += copy_size;/**@brief 网络层数据报数据字段指针后移，指向下一个待发送数据区*/
		if (copy_size < FRAMESIZE){
			memset(frame.data + copy_size, 0, FRAMESIZE - copy_size);
		}
		left_size -= copy_size;/**@brief 剩余待发送的数据字段长度减少*/

		packet = (u_char *)(&frame);/**@brief 待发送数据区指针指向待发送帧结构首地址*/
		tempCRC = crc16(packet, (char *)&frame.CRC - (char *)&frame);/**@brief 计算CRC的值*/
		frame.CRC = tempCRC << 8 | tempCRC >> 8;
		bool ck = false;
		while (!ck)
		{
			int wait = rand() % (1 << K), bg = clock();
			/**@brief 调用pcap_sendpacket发送当前帧结构，成功返回0*/
			if (pcap_sendpacket(adapterHandle,/**@brief 适配器指针*/ 
				packet, /**@brief 待发送帧*/
				sizeof(frame) /**@brief 发送数据的总子节数*/
				) != 0)
			{
				fprintf(stderr, "\nError sending the packet: \n", pcap_geterr(adapterHandle));
				return -1;
			}
			else
			{
				printf("send frame %d successfully!: size %d bytes\n", seq, sizeof(frame));	
				seq += 1;/**@brief 改帧结构发送成功，帧序号自动加1，准备在下一次循环中赋值给帧序号*/
				break;
			}
		}
	}
	return 0;/**@brief 发送成功，返回0*/
}

/**
* @author ACM2012
* @return 返回指向数据报的指针，用于将该数据报提交给网络层。若数据报未拼接完成，返回NULL指针。
* @note
* 该模块实现判断帧是否合法，以及把帧组装成数据报。帧结构参照以太网标准帧结构。
* @remarks
*/
char * my_linker::combine(const u_char * packetData)
{

	Frame &Receive = *((Frame *)packetData);/**@brief 收到的帧*/
	
	/**@brief 判断接收到的帧的MAC源地址与目的地址是否正确，若不正确，返回NULL。*/
	for (int i = 0; i < 3; ++i)
	{
		if (Receive.MAC_des[i] != mac_src[i]) return NULL;
		if (Receive.MAC_src[i] != mac_des[i]) return NULL;
	}
	
	/**@brief CRC检验收到的帧是否出错，若出错，返回NULL。*/
	if (checkCrc16((unsigned char *)packetData, (char *)&Receive.CRC - (char *)&Receive + 2) != 0)
	{
		return NULL;
	}

	//puts("fuck");
	
	/**@brief 发送方会将帧序号和帧的总数加上0xcccc，这里减去。*/
	Receive.seq -= 0xcccc;
	Receive.total_seq_num -= 0xcccc;

	int id = Receive.datagram_num;/**@brief 数据报编号*/
	int tot = Receive.total_seq_num;/**@brief 该数据报包含的帧的总数*/
	int len = Receive.length;/**@brief 收到的数据的长度*/
	int seq = Receive.seq;/**@brief 帧的编号*/

	if (data_pointer[Receive.datagram_num] == NULL)/**@brief 第一次收到该序号的数据报*/
	{

		data_pointer[id] = new int[tot];/**@brief 对该数据报的数据指针分配空间*/
		for (int i = 0; i < tot; ++i) data_pointer[id][i] = -1;/**@brief 对数据指针赋初值*/

		left[id] = tot;/**@brief 还未收到的帧的数目*/

		msg[id] = new char[1500];/**@brief 对该数据报分配空间*/
		
		/**@brief bp是指向缓冲区的指针，这里把收到的数据暂时放在第bp个缓冲区并记录长度*/
		buffer[bp].length = len;
		for (int i = 0; i < len; ++i)
			buffer[bp].data[i] = Receive.data[i];

		data_pointer[id][seq] = bp++;/**@brief 数据指针指向这个缓冲区*/
		
		/**@brief 还未收到的帧的数目为0，数据报接收完成*/
		if (--left[id] == 0)
		{
			/**@brief 将该数据报的数据指针指向的各个缓冲区的数据拼接成完整的数据报。*/
			int data_len = 0;
			for (int i = 0; i < tot; ++i)
			{
				int ptr = data_pointer[id][i];
				for (int j = 0; j < buffer[ptr].length; ++j)
					msg[id][data_len++] = (char)buffer[ptr].data[j];
			}
			
			delete[] data_pointer[id];/**@brief 删除分配过的存储空间*/
			data_pointer[id] = NULL;
			msg[id][data_len] = 0;
			return msg[id];/**@brief 返回指向该数据报的指针*/
		}
	}
	else if (data_pointer[id][seq] == -1)/**@brief 收到过该序号的数据报，但未收到过这个帧*/
	{
		/**@brief bp是指向缓冲区的指针，这里把收到的数据暂时放在第bp个缓冲区并记录长度*/
		buffer[bp].length = len;
		for (int i = 0; i < len; ++i)
			buffer[bp].data[i] = Receive.data[i];

		data_pointer[id][seq] = bp++;/**@brief 数据指针指向这个缓冲区*/
		
		/**@brief 还未收到的帧的数目为0，数据报接收完成*/
		if (--left[id] == 0)
		{
			/**@brief 将该数据报的数据指针指向的各个缓冲区的数据拼接成完整的数据报。*/
			int data_len = 0;
			for (int i = 0; i < tot; ++i)
			{
				int ptr = data_pointer[id][i];
				for (int j = 0; j < buffer[ptr].length; ++j)
					msg[id][data_len++] = (char)buffer[ptr].data[j];
			}
			
			delete[] data_pointer[id];/**@brief 删除分配过的存储空间*/
			data_pointer[id] = NULL;
			msg[id][data_len] = 0;
			return msg[id];/**@brief 返回指向该数据报的指针*/
		}
	}
	return NULL;
}

//获得网卡的MAC地址
//pDevName 网卡的设备名称
void my_linker::GetSelfMac(char* pDevName, unsigned short *mac_src){
	static unsigned short mac[3];
	memset(mac, 0, sizeof(mac));
	LPADAPTER lpAdapter = PacketOpenAdapter(pDevName);
	if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE))
	{
		return;
	}
	PPACKET_OID_DATA OidData = (PPACKET_OID_DATA)malloc(6 + sizeof(PACKET_OID_DATA));
	if (OidData == NULL)
	{
		PacketCloseAdapter(lpAdapter);
		return;
	}
	// 
	// Retrieve the adapter MAC querying the NIC driver 
	// 
	OidData->Oid = OID_802_3_CURRENT_ADDRESS;
	OidData->Length = 6;
	memset(OidData->Data, 0, 6);
	BOOLEAN Status = PacketRequest(lpAdapter, FALSE, OidData);
	*mac_src = (OidData->Data[1] << 8) | OidData->Data[0];
	*(mac_src + 1) = (OidData->Data[3] << 8) | OidData->Data[2];
	*(mac_src + 2) = (OidData->Data[5] << 8) | OidData->Data[4];
	free(OidData);
	PacketCloseAdapter(lpAdapter);
}

/*
* 封装ARP请求包
* source_mac 源MAC地址
* srcIP 源IP
* destIP 目的IP
*/
u_char* my_linker::BuildArpPacket(unsigned short *source_mac,
	unsigned int srcIP, unsigned int destIP)
{
	static Broadcast_frame broad_frame;
	memcpy(broad_frame.MAC_src, source_mac, sizeof(short)* 3);
	broad_frame.MAC_des[0] = 0xFFFF;
	broad_frame.MAC_des[1] = 0xFFFF;
	broad_frame.MAC_des[2] = 0xFFFF;
	broad_frame.IP_dst = destIP;
	broad_frame.IP_src = srcIP;
	broad_frame.type = 0x0806;
	return (u_char *)&broad_frame;
}

/**
* @author ACM2012
* @return 若这个帧是广播帧，返回true，否则返回false。
* @note
* 该模块是接收方用于判断接收的帧是否是广播帧的模块。
* @remarks
*/
bool my_linker::check(const u_char * packetData)
{
	Broadcast_frame frame = *((Broadcast_frame *)packetData);
	if (frame.type == 0x0806 && frame.MAC_des[0] == 0xFFFF && frame.MAC_des[0] == 0xFFFF && frame.MAC_des[0] == 0xFFFF
		&& frame.IP_dst==getIP())
		return true;
	return false;
}

/**
* @author ACM2012
* @param ptr: 指向帧内容的指针
*        len: 帧长度
* @return 返回计算得到的CRC16结果
* @note
* 该方法计算给定帧的CRC16结果，符合标准CRC16CCITT要求，同时CRC16也是以太网使用的CRC结构之一。
*   名称       生成多项式             简记式*  标准引用
*   CRC-4       x4+x+1                  3         ITU G.704
*   CRC-8       x8+x5+x4+1              0x31                   
*   CRC-8       x8+x2+x1+1              0x07                   
*   CRC-8       x8+x6+x4+x3+x2+x1       0x5E
*   CRC-12      x12+x11+x3+x+1          80F
*   CRC-16      x16+x15+x2+1            8005      IBM SDLC
*   CRC16-CCITT x16+x12+x5+1            1021      ISO HDLC, ITU X.25, V.34/V.41/V.42, PPP-FCS
*   CRC-32      x32+x26+x23+...+x2+x+1 04C11DB7 ZIP, RAR, IEEE 802 LAN/FDDI, IEEE 1394, PPP-FCS
*   CRC-32c     x32+x28+x27+...+x8+x6+1 1EDC6F41 SCTP
* @remarks
*/

unsigned short my_linker::crc16(unsigned char* ptr, int len)
{
	unsigned short crc = 0x0000;
	static const unsigned short SUBCRC = 0x1021;
	while (len--)
	{
		for (unsigned char i = 0x80; i >0; i /= 2)
		{
			if ((crc & 0x8000) != 0)
			{
				crc = (crc << 1) + ((*ptr & i) != 0);
				crc = crc ^ SUBCRC;
			}
			else
			{
				crc = (crc << 1) + ((*ptr & i) != 0);
			}
		}
		ptr++;
	}
	int j = 2;
	while (j--)
	{
		for (unsigned char i = 0x80; i > 0; i /= 2)
		{
			if ((crc & 0x8000) != 0)
			{
				crc = (crc << 1) + 0;
				crc = crc ^ SUBCRC;
			}
			else
			{
				crc = (crc << 1);
			}
		}
	}
	return crc;
}

/**
* @author ACM2012
* @param ptr: 指向帧内容的指针
*        len: 帧长度
* @return 返回0证明正确，返回其他结果证明错误
* @note
* 该方法对帧加上CRC结果进行求解，如果返回是0则正确，返回其他值则错误。
*/

unsigned short my_linker::checkCrc16(unsigned char *ptr, int len)
{
	unsigned short crc = 0x0000;
	static const unsigned short SUBCRC = 0x1021;
	while (len--)
	{
		for (unsigned char i = 0x80; i >0; i /= 2)
		{
			if ((crc & 0x8000) != 0)
			{
				crc = (crc << 1) + ((*ptr & i) != 0);
				crc = crc ^ SUBCRC;
			}
			else
			{
				crc = (crc << 1) + ((*ptr & i) != 0);
			}
		}
		ptr++;
	}
	return crc;
}
/**
* @author ACM2012
* @return 返回值指示编码后的ppp数据帧长度
* @note
* 该模块将实现ppp数据帧的编码
* 采用比特填充来完成
* @remarks
*/
int my_linker::pppEncode(unsigned char * buf, int len)
{
	const unsigned short PPP_FRAME_FLAG = 0x7e;
	const unsigned short PPP_FRAME_ESC = 0x7d;
	const unsigned short PPP_FRAME_ENC = 0x20;
	/**@brief 定义标志字符，转义字符和编码字符*/
	const unsigned short BUF_LEN = 1500;
	unsigned char * pi, *po;
	int i, olen;
	unsigned char obuf[BUF_LEN];

	if (len > (BUF_LEN >> 1))
	{
		return -1;
	}
        /**@brief 检查要编码的字符长度，按最坏情况，一个字符会编码成两个字符，所以这里只能编码最大缓冲区长度一半*/
	memset(obuf, 0, BUF_LEN);
	pi = buf;
	po = obuf;
	olen = len;

	for (i = 0; i<len; i++)
	{
		if (*pi == PPP_FRAME_FLAG
			|| *pi == PPP_FRAME_ESC
			|| *pi < 0x20)
		{
			*po = PPP_FRAME_ESC;
			po++;
			olen++;
			*po = *pi ^ PPP_FRAME_ENC;
		}
		/**@brief 遇到标志字符，转义字符和小于0x20的控制字符，都要进行编码。方法就是在其前面插入一个转义字符0x7d，然后对其第6位取补码*/
		else
		{
			*po = *pi;
		}
		/**@brief 其他字符，不做任何修改*/
		pi++;
		po++;
	}

	memcpy(buf, obuf, olen);

	return olen;
	/**@brief 修改缓冲区，返回编码后的字符长度*/
}
/**
* @author ACM2012
* @return 返回值指示解码后的ppp数据帧长度
* @note
* 该模块将实现ppp数据帧的解码
* 是编码函数的逆运算
* @remarks
*/
int my_linker::pppDecode(unsigned char * buf, int len)
{
	const unsigned short PPP_FRAME_FLAG = 0x7e;
	const unsigned short PPP_FRAME_ESC = 0x7d;
	const unsigned short PPP_FRAME_ENC = 0x20;
	/**@brief 定义标志字符，转义字符和编码字符*/
	const unsigned short BUF_LEN = 1500;
	unsigned char * pi, *po;
	int i, olen;
	unsigned char obuf[BUF_LEN];

	if (len > BUF_LEN)
	{
		return -1;
	}
	/**@brief 检查要解码的字符长度*/
	memset(obuf, 0, BUF_LEN);
	pi = buf;
	po = obuf;
	olen = len;

	for (i = 0; i<len; i++)
	{
		if (*pi == PPP_FRAME_ESC)
		{

			/* 跳过转义字节 */
			pi++;
			olen--;

			/*异或第6位*/
			*po = *pi ^ PPP_FRAME_ENC;
		}
		/**@brief 解码的主要实现，遇到转义字符，将其跳过，对紧接其后的字符的第6位去补码*/
		else
		{
			*po = *pi;
		}
		/**@brief 其他字符，不做处理*/
		pi++;
		po++;
	}

	memcpy(buf, obuf, olen);

	return olen;
	/**@brief 修改缓冲区，返回解码后的字符长度*/
}

void my_linker::send_broadcast(pcap_t  *adapterHandle, unsigned int src_IP, unsigned int dst_IP)
{
	//广播获取MAC地址
	if (pcap_sendpacket(adapterHandle,
		BuildArpPacket(mac_src, src_IP, dst_IP),
		sizeof(struct Broadcast_frame)
		) != 0)
	{
		fprintf(stderr, "Error broadcasting!\n", pcap_geterr(adapterHandle));
	}
}

bool my_linker::get_mac(pcap_t  *adapterHandle)
{
	//接收广播的数据报
	struct pcap_pkthdr * packetHeader;
	const u_char       * packetData;
	struct  Broadcast_frame* t;
	int retValue, times = 0;
	while ((retValue = pcap_next_ex(adapterHandle, &packetHeader, &packetData)) >= 0)
	{
		if (retValue == 0)
			continue;
		else
		{
			t = (struct Broadcast_frame *)packetData;
			if ((t->type == 0x0806) && (t->MAC_des[0] == mac_src[0] || t->MAC_des[1] == mac_src[1] || t->MAC_des[2] == mac_src[2]))
			{
				memcpy(mac_des, t->MAC_src, sizeof(short)* 3);
				return true;
			}
			if (++times >= 10) break;
		}
	}
	return false;
}
bool my_linker::transtable(unsigned int IP)
{
	for  (int i = 0; i < table_size; i++)
	{
		if (IP == transIP[i])
		{
			memcpy(mac_des, transmac[i], sizeof(short)* 3);
			return true;
		}
	}
	return false;
}

inline unsigned int my_linker::getIP()
{
	WSADATA wsaData;
	char name[155];
	char *ip;
	PHOSTENT hostinfo;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)
	{
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
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
			ip_seg_number = ip_seg_number * 10 + ip[i] - 48;
		}
	}
	ip_number = ip_number * 256 + ip_seg_number;
	return ip_number;
}

DWORD WINAPI my_linker::NewPackThread(LPVOID lParam)
{
	my_linker *pthis = (my_linker *)lParam;
	pthis->packcap();
	return 0;
}

/**
* @author ACM2012
* @return 无返回值。
* @note
* 该模块是接收方用于接收的模块，这个模块将单独在一个线程上运行，会不断使用pcap_next_ex方法捕捉帧，
* 每捕捉到一个帧，会判断这个帧是否为广播帧，若为广播帧，向发送方回复广播帧告知MAC地址，否则调用
* combine方法判断并合并这个帧。
* @remarks
*/
void my_linker::packcap()
{
	struct pcap_pkthdr * packetHeader;
	const u_char       * packetData;
	int retValue;
	
	/**@brief 重复接收帧*/
	while ((retValue = pcap_next_ex(adapterHandle,

		&packetHeader,

		&packetData)) >= 0)

	{
		if (retValue == 0) continue;/**@brief 收到的包是无效的，继续循环*/
		if (sizeof(Broadcast_frame) > packetHeader->len) continue;/**@brief 收到的包的长度小于广播帧的长度，继续循环*/

		if (check(packetData))/**@brief 判断是否为广播帧*/
		{
			Broadcast_frame r_frame = *((Broadcast_frame *)packetData), s_frame;
			
			/**@brief 发送响应的广播帧，告知发送方自己的MAC地址*/
			for (int i = 0; i < 3; ++i)
			{
				mac_des[i] = s_frame.MAC_des[i] = r_frame.MAC_src[i];
				s_frame.MAC_src[i] =mac_src[i];
			}
			s_frame.type = 0x0806;
			s_frame.IP_dst = r_frame.IP_src;
			s_frame.IP_src = r_frame.IP_dst;
			Sleep(200);
			pcap_sendpacket(adapterHandle, 
				(const u_char *)&s_frame,
				sizeof(s_frame));
		}
		else AfxGetApp()->m_pMainWnd->SendMessage(LINKTOIP, (WPARAM)packetData);/**@brief 不是广播帧，用发消息的机制调用combine方法*/
	}
}

void my_linker::Link2IP(WPARAM wparam)
{
	const u_char * packetData = (const u_char *)wparam;
	char * msg;
	msg = combine(packetData);
	if (msg != NULL)
	{
		AfxGetApp()->m_pMainWnd->SendMessage(IPTOTRANS, (WPARAM)msg);
	}
}
/**
* @author ACM2012
* @param adhandle:指向适配器的指针
*        ip_addr: 要获取MAC地址的主机的IP地址
*        ip_mac:  获取的MAC地址
* @return 返回true or false
* @note
* 该方法首先会按照标准ARP协议要求的帧结构组织报文头，
* 然后将目的mac地址设置成广播地址0xffffffffff
* 然后将该帧进行广播，其他主机收到该广播帧后遵循ARP协议，发送应答帧
* 该方法接收到应答帧后返回结果.
* @remarks
*/	
int my_linker::ArpGetMacFromIp(pcap_t *adhandle, const char *ip_addr, unsigned char *ip_mac)
{
	unsigned char sendbuf[42]; /**@brief arp包结构大小*/
	int i = -1;
	int res;
	EthernetHeader eh; /**@brief 以太网帧头 */
	Arpheader ah;  /**@brief ARP帧头 */
	struct pcap_pkthdr * pkt_header;
	const u_char * pkt_data;
	/**@brief 将已开辟内存空间 eh.dest_mac_add 的首 6个字节的值设为值 0xff */
	memset(eh.DestMAC, 0xff, 6); /**@brief 目的地址为全为广播地址 */
	memset(eh.SourMAC, 0x0f, 6);
	memset(ah.DestMacAdd, 0x0f, 6);
	memset(ah.SourceMacAdd, 0x00, 6);
	/**@brief htons将一个无符号短整型的主机数值转换为网络字节顺序 */
	eh.EthType = htons(ETH_ARP);
	ah.HardwareType = htons(ARP_HARDWARE);
	ah.ProtocolType = htons(ETH_IP);
	ah.HardwareAddLen = 6;
	ah.ProtocolAddLen = 4;
	ah.SourceIpAdd = inet_addr("192.168.1.111");
	/**@brief 指示该帧为请求帧 */
	ah.OperationField = htons(ARP_REQUEST);
	ah.DestIpAdd = inet_addr("192.168.1.1");
	memset(sendbuf, 0, sizeof(sendbuf));
	memcpy(sendbuf, &eh, sizeof(eh));
	memcpy(sendbuf + sizeof(eh), &ah, sizeof(ah));
	printf("%s", sendbuf);
	if (pcap_sendpacket(adhandle, sendbuf, 42) == 0) {
		printf("\nPacketSend succeed\n");
	}
	else {
		printf("PacketSendPacket in getmine Error: %d\n", GetLastError());
		return 0;
	}
	/**@brief 从interface或离线记录文件获取一个报文 */
	//pcap_next_ex(pcap_t* p,struct pcap_pkthdr** pkt_header,const u_char** pkt_data)
	while ((res = pcap_next_ex(adhandle, &pkt_header, &pkt_data)) >= 0) {
		if (*(unsigned short *)(pkt_data + 12) == htons(ETH_ARP)
			&& *(unsigned short*)(pkt_data + 20) == htons(ARP_REPLY)
			&& *(unsigned long*)(pkt_data + 38)
			== inet_addr("192.168.1.111")) {
			for (i = 0; i < 6; i++) {
				ip_mac[i] = *(unsigned char *)(pkt_data + 22 + i);
			}
			printf("获取自己主机的MAC地址成功!\n");
			break;
		}
	}
	if (i == 6) {
		return 1;
	}
	else {
		return 0;
	}
}
