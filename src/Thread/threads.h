#pragma once
#include <thread>
#include <stdexcept>

class scoped_thread
{
public:
	explicit scoped_thread(std::thread _t) :t(std::move(_t))
	{
		//保证是一个执行的线程
		if (!t.joinable())
			throw std::logic_error("No thread");
	}

	~scoped_thread() {
		//保证变量析构时线程执行完成
		t.join();
	}
	//保证thread只有一处构造函数赋值
	scoped_thread(scoped_thread const&) = delete;
	scoped_thread& operator=(scoped_thread const&) = delete;
private:
	std::thread t;
};