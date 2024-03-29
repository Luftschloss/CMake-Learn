#pragma once
#include <thread>
#include <stdexcept>

class scoped_thread
{
public:
	explicit scoped_thread(std::thread _t) :t(std::move(_t))
	{
		//��֤��һ��ִ�е��߳�
		if (!t.joinable())
			throw std::logic_error("No thread");
	}

	~scoped_thread() {
		//��֤��������ʱ�߳�ִ�����
		t.join();
	}
	//��֤threadֻ��һ�����캯����ֵ
	scoped_thread(scoped_thread const&) = delete;
	scoped_thread& operator=(scoped_thread const&) = delete;
private:
	std::thread t;
};