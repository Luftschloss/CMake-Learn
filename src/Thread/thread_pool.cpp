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
