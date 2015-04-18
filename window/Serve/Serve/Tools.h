#include "string"
#include "vector"
using namespace std;

namespace Tools{
	class STR{
		/*little tools : a string divided by a char
		  and it return a strArry
		*/
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
				if(j > i)substring = Seq.substr(i, j-i);//for a substring is ""
				strArray->push_back(substring);
				i = j;
			}
		}
	};
}