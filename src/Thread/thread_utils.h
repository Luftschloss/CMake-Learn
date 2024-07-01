#pragma once
#include <thread>
#include <mutex>
#include <stack>
#include <queue>
#include <exception>

using namespace std;

struct empty_stack : exception
{
	const char* what() const throw();
};

template<typename T>
class thread_safe_stack
{
public:
	thread_safe_stack() {}
	thread_safe_stack(const thread_safe_stack& other) {
		lock_guard<mutex> lock(other.mut);
		data = other.data;
	}

	thread_safe_stack& operator=(const thread_safe_stack&) = delete;

	void push(T val)
	{
		lock_guard<mutex> lk(mut);
		data.push(move(val));
	}

	shared_ptr<T> pop()
	{
		lock_guard<mutex> lk(mut);
		if(data.empty()) throw empty_stack();
		shared_ptr<T> const re(make_shared<T>(move(data.top())));
		data.pop();
		return re;
	}

	void pop(T& val)
	{
		lock_guard<mutex> lk(mut);
		if (data.empty()) throw empty_stack();
		val = move(data.top());
		data.pop();	
	}

	bool empty() const {
		lock_guard<mutex> lk(mut);
		return data.empty();
	}

private:
	stack<T> data;
	mutable mutex mut;
};


template<typename T>
class thread_safe_queue
{
public:
	thread_safe_queue() {}
	thread_safe_queue(const thread_safe_queue&) {}
	thread_safe_queue& operator=(const thread_safe_queue&) = delete;

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

	bool empty() const
	{
		lock_guard<mutex> lk(mut);
		return data_queue.empty();
	}

private:

    mutable mutex mut;			// 因为empty是一个const成员函数
	queue<T> data_queue;
	condition_variable data_cond;

};

// 典型的无锁队列(Lock-Free Queue)算法有循环缓冲区（Circular Buffer）和链表（Linked List）等。
// 循环缓冲区通常使用两个指针（head 和 tail）来表示队列的开始和结束位置，利用自旋、CAS(Compare - and -Swap) 等原子操作来进行入队和出队操作。
// 链表则通过利用 CAS 操作插入或删除节点来实现并发访问
// 常见队列操作场景分为：单生产者——单消费者、多生产者——单消费者、单生产者——多消费者、多生产者——多消费者四种情况，队列数据可以是定长也可以是可变的

template<class T>
class RingBuffer
{
public:
	RingBuffer(size_t size) :m_size(size), m_front(0), m_rear(0)
	{
		m_data = new T[size];
	}

	~RingBuffer()
	{
		delete[] m_data;
		m_data = NULL;
	}

	inline bool IsEmpty() const
	{
		return m_front == m_rear;
	}

	inline bool IsFull() const
	{
		return m_front == (m_rear + 1) % m_size;
	}

	bool Push(const T& value)
	{
		if (IsFull())
			return false;
		m_data[m_rear] = value;
		m_rear = (m_rear + 1) % m_size;
		return true;
	}

	bool Push(const T* value)
	{
		if (IsFull())
			return false;
		m_data[m_rear] = *value;
		m_rear = (m_rear + 1) % m_size;
		return true;
	}

	inline bool Pop(T& value)
	{
		if (IsEmpty())
			return false;
		value = m_data[m_front];
		m_front = (m_front + 1) % m_size;
		return true;
	}

	inline size_t front() const
	{
		return m_front;
	}

	inline size_t rear() const
	{
		return m_rear;
	}

	inline size_t size() const
	{
		return m_size;
	}

private:
	size_t m_size;
	size_t m_front;
	size_t m_rear;
	T* m_data;
};