#pragma once
#include <thread>
#include <numeric>
#include <vector>
#include <algorithm>
#include <functional>

template<typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);

    if (!length)
        return init;

    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    unsigned long const block_size = length / num_threads; 

    std::vector<T> results(num_threads);                //每个线程求和的结果
    std::vector<std::thread> threads(num_threads);      //每个Block分配一个线程

    Iterator block_start = first;
    for (unsigned long i = 0; i < num_threads; ++i)
    {
        Iterator block_end = block_start;
        if (i == (num_threads - 1))             //最后一块直接到末尾
            block_end = last;
        else
            std::advance(block_end, block_size); 
        threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));  //将Block的计算任务分配给线程
        block_start = block_end; 
    }
    
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));  //等待所有线程结束

    return std::accumulate(results.begin(), results.end(), init);
}