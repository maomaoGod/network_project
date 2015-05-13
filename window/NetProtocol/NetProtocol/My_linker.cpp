#pragma once
#include "stdafx.h"
#include "my_linker.h"
#include "NetProtocol.h"

typedef _iphdr IP_HEADER;

pcap_t * my_linker::get_adapter()
{
	pcap_if_t * allAdapters;//�������б�

	pcap_if_t * adapter;

	pcap_t           * adapterHandle;//���������

	char errorBuffer[PCAP_ERRBUF_SIZE];//������Ϣ������

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL,

	&allAdapters, errorBuffer) == -1)

	{//�����������ӵ���������������

	fprintf(stderr, "Error in pcap_findalldevs_ex function: %s\n", errorBuffer);

	return NULL;

	}

	if (allAdapters == NULL)

	{//�������κ�������

	printf("\nNo adapters found! Make sure WinPcap is installed.\n");

	return 0;

	}

	int crtAdapter = 0;
	FILE *pfile;	
	fopen_s(&pfile,"adapter.txt", "w+");
	for (adapter = allAdapters; adapter != NULL; adapter = adapter->next)

	{
		fprintf(pfile, "\n%d.%s ", ++crtAdapter, adapter->name);
	//printf("-- %s\n", adapter->description);
	}
	fclose(pfile);



	printf("\n");
	
	//ѡ��Ҫ�������ݰ���������

	int adapterNumber = 6;
	/*
	printf("Enter the adapter number between 1 and %d:", crtAdapter);

	scanf_s("%d", &adapterNumber);
	
	if (adapterNumber < 1 || adapterNumber > crtAdapter)

	{

		printf("\nAdapter number out of range.\n");

		// �ͷ��������б�

		pcap_freealldevs(allAdapters);

		return NULL;

	}
	*/
	adapter = allAdapters;

	for (crtAdapter = 0; crtAdapter < adapterNumber - 1; crtAdapter++)

		adapter = adapter->next;

	// ��ָ��������

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

	{//ָ����������ʧ��

		fprintf(stderr, "\nUnable to open the adapter\n", adapter->name);

		// �ͷ��������б�

		pcap_freealldevs(allAdapters);

		return NULL;

	}
	pcap_freealldevs(allAdapters);//�ͷ��������б�

	return adapterHandle;

}

int my_linker::send_by_frame(struct IP_Msg *data_gram, pcap_t * adapterHandle, unsigned short i)
{
	unsigned short seq = 0;
	unsigned short total_seq = ((*data_gram).iphdr->ih_len + FRAMESIZE - 1) / FRAMESIZE;
	unsigned short copy_size;
	unsigned short left_size = (*data_gram).iphdr->ih_len;
	Byte *temp = (Byte *)&((*data_gram).data);
	u_char *packet; //�����͵����ݷ��
	while (left_size > 0)
	{
		struct Frame frame;
		frame.MAC_des[0] = 0x34C4;
		frame.MAC_des[1] = 0x016B;
		frame.MAC_des[2] = 0x58D1;
		frame.MAC_src[0] = 0x34C4;
		frame.MAC_src[1] = 0x016B;
		frame.MAC_src[2] = 0x58D1;
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
		//����֡
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
			printf("send frame %d successfully!: size %d bytes\n", seq, sizeof(frame));
			seq += 1;
		}
	}
	return 0;
}

IP_Msg * my_linker::combine(const u_char * packetData)
{
	Frame &Receive = *((Frame *)packetData);

	Frame_data *fdp;

	Frame_data frame_data;

	if (Receive.MAC_des[0] != 0x34C4 || Receive.MAC_des[1] != 0x016B || Receive.MAC_des[2] != 0x58D1) return NULL;
	if (Receive.MAC_src[0] != 0x34C4 || Receive.MAC_src[1] != 0x016B || Receive.MAC_src[2] != 0x58D1) return NULL;

	//puts("fuck");

	frame_data = Receive.frame_data;

	int id = Receive.datagram_num;
	int tot = Receive.total_seq_num;
	int len = Receive.length;
	int seq = Receive.seq;

	if (data_pointer[Receive.datagram_num] == NULL)					//��һ���յ�����ŵ����ݱ�
	{

		data_pointer[id] = new int[tot];
		for (int i = 0; i < tot; ++i) data_pointer[id][i] = -1;

		left[id] = tot;

		ip_msg[id].iphdr = new IP_HEADER;
		*(ip_msg[id].iphdr) = frame_data.IP;

		{
			IP_HEADER out = *(ip_msg[id].iphdr);
		}

		buffer[bp].length = len;
		for (int i = 0; i < len; ++i)
			buffer[bp].data[i] = frame_data.data[i];

		data_pointer[id][seq] = bp++;

		if (--left[id] == 0)									//���ݱ��������
		{
			puts("Finish!");
			int data_len = 0;
			for (int i = 0; i < tot; ++i)
			{
				int ptr = data_pointer[id][i];
				for (int j = 0; j < buffer[ptr].length; ++j)
					ip_msg[id].data[data_len++] = (char)buffer[ptr].data[j];
			}
			ip_msg[id].data[data_len] = 0;
			return ip_msg + id;
		}
	}
	else if (data_pointer[id][seq] == -1)					//δ�յ�����һ��֡
	{
		buffer[bp].length = len;
		for (int i = 0; i < len; ++i)
			buffer[bp].data[i] = frame_data.data[i];

		data_pointer[id][seq] = bp++;

		if (--left[id] == 0)									//���ݱ��������
		{
			puts("Finish!");
			int data_len = 0;
			for (int i = 0; i < tot; ++i)
			{
				int ptr = data_pointer[id][i];
				for (int j = 0; j < buffer[ptr].length; ++j)
					ip_msg[id].data[data_len++] = (char)buffer[ptr].data[j];
			}
			ip_msg[id].data[data_len] = 0;
			return ip_msg+id;
		}
	}
	return NULL;
}
