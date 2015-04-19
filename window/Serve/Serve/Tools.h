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
		static void Split(string Seq,vector<string> *strArray, char div){
			if (strArray->size() != 0){
				strArray->clear();
			}
			string substring;
			int i, j;
			int len = Seq.length;
			for (i = 0; i < len; i++){
				for (j = i; j < len; j++){
					if (Seq[j] == div) break;
				}
				/**<for a substring is*/
				if(j > i)substring = Seq.substr(i, j-i);
				strArray->push_back(substring);
				i = j;
			}
		}
	};
}
