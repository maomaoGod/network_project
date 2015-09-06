/**@file
*@brief 一些函数的聚集
*@author ACM1201
*@date 2015/04/19
*@version 1.1
*@note
*a string divided by a char and it return a strArry
*/
#pragma once
#include <string>
#include "vector"
#include "time.h"
#include <sstream>
#include <sys/stat.h>
#include <io.h>
#include <fstream>
#define BUFSIZE 1024
using namespace std;
/**@file
*@brief 一些函数的具体实现
*@author ACM1201
*@date 2015/04/19
*@version 1.1
*@note
*/
namespace Tools{

	class STR{
	public:
		/**
		* @brief
		* @param [in] <s> string型
		* @author ACM2012
		* @return 不包含小写字母的字符串
		* @note
		*  如果字符串中有小写字母，则将该小写字母改为大写字母
		*/
		static string Upper(string s){
			int len = s.length();
			for (int i = 0; i < len; i++){
				if (s[i] >= 'a'&&s[i] <= 'z')
					s[i] = s[i] + 'A' - 'a';
			}
			return s;
		}
		/**
		* @brief
		* @param [in] <s> string型
		* @author ACM2012
		* @return 不包含大写字母的字符串
		* @note
		*  如果字符串中有大写字母，则将该大写字母改为小写字母
		*/
		static string lower(string s)
		{
			for (string::iterator it = s.begin(); it < s.end(); it++)
			if (*it >= 'A'&&*it <= 'Z')
				*it = *it + 'a' - 'A';
			return s;
		}
		/**
		* @brief
		* @param [in] <d> TCHAR型
		* @param [in] <len> int型
		* @author ACM2012
		* @return CString型数据
		* @note
		*  如果字符串的长度不足len，则在其后填充字符d，直到长度为len
		*/
		static CString FillStr(TCHAR d, int len){
			int i;
			CString str = _T("");
			for (i = 0; i < len; i++){
				str += d;
			}
			return str;
		}
		/**
		* @brief
		* @param [in] <Subject> string型
		* @author ACM2012
		* @return 字符型数组的指针
		* @note
		*  将字符串中每个字符依序放入字符型数组
		*/
		static char *S2Cstar(string Subject)
		{
			int len = Subject.length() + 1, i;
			char *temp = (char *)malloc(len*sizeof(char));
			for (i = 0; i < Subject.length(); ++i)
			{
				temp[i] = Subject[i];
			}
			temp[i] = '\0';
			return temp;
		}


