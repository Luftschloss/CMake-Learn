#include "background_task.h"
#include <mutex>

using namespace std;

void ThreadFunc1()
{
	cout << "ThreadFunc1" << endl;
}

void ThreadFunc2(int p)
{
	cout << "ThreadFunc2  Input : "<< p << endl;
}

void ThreadWhile1()
{
	while (1)
	{
		cout<<"While Thread1 On"<< endl;
		_sleep(1000);
	}
}

void ThreadWhile2(int l)
{
	while (l > 0)
	{
		cout << "While Thread2 On " << this_thread::get_id() << endl;
		_sleep(1000);
		l--;
	}
}

mutex mtx1;
mutex mtx2;
int testNum = 1;

void MutexTest1(int n, char c)
{
	mtx1.lock();
	cout << "MutexTest1:";
	for (size_t i = 0; i < n; i++)
	{
		testNum = 1;
		cout << c << testNum << " ";
	}
	cout << n << endl;
	mtx1.unlock();
}

void MutexTest2(int n, char c)
{
	mtx2.lock();
	cout << "MutexTest2:";
	for (size_t i = 0; i < n; i++)
	{
		testNum = 2;
		cout << c << testNum << " ";
	}
	cout << n << endl;
	mtx2.unlock();
}

void ThreadTest()
{
	//thread first(ThreadWhile1);
	thread second(ThreadWhile2, 5);
	cout << "Main Thread" << endl;
	//first.join(); 
	//cout << "first thread finish" << endl;
	second.join();
	cout << "second thread finish" << endl;

	thread mtThread1(MutexTest1, 20, '*');
	thread mtThread2(MutexTest1, 20, '&');
	thread mtThread3(MutexTest2, 20, '%');
	mtThread1.join();
	mtThread2.join();
	mtThread3.join();
	cout << "threads over.\n";//±ØÐëjoinÍê³É

	int testN = 1;
	thread dThread([testN] {
		for (size_t i = 0; i < 100000; i++)
		{
			cout << testN;
		}
	});
	dThread.detach();
}

