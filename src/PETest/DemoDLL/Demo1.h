#pragma once
#include <string>

#define LIBRARY_EXPORTS

#ifdef LIBRARY_EXPORTS
#define DLLDEMO_API __declspec(dllexport)
#else
#define DLLDEMO_API __declspec(dllimport)
#endif

namespace PETest
{
	class Demo1
	{
	public:
		Demo1();
		~Demo1();

	private:
		int value;
		std::string name;
	};

	struct DemoStruct
	{
		int Index;
		float age;
		std::string addr;
	};

	void TestFunc1();

	void TestFunc2(int num, const std::string& s);

	extern "C" DLLDEMO_API int TestFunc3();
}