
#pragma once
#include "stdafx.h"
#include "Serve.h"
#include "FTPClientSocket.h"
#include "Network.h"
#include <map>

using namespace std;
using namespace NetWork;
extern void PrintView(CString e);
extern map <FTPClientSocket *, int> myclient;

// FTPClientSocket

FTPClientSocket::FTPClientSocket()
{
	ftpworker.user.mark = false;
}

FTPClientSocket::~FTPClientSocket()
{
}

AppLayerFtp app;
// FTPClientSocket 成员函数
void FTPClientSocket::OnReceive(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (nErrorCode == 0){
		CString result=_T(""), back;
		//memset(Buffer, 0, sizeof(Buffer));
		//Receive(Buffer, BUFSIZE);//来自客户端%d的消息: myclient[this],
		//result.Format(_T("%s"), Buffer);
		CString temp;
		//Receive(data, sizeof(HttpRMsg));
		//Receive(Buffer, MAXLEN);//来自客户端%d的消息: myclient[this],
		//data.Format(_T("%s"), Buffer);
		int count;
		while (true){
			count = 0;
			memset(Buffer, 0, sizeof(Buffer));
			while (true){
			    //Receive(Buffer, BUFSIZE);while (true){
			    count += Receive((void *)(Buffer + count), (BUFSIZE - count));
			    if (count == BUFSIZE) break;
		    }
			temp.Format(_T("%s"), Buffer);
		    string s = STR::RemoveStr(STR::CS2S(temp), '\n');
		    if (s[0]=='\0') 
				break;
			result += temp;
		}
		string msg = STR::CS2S(result);
		ftpworker.setMsg(msg);
		int error = ftpworker.dealwith();
		ftpworker.checkMsg();
		back = STR::S2CS(ftpworker.getMsg());
		
		/*
		if (error == 2){
			if (ftpworker.getWay() == 2){
				dataSocket = new FTPDataSocket();
				dataSocket->Create(ftpworker.GetPort());
				dataSocket->Listen();
			}
			else{
			}
		}*/
		
		//Send(back, back.GetLength()*sizeof(TCHAR));

		TCHAR *senddata;
		int len = STR::CString2TCHAR(&senddata, &back);
		//aSocket->Send(data, data.GetLength()*sizeof(TCHAR));
		int i;
		for (i = 0; i < len; i += BUFSIZE){
			if (i + BUFSIZE < len)
				Send(senddata + i, BUFSIZE*sizeof(TCHAR));
			else{
				CString str = senddata + i;
				str += STR::FillStr(_T('\n'), BUFSIZE - (len % BUFSIZE));
				Send(str, BUFSIZE*sizeof(TCHAR));
			}
		}
		//back = _T("ftp res end");
		//Send(back, back.GetLength()*sizeof(TCHAR));

		/*if (error == 3){
			if (ftpworker.getWay() == 1){//port that say ,we send to
				dataSend = new CSocket();
			    dataSend->Create();
				TCHAR buf[1024];
				memset(buf, 0, sizeof(buf));//STR::S2CS(ftpworker.getIP())
				if (dataSend->Connect(STR::S2CS("127.0.0.1"), ftpworker.GetPort())){
					dataSend->Receive((void *)buf, 1024);
				}
				else{
					dataSend->GetLastError();
				}
				memset(buf, 0, sizeof(buf));
				CString sendmsg = _T("null");
				if (ftpworker.getStyle()){//download
					dataSend->Send(sendmsg, sizeof(TCHAR)*sendmsg.GetLength());
					CString rev;
					CString temp;
					while (dataSend->Receive((void *)buf, 1024)){
						temp.Format(_T("%s"), buf);
						rev += temp;
						memset(buf, 0, 1024 * sizeof(TCHAR));
					}
					FIO::SaveFile(ftpworker.getDes(), &STR::CS2S(rev));
				}
				else{//upload
					sendmsg = STR::S2CS(FIO::ReadFile(ftpworker.getSrc(), 1));
					dataSend->Send(sendmsg, sizeof(TCHAR)*sendmsg.GetLength());
					dataSend->Receive((void *)buf, 1024);
				}
				dataSend->Close();
				delete dataSend;
			}
			else{
				dataSocket->src_path = ftpworker.getSrc();
				dataSocket->des_path = ftpworker.getDes();
				dataSocket->style = ftpworker.getStyle();
				dataSocket->good = true;
			}
		}*/
		if(ftpworker.user.mark) 
			PrintView(ftpworker.show_msg());
		//Exit the Socket
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void FTPClientSocket::OnSend(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString back;
	back.Format(_T("已连接到服务器"), myclient[this] + 1);
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void FTPClientSocket::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString log;
	log.Format(_T("客户端%d已断开连接"), myclient[this]+1);
	PrintView(log);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