		static void Turn(){

		}
		/**
		* @brief
		* @param [in] <str> string型
		* @param [in] <ans> vector<string> *型
		* @param [in] <pattern> string型
		* @author ACM2012
		* @return 无
		* @note
		*  将输入的命令以空格划分成单个字符串
		*/
		static void Split(string str, vector<string> *ans, string pattern){
			ans->clear();
			string::size_type pos;
			str += pattern;
			int len = str.size();
			int i;
			for (i = 0; i < len; i++){
				pos = str.find(pattern, i);
				if (pos < len){
					ans->push_back(str.substr(i, pos - i));
					i = pos + pattern.size() - 1;
				}
			}
		}
		/**
		* @brief
		* @param [in] <look> string型
		* @param [in] <rm> char型
		* @author ACM2012
		* @return string数据
		* @note
		*  去掉字符串look中的rm字符
		*/
		static string RemoveStr(string look, char rm){
			string s = look;
			int i, j, len = look.length();
			for (i = 0, j = 0; i < len; i++){
				if (look[i] == rm) continue;
				else{
					s[j++] = look[i];
				}
			}
			s[j] = '\0';
			return s;
		}
		/**
		* @brief
		* @param [in] <point> TCHAR **型
		* @param [in] <msg> CString *型
		* @author ACM2012
		* @return int数据
		* @note
		*  将字符串转化为TCHAR型
		*/
		static int CString2TCHAR(TCHAR **point, CString *msg){
			int len = msg->GetLength();
			(*point) = (TCHAR *)malloc(sizeof(TCHAR)*len);
			for (int i = 0; i < len; i++){
				*(*point + i) = (*msg)[i];
			}
			*(*point + len) = _T('\0');
			return len;
		}
		//command & args
		/**
		* @brief
		* @param [in] <strArry> CStringArray *型
		* @param [in] <Cmd> CString型
		* @param [in] <div> TCHAR型
		* @author ACM2012
		* @return 无
		* @note
		*  将输入的命令存入数组
		*/
		static void CCarg(CStringArray *strArry, CString Cmd, TCHAR div){
			int i = 0;
			CString strGet(_T(""));
			//CStringArray strArry;
			while (AfxExtractSubString(strGet, Cmd, i++, div)){
				strArry->Add(strGet);
			}
			//return strArry;
		}
		/**
		* @brief
		* @param [in] <string> Seq型
		* @param [in] <strArray> vector<string> *型
		* @param [in] <div> char型
		* @author ACM2012
		* @return 无
		* @note
		*  将输入的命令以空格划分成单个字符串
		*/
		static void Split(string Seq, vector<string> *strArray, char div){
			if (strArray->size() != 0){
				strArray->clear();
			}
			string substring;
			int i, j;
			int len = Seq.length();
			for (i = 0; i < len; i++){
				for (j = i; j < len; j++){
					if (Seq[j] == div) break;
				}
				/**<for a substring is*/
				if (j > i){
					substring = Seq.substr(i, j - i);
					strArray->push_back(substring);
				}
				i = j;
			}
		}
		/**
		* @brief
		* @param [in] <str> string型
		* @author ACM2012
		* @return LPCWSTR数据
		* @note
		*  将字符串转化为LPCWSTR型
		*/
		static LPCWSTR String2LPCWSTR(string str){
			/*wstring w_str = ANSIToUnicode(str);
			LPCWSTR L_str = w_str.c_str();
			return L_str;*/
			size_t origsize = str.length() + 1;
			const size_t newsize = 100;
			size_t convertedChars = 0;
			wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(str.length() - 1));
			mbstowcs_s(&convertedChars, wcstring, origsize, str.c_str(), _TRUNCATE);
			return wcstring;
		}
		/**
		* @brief
		* @param [in] <Subject> CString型
		* @author ACM2012
		* @return LPSTR数据
		* @note
		*  将CString数据转化为LPSTR型
		*/
		static LPSTR CString2LPSTR(CString Subject){
			LPSTR sub = (LPSTR)Subject.GetBuffer();
			Subject.ReleaseBuffer();
			return sub;
		}
		/**
		* @brief
		* @param [in] <i> int型
		* @author ACM2012
		* @return string数据
		* @note
		*  将int数据转化为string型
		*/
		static string int2string(int i){
			string temp;
			stringstream ss;
			ss << i;
			ss >> temp;
			return temp;
		}
		/**
		* @brief
		* @param [in] <s> string型
		* @author ACM2012
		* @return string数据
		* @note
		*  将string数据转化为int型
		*/
		static int string2int(string s){
			int temp;
			stringstream ss;
			ss << s;
			ss >> temp;
			return temp;
		}
		/**
		* @brief
		* @param [in] <look> string型
		* @param [in] <rm> char型
		* @author ACM2012
		* @return string数据
		* @note
		*  去掉字符串look中的rm字符
		*/
		static string Clearstr(string look, char rm)
		{
			string::iterator it;
			for (it = look.begin(); it != look.end(); ++it)
			{
				if (*it == rm)
				{
					look.erase(it);
				}
			}
			return look;
		}
		/**
		* @brief
		* @param [in] <smsg> TCHAR *型
		* @param [in] <rm> char型
		* @author ACM2012
		* @return string数据
		* @note
		*  将TCHAR *数据转化为string型
		*/
		static string Tchar2string(TCHAR *smsg){
			int iLen = WideCharToMultiByte(CP_ACP, 0, smsg, -1, NULL, 0, NULL, NULL);
			char* chRtn = new char[iLen*sizeof(char)];
			WideCharToMultiByte(CP_ACP, 0, smsg, -1, chRtn, iLen, NULL, NULL);
			string str(chRtn);
			return str;
		}
		/**
		* @brief
		* @param [in] <Subject> CString型
		* @author ACM2012
		* @return string数据
		* @note
		*  将CString数据转化为string型
		*/
		static string CS2S(CString Subject){
			string sub = Tchar2string(Subject.GetBuffer());//Subject.GetBuffer(0);
			Subject.ReleaseBuffer();
			return sub;
		}
		/**
		* @brief
		* @param [in] <str> string&型
		* @author ACM2012
		* @return wstring数据
		* @note
		*  将ansit数据转化为unicode型
		*/
		static wstring ANSIToUnicode(const string& str)
		{
			int len = 0;
			len = str.length();
			int  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
			wchar_t *  pUnicode;
			pUnicode = new  wchar_t[unicodeLen + 1];
			memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
			::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
			wstring  rt;
			rt = (wchar_t*)pUnicode;
			delete  pUnicode;
			return  rt;
		}
		/**
		* @brief
		* @param [in] <Subject> string型
		* @author ACM2012
		* @return CString数据
		* @note
		*  将string数据转化为CString型
		*/
		static CString S2CS(string Subject){
			int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)(Subject.c_str()), -1, NULL, 0);
			TCHAR *pBuffer = new TCHAR[nBufferSize + 1];
			memset(pBuffer, 0, (nBufferSize + 1)*sizeof(TCHAR));
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)(LPCSTR)(Subject.c_str()), -1, pBuffer, nBufferSize*sizeof(TCHAR));//UTF-8תUnicode
			CString myhtml = pBuffer;
			return myhtml;
		}

	};

	/**
	*@class Timer Tools.h "HustNet/Tools.h"
	*@brief 计时器
	*@author  ACM2012
	*@note
	* 计时器
	*/
	class Timer{
	public:
		///<output style: xx-xx-xx xx::xx::xx
		static string GetCurtime(){
			string str, temp;
			stringstream ss;
			time_t now;
			struct tm timeinfo;
			time(&now);
			localtime_s(&timeinfo, &now);
			int year, month, day;
			year = timeinfo.tm_year + 1900;
			month = timeinfo.tm_mon + 1;
			day = timeinfo.tm_mday;
			int hour, min, sec, weekday;
			hour = timeinfo.tm_hour;
			min = timeinfo.tm_min;
			sec = timeinfo.tm_sec;
			weekday = timeinfo.tm_wday;
			str += STR::int2string(year);
			str += ("-" + STR::int2string(month));
			str += ("-" + STR::int2string(day));
			str += (" " + STR::int2string(hour));
			str += ("::" + STR::int2string(min));
			str += ("::" + STR::int2string(sec));
			str += ("/" + STR::int2string(weekday));
			return str;
		}
		/**
		* @brief
		* @param [in] <path> string型
		* @author ACM2012
		* @return string数据
		* @note
		*  记录文件最后一次修改的时间
		*/
		static string FilelastChange(string path){
			string *str;
			struct _stat buf;
			int result;
			char buffur[128];
			result = _stat(path.c_str(), &buf);
			if (!result){
				ctime_s(buffur, 128, &buf.st_mtime);
				str = new string(buffur);
			}
			return *str;
		}
	};

	class FIO{
	public:
		/**
		* @brief
		* @param [in] <path> string型
		* @author ACM2012
		* @return string数据
		* @note
		*  读取文件
		*/
		static string ReadFullFile(string path){
			/*
			char s;
			string read = "";
			FILE *fp;
			fopen_s(&fp, path.c_str(),"r");
			while (s = fgetc(fp) != EOF){
			read += s;
			}
			fclose(fp);
			return read;*/
			ifstream in(path.c_str(), ios::in);
			string temp = "", s = "";
			string *out;
			int len = 0;
//			char c;
			while (true){
				if (in.eof()){
					out = new string(s);
					break;
				}
				getline(in, temp);
				s += temp + "\r\n";
			}
			//s += '\0';
			in.close();
			return *out;
		}

		/**
		* @brief
		* @param [in] <path> string型
		* @param [in] <mark> int型
		* @author ACM2012
		* @return string数据
		* @note
		*  读取文件
		*/
		static string ReadFile(string path, int mark){
			FILE *fp;
			string *temp;
			char t[1024];
			string str = "";
			fopen_s(&fp, path.c_str(), "r");
			while (fscanf_s(fp, "%s", t, 1024) != -1){
				temp = new string(t);
				str += *temp;
				if (mark) str += '\n';
				free(temp);
			}
			fclose(fp);
			return str;
		}
		/**
		* @brief
		* @param [in] <path> string型
		* @author ACM2012
		* @return int型数据，表示文件的大小
		* @note
		*  计算读取文件的大小
		*/
		static int file_size(string path){
			FILE *fp;
			fopen_s(&fp, path.c_str(), "r");
			fseek(fp, 0L, SEEK_END);
			int size = ftell(fp);
			fclose(fp);

			return size;
		}
		/**
		* @brief
		* @param [in] <path> string型
		* @author ACM2012
		* @return string型数据，表示文件的大小
		* @note
		*  读取大文件
		*/
		static string ReadBigfile(string path){
			ifstream in;
			int len = file_size(path);
			in.open(path.c_str(), ios::in);
			char *c;
			c = (char *)malloc(sizeof(char)*len);
			in.read(c, len);
			in.close();
			return (*new string(c));
		}
		/**
		* @brief
		* @param [in] <path> string型
		* @author ACM2012
		* @return 无
		* @note
		*  删除文件
		*/
		static void DelFile(string path){
			remove(path.c_str());
		}
		/**
		* @brief
		* @param [in] <path> string型
		* @param [in] <data> string *型
		* @author ACM2012
		* @return 无
		* @note
		*  保存文件
		*/
		static void SaveFile(string path, string *data){
			FILE *fp;
			vector<string> d;
			STR::Split(*data, &d, '\n');
			if (Exist(path)){ DelFile(path); }
			fopen_s(&fp, path.c_str(), "w");
			for (int i = 0; i < d.size(); i++)
				fprintf_s(fp, "%s\n", d[i].c_str());
			fclose(fp);
		}
		/**
		* @brief
		* @param [in] <path> string型
		* @param [in] <data> string *型
		* @author ACM2012
		* @return 无
		* @note
		*  判断是否存在该文件，如果不存在，返回0；否则，返回1
		*/
		static int Exist(string path){
			return (_access(path.c_str(), 0) == 0);
		}
	};

	/**
	*@class MYFILE Tools.h "HustNet/Tools.h"
	*@brief 对指定文件夹下的文件进行处理
	*@author  ACM2012
	*@note
	*/
	class MYFILE{
	public:
		static void myDeleteDirectory(CString directory_path)   ///<删除一个文件夹下的所有内容  
		{
			CFileFind finder;
			CString path;
			path.Format(_T("%s/*.*"), directory_path);
			BOOL bWorking = finder.FindFile(path);
			while (bWorking){
				bWorking = finder.FindNextFile();
				if (finder.IsDirectory() && !finder.IsDots()){///<处理文件夹  
					myDeleteDirectory(finder.GetFilePath()); ///<递归删除文件夹  
					RemoveDirectory(finder.GetFilePath());
				}
				else{///<处理文件  
					DeleteFile(finder.GetFilePath());
				}
			}
		}
	};
}
