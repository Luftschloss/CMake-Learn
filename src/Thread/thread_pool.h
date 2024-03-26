#pragma once
#include <atomic>
#include <thread>
#include <functional>
#include <future>
#include "thread_utils.h"


class FunctionWrapper
{
public:
	template<typename F>
	FunctionWrapper(F&& f) : impl(new impl_type<F>(std::move(f))) {}

	void operator()() { impl->call(); }

	FunctionWrapper() = default;

	FunctionWrapper(FunctionWrapper&& other) : impl(std::move(other.impl)) {}

	FunctionWrapper& operator=(FunctionWrapper&& other)
	{
		impl = std::move(other.impl);
		return *this;
	}

	FunctionWrapper(const FunctionWrapper& other) = delete;
	FunctionWrapper(FunctionWrapper& other) = delete;
	FunctionWrapper& operator=(const FunctionWrapper&) = delete;

private:
	struct impl_base
	{
		virtual void call() = 0;
		virtual ~impl_base() {}
	};

	std::unique_ptr<impl_base> impl;

	template<typename F>
	struct impl_type : impl_base
	{
		F f;
		impl_type(F&& f_) : f(std::move(f_)) {}
		void call() { f(); }
	};
};

template<typename Iterator, typename T>
T ParallelAccumulate(Iterator first, Iterator last, T init)
{
	unsigned long const length = std::distance(first, last);

	if (!length)
		return init;

	unsigned long const block_size = 25;
	unsigned long const num_blocks = (length + block_size - 1) / block_size;  // 1

	std::vector<std::future<T> > futures(num_blocks - 1);
	thread_pool pool;

	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_blocks - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		futures[i] = pool.submit(accumulate_block<Iterator, T>());  // 2
		block_start = block_end;
	}
	T last_result = accumulate_block<Iterator, T>()(block_start, last);
	T result = init;
	for (unsigned long i = 0; i < (num_blocks - 1); ++i)
	{
		result += futures[i].get();
	}
	result += last_result;
	return result;
}


class thread_pool
{
public:

	thread_pool();

	~thread_pool()
	{
		done = true;
	}

	template<typename FunctionType>
	std::future<typename std::result_of<FunctionType()>> Submit(FunctionType f)
	{
		typedef typename std::result_of<FunctionType()>::type result_type;
		std::packaged_task<result_type()> task(std::move(f));
		std::future<result_type> res(task.get_future());
		work_queue.push(std::move(task));
		return res;
	}

private:
	void WorkerThread();
	void RunPendingTask();

	std::atomic<bool> done;
	thread_safe_queue<FunctionWrapper> work_queue;
	std::vector<std::thread> threads;

};
