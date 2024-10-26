#pragma once
#include <iostream>
#include <excpt.h>
#include <windows.h>   

namespace WindowsSEH
{
	int Funcensteinl()
	{
		int re = 0;
		while (re < 10)
		{
			__try {
				if (re == 2)
					continue;
				if (re == 3)
					break;
			}
			__finally {
				re++;
			}
			re++;
		}
		return re;
	}

	int Funcenstein2()
	{
		int re = 0;
		__try {
			re = 5;
			return re;
		}
		__finally {
			re = 50;
			return (100);
		}
		re = 20;
		return re;
	}

	int Funcenstein3(int a)
	{
		int re = 0;
		__try {
			if (a == 2)
				re = 1;
			else
				re = 2;
			__leave;
			re = 5;
		}
		__finally {
			//re = 3;
		}
		return re;
	}

	void ErrorFunc()
	{
		int a = 0;
		//malloc(5 / a);
	}

	int ExceptionCatch()
	{
		int a = 0;
		auto b = 0;
		__try {
			b = 10 / a;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			a = 2;
		}
		return b;
	}
}