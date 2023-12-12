#include <iostream>
#include "test.h"

namespace CPP11
{
	void Test01()
	{
		{
			HasPtrMem a;
			HasPtrMem b(a);

			std::cout << a.d << std::endl;
			std::cout << b.d << std::endl;
		}
		std::cout << "HasPtrMem destruct error" << std::endl;
	}
}