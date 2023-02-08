#include <iostream>
#include <thread>
#include "hello.h"
#include "world.h"
#include <string>

class background_task
{
public:
	void operator()() const
	{
		std::cout << "do_something" << std::endl;
		std::cout << "do_something_else" << std::endl;
	}
};

struct func
{
	int& i;
	func(int& i_) : i(i_) {}
	void operator() ()
	{
		for (unsigned j = 0; j < 100; ++j)
		{
			std::cout << "Do SomeThing" + std::to_string(i+j) << std::endl;; // 1 潜在访问隐患：悬空引用
		}
	}
};

void oops()
{
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread my_thread(my_func);
	//my_thread.detach(); // 2 不等待线程结束
	try
	{
		std::cout << "Test In Current" << std::endl;
	}
	catch (...)
	{
		my_thread.join(); // 1
		throw;
	}
	my_thread.join(); // 2
}

int* funcTestPtr(void)
{
	int num = 1234;
	/* ... */
	return &num;
}


int main()
{
	/*std::cout << "CmakeTest" << std::endl;
#ifdef USE_MACRO
		std::cout << "USE_MACRO" << std::endl;
#else
		std::cout << "UNUSE_MACRO" << std::endl;
#endif
	hello();
	std::cout << "thread_test1" << std::endl;

	std::thread t(world);

	t.join();

	std::cout << "thread_test2" << std::endl;*/

	//background_task f;
	//std::thread my_thread(f);
	//如果你传递了一个临时变量，而不是一个命名的变量；
	//C++编译器会将其解析为函数声明，而不是类型对象的定义。
	/*std::thread my_thread(background_task());
	
	std::thread my_thread{ background_task() };
	std::thread my_thread((background_task()));*/

	//oops();

	int* a = funcTestPtr();

	std::cout << *a << std::endl;

	a = funcTestPtr();

	std::cout << *a << std::endl;

	return 0;
}