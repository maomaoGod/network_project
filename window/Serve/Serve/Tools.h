/**@file
 *@brief a little tool
 *@author xinYao,jinxiaFang
 *@date 2015/04/19
 *@version XXXXXX
 *@note
 *a string divided by a char and it return a strArry
 */
#include "string"
#include "vector"
using namespace std;

namespace Tools{
	class STR{
	public:
		static string lower(string s)
		{
			for (string::iterator it = s.begin(); it < s.end(); it++)
			if (*it >= 'A'&&*it <= 'Z')
				*it = *it + 'a' - 'A';
			return s;
		}
		static void Split(string Seq,vector<string> *strArray, char div){
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

		static LPSTR CString2LPSTR(CString Subject){
			LPSTR sub = (LPSTR)Subject.GetBuffer();
			Subject.ReleaseBuffer();
			return sub;
		}

		static string Tchar2string(TCHAR *smsg){
			int iLen = WideCharToMultiByte(CP_ACP, 0, smsg, -1, NULL, 0, NULL, NULL);
			char* chRtn = new char[iLen*sizeof(char)];
			WideCharToMultiByte(CP_ACP, 0, smsg, -1, chRtn, iLen, NULL, NULL);
			string str(chRtn);
			return str;
		}

		static string CS2S(CString Subject){
			string sub = Tchar2string(Subject.GetBuffer());//Subject.GetBuffer(0);
			Subject.ReleaseBuffer();
			return sub;
		}

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

		static CString S2CS(string Subject){
			int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)(Subject.c_str()), -1, NULL, 0);
			TCHAR *pBuffer = new TCHAR[nBufferSize + 1];
			memset(pBuffer, 0, (nBufferSize + 1)*sizeof(TCHAR));
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)(LPCSTR)(Subject.c_str()), -1, pBuffer, nBufferSize*sizeof(TCHAR));//UTF-8תUnicode
			CString myhtml = pBuffer;
			return myhtml;
		}

	};
	
	class MYFILE{
	public:
		static void myDeleteDirectory(CString directory_path)   //删除一个文件夹下的所有内容  
		{
			CFileFind finder;
			CString path;
			path.Format(_T("%s/*.*"), directory_path);
			BOOL bWorking = finder.FindFile(path);
			while (bWorking){
				bWorking = finder.FindNextFile();
				if (finder.IsDirectory() && !finder.IsDots()){//处理文件夹  
					myDeleteDirectory(finder.GetFilePath()); //递归删除文件夹  
					RemoveDirectory(finder.GetFilePath());
				}
				else{//处理文件  
					DeleteFile(finder.GetFilePath());
				}
			}
		}
	};
}
