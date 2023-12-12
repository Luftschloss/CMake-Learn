#pragma once
#include <thread>
#include <mutex>
#include <queue>

using namespace std;

template<typename T>
class thread_safe_queue
{
public:
	thread_safe_queue() {}

	void push(T val)
	{
		lock_guard<mutex> lk(mut);
		data_queue.push(move(val));
		data_cond.notify_one();
	}

	void wait_and_pop(T& val)
	{
		unique_lock<mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		val = move(data_queue.front());
		data_queue.pop();
	}

	shared_ptr<T> wait_and_pop()
	{
		unique_lock<mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		shared_ptr<T> re(make_shared<T>(move(data_queue.front())));
		data_queue.pop();
		return re;
	}

	bool try_pop(T& val)
	{
		lock_guard<mutex> lk(mut);
		if (data_queue.empty())
			return false;
		val = move(data_queue.front());
		data_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		lock_guard<T> lk(mut);
		if (data_queue.empty())
			return make_shared<T>();
		shared_ptr<T> re(make_shared<T>(move(data_queue.front())));
		data_queue.pop();
		return re;
	}

	bool empty()
	{
		lock_guard<mutex> lk(mut);
		return data_queue.empty();
	}

private:

	struct node
	{
		shared_ptr<T> data;
		unique_ptr<mode> next;
	};

	mutable mutex mut;
	queue<T> data_queue;
	condition_variable data_cond;

};