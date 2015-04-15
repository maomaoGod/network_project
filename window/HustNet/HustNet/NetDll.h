#pragma once
class NetDll
{
public:
	 NetDll();
	~NetDll(); 
private:
    HINSTANCE Hinst;
public:
	void OpenDll();
	BOOL LoadDll(LPCTSTR path);
	FARPROC GetFun(LPCSTR);
};
