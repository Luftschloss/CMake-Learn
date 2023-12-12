#pragma once
#include <future>
#include <iostream>
#include <thread>

namespace future_test {

	int do_something() {
		std::cout << "do_something_start" << std::endl;
		_sleep(5000);
		std::cout << "do_something_end" << std::endl;
		return 1;
	}

	void test1() {
		std::future<int> f = std::async([]() { return do_something(); });
		std::cout << "future started" << std::endl;
		std::cout << "future get:" << f.get() << std::endl;
	}

	void test2(){
		std::promise<int> re;
		std::future<int> fu = re.get_future();
		std::thread th([r = std::move(re)]() mutable { r.set_value(do_something()); });
		std::cout << "thread started" << std::endl;

		int result = fu.get();
		std::cout << "get thread result : " << result << std::endl;
		th.join();
	}
}