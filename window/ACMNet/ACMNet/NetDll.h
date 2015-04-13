#pragma once
class NetDll
{
public:
	  NetDll();
	~NetDll(); 
public:
	HINSTANCE Hinst; 
	void OpenDll();
	void LoadDll(LPCTSTR path);
};
