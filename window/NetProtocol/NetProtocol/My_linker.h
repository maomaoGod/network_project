//定义了链路层类，该类内包含帧结构的定义以及收发函数的定义等等
class my_linker
{
  private:
	  struct Frame_data{
		struct _iphdr IP;                  // 网络层传来的IP
		Byte data[512];                    // 网络层数据
  };

  //帧结构的定义
  struct Frame{
		unsigned short MAC_des[3];           // MAC_dst MAC目标地址
		unsigned short MAC_src[3];           // MAC_src MAC源地址
		unsigned short total_seq_num;        // 帧的总个数
		unsigned short datagram_num;         // 数据报序号
		unsigned short seq;                  // 帧序号
		unsigned short length;               // 当前帧数据的长度
		struct Frame_data frame_data;        // 帧数据结构
	};

	struct Data_Segment
	{
		Byte data[512];
		unsigned short length;
	};

  public:
  int send_by_frame(struct IP_Msg *data_gram,pcap_t * adapterHandle, unsigned short i);
	my_linker();
	~my_linker();
};

