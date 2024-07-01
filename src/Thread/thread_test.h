#pragma once
#include <iostream>
#include <thread>

void ThreadFunc1();
void ThreadFunc2(int p);

void ThreadWhile1();
void ThreadWhile2(int l);

void MutexTest1(int n, char c);
void MutexTest2(int n, char c);

void ThreadTest();

void MoveThread();

class ThreadX
{
public:
	void do_work(int a)
	{
		std::cout << "ThreadX do_work : start \t" << a << std::endl;
		a += 10;
		_sleep(a);
		std::cout << "ThreadX do_work : end \t" << a << std::endl;
	}
};

void ThreadParam1();

void ThreadTest2();

void ThreadCondition1();

void RingBufferTest();