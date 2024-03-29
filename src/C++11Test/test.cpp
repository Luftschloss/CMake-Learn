#include <iostream>
#include "test.h"
#include <regex>
#include <queue>
#include <cstdarg>

namespace CPP11
{
	void Test01()
	{
		/*{
			HasPtrMem a;
			HasPtrMem b(a);

			std::cout << a.d << std::endl;
			std::cout << b.d << std::endl;
		}*/
		std::cout << "HasPtrMem destruct error" << std::endl;

		std::cout << "smart ptr test" << std::endl;
		{
			auto ptr1 = std::make_shared<SmartPtr>(2.0f);
			{
				auto ptr2 = std::make_shared<SmartPtr>(2.0f, "test");
			}
			auto ptr3 = std::make_shared<SmartPtr>(ptr1);
		}

		TestUniquePtr();

		std::cout << "smart ptr test end" << std::endl;
	}



	void TestUniquePtr()
	{
		auto ut1 = UniqueT::MakeOne("P1");
		std::cout << ut1->GetName() << std::endl;
		auto ut2 = std::move(ut1);
		std::cout << ut2->GetName() << std::endl;

		auto ut3 = UniqueT::MakeOne("P2");
		std::cout << ut3->GetName() << std::endl;

	}

	void PtrTest1()
	{
		int* p;				// 指针未初始化，此时 p 为野指针
		int* pi = nullptr;

		{
			int i = 6;
			pi = &i;		// 此时 pi 指向一个正常的地址
			*pi = 8;		// ok
		}

		*pi = 6;			// 由于 pi 指向的变量 i 已经销毁，此时 pi 即成了悬空指针
	}

	

	void PtrTest2()
	{
		data1 data{};
		auto const& d1 = data.Get()[1];
		auto const& d2 = data.Get()[2];

		auto d3 = data.GetRef()[1];
		auto d4 = data.GetRef()[2];

		std::cout << "d1:" << d1 << ", d2:" << d2 << "; d3:" << d3 << ", d4:" << d4 << std::endl;
	}

	int TestVa_List(int count, ...)
	{
		int sum = 0;
		va_list args;
		va_start(args, count);

		for (size_t i = 0; i < count; i++)
		{
			sum += va_arg(args, int);
		}

		va_end(args);

		return sum;
	}

	namespace Regex
	{

		void Test01()
		{
			std::string input = "Thread-9-14446   (  14378) [005] .... 105302.236372: tracing_mark_write: B|14378|QueuePresentKHR";
			std::string input2 = "UnityGfxDevice-5062  ( 4702) [007] ...1   722.085916: tracing_mark_write: B|4702|eglSwapBuffersWithDamageKHR";
			//std::regex pattern("(\\d+\\.\\d+):\\s+tracing_mark_write:\\s+(\\w+).*VSYNC-app\\|([0-9]+)");
			const std::regex pattern("(\\d+\\.\\d+):\\s+tracing_mark_write:\\s+(\\w+)\\|(\\d+)\\|");
			std::smatch matches;

			if (std::regex_search(input, matches, pattern)) {
				std::string time = matches[1].str();
				std::string letter = matches[2].str();
				std::string number = matches[3].str();

				std::cout << "Time: " << time << std::endl;
				std::cout << "Letter: " << letter << std::endl;
				std::cout << "Number: " << number << std::endl;
			}

		}

		void Test02()
		{
			std::string input = "UnityGfxDeviceW-21380 (-----) [004] .... 2109942.311648: tracing_mark_write: B|21320|eglSwapBuffersWithDamageKHR";
			std::regex pattern("(\\d+\\.\\d+):\\s+tracing_mark_write:\\s+(\\w+)\\|(\\d+)\\|.*eglSwapBuffersWithDamageKHR");

			std::smatch matches;
			if (std::regex_search(input, matches, pattern)) {
				std::string letter = matches[1].str();
				std::string number = matches[2].str();

				std::cout << "Letter: " << letter << std::endl;
				std::cout << "Number: " << number << std::endl;
			}
		}
	}

	namespace Container
	{
		void QueueTest()
		{
			std::queue<int> a;
			a.push(1);
			a.push(5);
			a.push(9);
			a.push(2);

			auto t1 = a.front();
			auto t2 = a.back();

			a.pop();

			auto t3 = a.front();
		}
	}
}
