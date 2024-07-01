#include "thread_pool.h"

thread_pool::thread_pool()
{
	done = false;
	unsigned const thread_count = std::thread::hardware_concurrency();
	try
	{
		for (size_t i = 0; i < thread_count; i++)
			threads.push_back(std::thread(&thread_pool::WorkerThread, this));
	}
	catch (...)
	{
		done = true;
		throw;
	}
}

void thread_pool::WorkerThread()
{
	while (!done)
	{
		FunctionWrapper task;
		if (work_queue.try_pop(task))
		{
			task();
		}
		else
		{
			std::this_thread::yield();
		}
	}
}

void thread_pool::RunPendingTask()
{
	FunctionWrapper task;
	if (work_queue.try_pop(task))
	{
		task();
	}
	else
	{
		std::this_thread::yield();
	}
}


ThreadPool::ThreadPool(size_t size): stop(false)
{
	for (size_t i = 0; i < size; i++)
	{
		m_workers.emplace_back(
			[this] {
			for (;;)
			{
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(m_queue_mutex);
					m_condition.wait(lock,[this] {return stop || !m_tasks.empty(); });
					if(stop && m_tasks.empty())
						return;
					task = std::move(m_tasks.front());
					m_tasks.pop();
				}
				task();
			}
		});
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::lock_guard<std::mutex> lock(m_queue_mutex);
		stop = true;
	}
	m_condition.notify_all();
	for (auto& worker : m_workers)
		worker.join();
}

template<class F, class ...Args>
auto ThreadPool::Enqueue(F&& f, Args && ...args) -> std::future<typename std::result_of<F(Args ...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;
	auto task = std::make_shared<std::packaged_task<return_type()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...));

	std::future<return_type> res = task->get_future();
	{
		std::lock_guard<std::mutex> lock(m_queue_mutex);
		if (stop)
			throw std::runtime_error("enqueue on stopped ThreadPool");

		// 将封装了 lambda 表达式（调用了 task ）的 function 对象存入队列当中
		m_tasks.emplace([task]() { (*task)(); });
	}
	///< 通知一个等待条件变量的子线程
	m_condition.notify_one();
	return res;
}
