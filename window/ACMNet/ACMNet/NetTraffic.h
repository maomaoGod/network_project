#include "Tools.h"
#include "afxsock.h"
//there we will use the Socket class to transform the data
using namespace Tools;

namespace NetTraffic{
	//MySocket
	class MySocket : CSocket{

	};
	// the TCP link
	class TCP{
		Dlluser SocketDLL;//the socket we define from the dll
		MySocket mysocket;
	};
	// the UDP link
	class UDP{
		Dlluser SocketDLL;//the socket we define from the dll
		 
	};
}