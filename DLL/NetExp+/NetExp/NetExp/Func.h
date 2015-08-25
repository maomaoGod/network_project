#pragma once
class Func
{
private:
	float a, b;
public:
	Func();
	~Func();
public:
	bool GetFile();
	CString GetAdd();
	CString GetDec();
	CString GetMul();
	CString GetDiv();
};

