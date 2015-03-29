//tools for other to use
#include <cstring>
#include "stdafx.h"
#pragma once

namespace Tools{
	//Dllusr
	class Dlluser{


	public :
		void Getpath(CString path){
			this->path = path;
		}

	private:
		CString path;
	};

	//get html 
	class Htmlget{
	public:
		void GetUrl(CString path){
			this->path = path;
		}

	private:
		CString path;

	};
}