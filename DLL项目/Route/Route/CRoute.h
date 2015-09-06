#define RoutePROTOCOL_TCP 6
#define RoutePROTOCOL_UDP 17
#define LINKSEND WM_USER+1002
#define ipMSS 1480						///< 定义IP数据报最大的分片大小1480
#define MSS 1024
#define maxnum 100				///<定义最大的节点数
#define maxint 999999			///<定义最长的节点间距离
#define MAX 0x3f3f3f3f
#define N 1010
#define data_info 1



/**
*@class <Edge>
*@author ACM2012
*@note
*	需要添加一个这样的结构，然后把边的个数和每个边的初始节点u,v,长度cost建立一个Edge结构
*/
struct Edge{
	int u, v;
};

/**
*@class <common_data>
*@author ACM2012
*@note
*	定义LS算法路由信息和DV算法路由信息的共同结构
*/
struct common_data{
	int node;				 ///< 节点个数
	int sid;				 ///< 源节点
	int did;				 ///< 目的节点
	int dist[maxnum];		 ///< 表示当前点到源点的最短路径长度
	int pre[maxnum];		 ///< 记录当前点的前一个结点
};

/**
*@class <LS_data>
*@author ACM2012
*@note
*	定义LS算法路由信息结构
*/
struct LS_data{
	common_data LsData;
	int c[maxnum][maxnum];   ///< 记录图的两点间路径长度
};

/**
*@class <DV_data>
*@author ACM2012
*@note
*	定义DV算法路由信息结构
*/
struct DV_data{
	common_data DvData;
	int edgenum;			 ///< 边数
	Edge E[maxnum];			 ///< 边结构数组
};


/**
*@class <Route_info>
*@author ACM2012
*@note
*	定义路由信息结构
*/
struct Route_info{
	Edge E[maxnum];
	int node;
	int edgenum;
	int sid;
	int did;
};

/**
*@class <IP_Msg>
*@author ACM2012
*@note
*	定义网络层传给链路层的结构
*/
struct RouteIP_Msg{
	unsigned int ih_saddr;		///< 32位源IP
	unsigned int ih_daddr;		///< 32位目的IP
	unsigned short ih_len;	    ///< 16位数据报总长度
	char *data;
};

/**
*@class UDP报文段结构
*@brief 存放UDP报文段的结构体
*@author ACM1201
*@note
*UDP报文段基本结构
*/
struct Routeudp_message
{
	unsigned udp_src_port : 16;			///< 源端口号
	unsigned udp_dst_port : 16;			///< 目的端口号
	unsigned udp_msg_length : 16;		///< 报文长度
	unsigned udp_checksum : 16;			///< 检验和
	char udp_app_data[MSS];			///< 数据
};

/**
*@class TCP报文段结构
*@brief 存放TCP报文段的结构体
*@author ACM1201
*@note
*TCP报文段基本结构
*/
struct Routetcp_message
{
	unsigned tcp_src_port : 16;	///< 源端口号
	unsigned tcp_dst_port : 16;	///< 目的端口号
	unsigned tcp_seq_number : 32;	///< 序号
	unsigned tcp_ack_number : 32;	///< 确认号（所期待的下一个序号）
	unsigned tcp_hdr_length : 4;	///< 首部长度（通常为20Byte）
	unsigned tcp_reserved : 6;	///< 保留字段（并未使用）
	unsigned tcp_urg : 1;	///< 紧急标志（并未使用）
	unsigned tcp_ack : 1;	///< 确认标志
	unsigned tcp_psh : 1;	///< 立即上交标志（并未使用）
	unsigned tcp_rst : 1;	///< 用于连接建立与拆除（并未使用）
	unsigned tcp_syn : 1;	///< 用于连接建立与拆除
	unsigned tcp_fin : 1;	///< 用于连接建立与拆除
	unsigned tcp_rcv_window : 16;	///< 接收窗口
	unsigned tcp_checksum : 16;	///< 检验和
	unsigned tcp_urg_ptr : 16;	///< 紧急数据指针（并未使用）
	char tcp_opts_and_app_data[MSS];	///< 选项字段（通常没有，故通常首部长度为20Byte）和数据
};

/**
*@class <Routeip_message>
*@brief 存放IP首部的结构体
*@author ACM2012
*@note
*自己定义的IP报文首部
*/
struct Routeip_message //定义IP数据报文
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
	bool operator == (const Routeip_message &it) const
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
	bool operator != (const Routeip_message &it) const
	{
		return !((*this) == it);
	}
};

/**
*@class <NAT_translation_table>
*@author ACM2012
*@note
*	定义NAT转换表
*/
struct NAT_translation_table
{
	unsigned int WAN_IP;			 ///<  外网IP
	unsigned short WAN_port;         ///<  外网端口
	unsigned int LAN_IP;			 ///<  内网IP
	unsigned short LAN_port;		 ///<  内网端口
	struct NAT_translation_table* next_p;
};

/**
*@class <In_NAT>
*@author ACM2012
*@note
*	定义NAT协议需要输入信息的结构
*/
struct In_NAT{
	unsigned int SIP;			 ///<  外网IP
	unsigned short Sport;        ///<  外网端口
	unsigned int DIP;			 ///<  内网IP
	unsigned short Dport;        ///<  内网端口
};


class CRoute
{
public:
	CRoute(void);
public:
	virtual ~CRoute(void);
	virtual void RS(int Routing_Select)=0;
	virtual int Datainfo(WPARAM wparam, LPARAM lparam)=0;
	virtual unsigned int getLocalIP()=0;
	virtual unsigned int getPublicIP()=0;
	virtual In_NAT* NAT(In_NAT* Nat)=0;
	virtual unsigned short iphdrmakesum(Routeip_message *ip)=0;
	virtual int IP2Link(WPARAM wparam, LPARAM lparam)=0;
public:
	virtual int  RecvMsg(WPARAM wparam, LPARAM lparam)=0;
	virtual int  SendMsg(WPARAM wparam, LPARAM lparam)=0;
	virtual unsigned int Num2IP(int num)=0;
	virtual int  IP2Num(unsigned int IP)=0;

	virtual void Route2LS(Route_info *info, LS_data *LsData)=0;
	virtual void Route2DV(Route_info *info, DV_data *DvData)=0;
	virtual int  searchPath(int *pre, int v, int u)=0;
	virtual void Dijkstra(int n, int v, int *dist, int *pre, int c[maxnum][maxnum])=0;
	virtual int  LS(LS_data *LSdata)=0;
	virtual void Bellman_Ford(int n, int edgenum, int v, int *dist, int *pre, Edge edge[N])=0;
	virtual int  DV(DV_data *DVdata)=0;
};