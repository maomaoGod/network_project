#pragma once

// mycsocket ����Ŀ��

class mycsocket : public CSocket
{
public:
	mycsocket();
	virtual ~mycsocket();
	virtual void OnReceive(int nErrorCode);
};


