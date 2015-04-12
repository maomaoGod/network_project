#pragma once
#include "UserCmd.h"
#include "CmdView.h"
#include "stdafx.h"

extern void PrintLog(CString);
extern void PrintRp(CString);
void Compute(CString e){
	int pos;
	CString a, b,result;
	int c1, c2;
	pos=e.Find(_T('+'));
	a.Format(_T("%s"), e.Mid(0, pos));
	b.Format(_T("%s"), e.Mid(pos));
	c1=  _wtoi(a);
	c2 = _wtoi(b);
	result.Format(_T("%d+%d=%d"), c1, c2, c1 + c2);
	PrintRp(result);
}