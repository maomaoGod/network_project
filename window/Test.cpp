#include "Tools.h"

using namespace Tools;

int MainTest(){
	dll_test();
}
//hoe to use a class in a namespace
//set yourself fun's type
typedef int(*Int2Int)(int nVal1, int nval2);

void dll_test(){
	string path;
	string fun_name;
	Dlluser du ;
	du.Setpath(path);//set dll's path
	du.Load_dll();//load dll 
	du.Set_fun_name(fun_name);//set the fun
	Int2Int add = (Int2Int)du.Getfun();//get the pointer
	printf("%d\n",add(1,2));//just use it for fun
}