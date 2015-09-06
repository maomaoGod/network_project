
#define  ipMSS 1480			///< 定义IP数据报最大的分片大小1480
#define  LINKSEND           WM_USER+1002
#define  TRANSTOAPP         WM_USER+1003

/**
*@class <NETMsg>
*@author ACM2012
*@note
*	定义传输层传给网络层的结构
*/
struct NETMsg{                     ///<数据
	unsigned int ih_sport;		///< 32位源端口号
	unsigned int ih_dport;		///< 32位目的端口号
	unsigned int sip;			///< 32位源IP
	unsigned int dip;			///< 32位目的IP
	int datelen;
	char data[2048];
	unsigned int protocol;		///< 上层协议类型
};

/**
*@class <NETip_message>
*@brief 存放IP首部的结构体
*@author ACM2012
*@note
*自己定义的IP报文首部
*/
struct NETip_message //定义IP数据报文
{
	unsigned ih_version : 4;            ///< 标识了数据包的IP版本号,一共4位, 0100表示IPv4, 0110表示IPv6
	unsigned ip_hdr_length : 4;         ///< 首部长度, 一般为20字节
	unsigned ih_sever : 8;              ///< 8bit的服务类型
	unsigned ih_data_len : 16;	        ///< 16位数据报长度单位为字节

	unsigned ih_ident : 16;	            ///< 数据报的16bit标识
	unsigned ih_flags : 3;              ///< 数据报的3bit标志
	unsigned ih_offset : 13;	        ///< 数据报的13bit片偏移

	unsigned ih_TTL : 8;                ///< 数据报的寿命
	unsigned ih_protl : 8;	            ///< 数据报的协议, 定义0代表传输层数据, 定义1代表节点相接的路由信息
	unsigned ip_checksum : 16;		    ///< 数据报的首部检验和

	unsigned ih_saddr : 32;		            ///< 32位源IP
	unsigned ih_daddr : 32;		            ///< 32位目的IP
	char data[ipMSS];
	bool operator == (const NETip_message &it) const
	{
		if (ih_version != it.ih_version) return false;
		if (ip_hdr_length != it.ip_hdr_length) return false;
		if (ih_sever != it.ih_sever) return false;
		if (ih_data_len != it.ih_data_len) return false;
		if (ih_ident != it.ih_ident) return false;
		if (ih_flags != it.ih_flags) return false;
		if (ih_offset != it.ih_offset) return false;
		if (ih_TTL != it.ih_TTL) return false;
		if (ih_protl != it.ih_protl) return false;
		if (ip_checksum != it.ip_checksum) return false;
		if (ih_saddr != it.ih_saddr) return false;
		if (ih_daddr != it.ih_daddr) return false;
		return true;
	}
	bool operator != (const NETip_message &it) const
	{
		return !((*this) == it);
	}
};

/**
*@class <IP_Msg>
*@author ACM2012
*@note
*	定义网络层传给链路层的结构
*/
struct NETIP_Msg{
	unsigned int ih_saddr;		///< 32位源IP
	unsigned int ih_daddr;		///< 32位目的IP
	unsigned short ih_len;	    ///< 16位数据报总长度
	char *data;
};

// 此类是从 CNETCLI.dll 导出的
class CNETCLI {
public:
	CNETCLI();
public:
	virtual ~CNETCLI();
public:
	/** @brief 从网络层传送数据到传输层的函数 */
	virtual BOOL IP2Trans(WPARAM wparam, LPARAM lparam)=0;
	/** @brief 从网络层传送数据到链路层的函数 */
	virtual BOOL IP2Link(WPARAM wparam, LPARAM lparam)=0;
	/** @brief 计算IP数据报的首部检验和 */
	virtual unsigned short iphdrmakesum(NETip_message *ip)=0;
};

