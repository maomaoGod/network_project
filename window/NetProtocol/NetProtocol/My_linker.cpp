#include <pcap.h>
#include <stdio.h>
#include <my_linker.h>
#include <string.h>

int my_linker::send(struct IP_Msg *data_grame, unsigned short i){
  pcap_if_t * allAdapters;//适配器列表
	pcap_if_t * adapter;
	pcap_t  * adapterHandle;//适配器句柄

	char errorBuffer[PCAP_ERRBUF_SIZE];//错误信息缓冲区
		
	//检索机器连接的所有网络适配器
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL,
		&allAdapters, errorBuffer) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex function: %s\n", errorBuffer);
		return -1;
	}
	if (allAdapters == NULL)
	{
		printf("\nNo adapters found! Make sure WinPcap is installed.\n");
		return 0;
	}
	
	int crtAdapter = 0;
	int adapterNumber;
	adapterNumber = 3;
	adapter = allAdapters;
	for (crtAdapter = 0; crtAdapter < adapterNumber - 1; crtAdapter++)
		adapter = adapter->next;
	
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
	if (adapterHandle == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter\n", adapter->name);
		pcap_freealldevs(allAdapters);
		return -1;
	}
	pcap_freealldevs(allAdapters);
	//发送数据报
	if ((send_by_frame(data_gram, adapterHandle, i)) != 0)
	{
		printf("error sending frames!\n");
		return -1;
	}
	return 0;
}

int send_by_frame(struct IP_Msg *data_gram, pcap_t * adapterHandle, unsigned short i)
{
	unsigned short seq = 0;
	unsigned short total_seq = ((*data_gram).iphdr->ih_len+FRAMESIZE-1) / FRAMESIZE;
	unsigned short copy_size;
	unsigned short left_size = (*data_gram).iphdr->ih_len;
	Byte *temp=(Byte *)&((*data_gram).data);
	u_char *packet; //待发送的数据封包
	while (left_size > 0)
	{
		struct Frame frame;
		frame.MAC_des[0] = 0xEC24;
		frame.MAC_des[1] = 0x1A99;
		frame.MAC_des[2] = 0x8C07;
		frame.MAC_src[0] = 0x5D68;
		frame.MAC_src[1] = 0xE643;
		frame.MAC_src[2] = 0xFDAC;
		frame.total_seq_num = total_seq;
		frame.seq = seq;
		memcpy(&(frame.frame_data.IP), (*data_gram).iphdr, IP_SIZE);
		frame.datagram_num = i;
		copy_size = min(FRAMESIZE, left_size);
		memcpy(frame.frame_data.data, temp, copy_size);
		frame.length = copy_size;
		temp += copy_size;
		if (copy_size < FRAMESIZE){
			memset(frame.frame_data.data + copy_size, 0, FRAMESIZE - copy_size);
		}
		left_size -= copy_size;
		packet = (u_char *)(&frame);
		//发送帧
		if (pcap_sendpacket(adapterHandle, // the adapter handle
			packet, // the packet
			sizeof(frame) // the length of the packet
			) != 0)
		{
			fprintf(stderr, "\nError sending the packet: \n", pcap_geterr(adapterHandle));
			return -1;
		}
		else
		{
			printf("send frame %d successfully!: size %d bytes\n", seq,sizeof(frame));
			seq += 1;
		}
	}
	return 0;
}
