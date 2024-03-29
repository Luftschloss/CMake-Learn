#pragma once
#include <future>
#include <iostream>
#include <thread>

namespace future_test {

	int do_something() {
		std::cout << "do_something_start >>" << std::time(0) << std::endl;
		_sleep(5000);
		std::cout << "do_something_end" << std::endl;
		return 1;
	}

	void test1() {
		std::future<int> f = std::async([]() { return do_something(); });
		std::cout << "future get:" << f.get() << std::endl;
	}

	void test2()
	{
		std::cout << "f1 future star >>" << std::time(0) << std::endl;
		std::future<int> f1 = std::async(std::launch::deferred ,[]() { return do_something(); });
		std::cout << "f1 try get >>" << std::time(0) << std::endl;
		_sleep(2000);
		std::cout << "f1 future get:" << f1.get() << std::endl;

		std::cout << "f2 future start >>" << std::time(0) << std::endl;
		std::future<int> f2 = std::async(std::launch::async, []() { return do_something(); });
		std::cout << "f2 try get >>" << std::time(0) << std::endl;
		_sleep(2000);
		std::cout << "f2 future get:" << f2.get() << std::endl;
	}

	void test3() {
		std::promise<int> re;
		std::future<int> fu = re.get_future();
		std::thread th([r = std::move(re)]() mutable { r.set_value(do_something()); });
		std::cout << "thread started" << std::endl;

		int result = fu.get();
		std::cout << "get thread result : " << result << std::endl;
		th.join();
	}
}