/** @file
* @brief TCP UDP方法集
* @author ACM2012
* @date 2015/04/19
* @version 0.1
* @note
* 用类似CSocket的方法定义TCP连接，但是实现时分别实现TCP，UDP的功能
* 首先解决比较难的TCP连接，待TCP的功能实现后再来补全UDP的部分
* 具体实现部分包括报文头的构造，连接的实现，可靠传输机制，流量控制等
* 具体定义在后面详细说明
* 该文件中的UDP类尚未定义,待实现TCP后，再来补全相应部分.
*/


/**
* @class
* @brief 该类封装TCP协议方法.
* @author ACM2012
* @note
* 此类包含该类的构造函数和析构函数，将实现TCP协议.
* 构造函数MyTCP应传入对象进程的地址及端口号
* 析构函数根据需要设置
* 连接函数TCP_Conn 应当完成TCP协议的三次握手
* 被连接函数TCP_Accept应当能够正确的和请求连接的进程进行通讯
* 监视函数TCP_Listen应当监视外部请求，当收到请求时再调用其它成员来处理
* 设置函数TCP_SetOpt应当设置TCP中各项参数，如报文头中部分参数以及拥塞控制相关参数
* 不直接修改对应变量主要是出于安全考虑-------->>>>>初步实现可不使用此函数来控制
* 发送函数Simple_Send实现简单的发送一段报文的功能，具体的可靠协议在TCP_Send中实现
* 发送函数TCP_Send实现发送功能，同时实现中应该考虑到可靠传输的各项机制，如确认重传等
* 接收函数TCP_Receive实现接收数据的功能，注意这个函数可以设置阻塞的功能，
* 同时依然需要考虑可靠传输的问题
* 解除连接TCP_DisConn 完成解除连接的功能
* 类定义并不完整，只给出整体思路及框架
*/

class MyTCP
{
	/** @brief 构造函数*/
	MyTCP();
	/** @brief 析构函数*/
	~MyTCP();
public:
	/** @brief 连接函数,
	* @note
	* 对于一个连接，应当返回连接成功与否相关信息
	* 此处应该完成三次握手的可靠方案
	* 包括SYN,ACK等标志位的设置
	*/
	bool TCP_Conn();

	/** @brief 被连接函数,
	* @note
	* 应当能够正确的和请求连接的进程进行连接
	*/
	void TCP_Accept();

	/** @brief 监听函数,
	* @note
	* 监视外部请求，当收到请求时再调用其它成员来处理
	*/
	void TCP_Listen();

	/** @brief 发送函数,
	* @note
	* 实现发送功能，同时实现中应该考虑到可靠传输的各项机制
	* 注意重传的参数，如定时等不应使用局部变量
	* 可靠传输包括：超时重传，三次冗余ACK重传
	* 重传使用回退N步方法
	*/
	void TCP_Send();

	/** @brief 接收函数,
	* @note
	* 实现接收数据的功能
	* 注意可靠机制下的确认报文发送
	* 需要解决处理丢失的方法，包括累积确认等
	*/
	void TCP_Receive();

	/** @brief 解除连接函数,
	* @note
	* 完成解除连接的功能
	*/
	void TCP_DisConn();
private:
	/** @brief 报文头结构
	* @note
	*其中选项部分根据需要额外添加
	*/
	struct TCP_Head
	{
		unsigned short SourcePort;
		unsigned short DesPort;
		unsigned int No;
		unsigned int AckNo;
		short Len_Flag;
		unsigned short Receive_Window;
		unsigned short CheckSum;
		unsigned short Urgent_Pt;
	};
	/** @brief 简单发送
	* @note
	* 简单发送报文，用户不应该能使用这个函数，因为他并没有可靠机制
	*/
	void Simple_Send();
	/** @brief 简单接收
	* @note
	* 简单接收报文，用户不应该能使用这个函数，因为他并没有可靠机制
	*/
	void Simple_Receive();
};