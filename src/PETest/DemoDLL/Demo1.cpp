#include "Demo1.h"
#include <iostream>

namespace PETest
{
	Demo1::Demo1()
	{
		value = 255;
		name = "Demo";
	}

	Demo1::~Demo1()
	{
	}


	void TestFunc1()
	{
		std::cout << "TextureFunc1" << std::endl;
	}

	void TestFunc2(int num, const std::string& s)
	{
		std::cout << "TestFunc2 ==> Index :" << num << "String :" << s << std::endl;
	}

	int TestFunc3()
	{
		return 255;
	}

}
