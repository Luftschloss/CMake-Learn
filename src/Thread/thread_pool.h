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
