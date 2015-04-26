#pragma once

// mycsocket ÃüÁîÄ¿±ê

class mycsocket : public CSocket
{
public:
	mycsocket();
	virtual ~mycsocket();
	virtual void OnReceive(int nErrorCode);
};


