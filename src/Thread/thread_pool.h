#pragma once
#include <atomic>
#include <thread>
#include <thread_utils.h>

class thread_pool
{
	
	std::atomic<bool> done;
};