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
	cout << "MutexTest1====>Start" << endl;
	mtx1.lock();
	for (size_t i = 0; i < n; i++)
	{
		testNum = 1;
		cout << c << testNum << " ";
	}
	cout << n << endl;
	mtx1.unlock();
	cout << "MutexTest1====>Done" << endl;
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

mutex lock1;
mutex lock2;

/// <summary>
/// 环路资源等待死锁，梳理两个线程获取资源的顺序可解决
/// </summary>
void MutexLockTest()
{
	thread t1([] {
		cout << "Thread 1 wait res1" << endl;
		lock1.lock();
		cout << "Thread 1 get res1" << endl;
		_sleep(1);
		cout << "Thread 1 wait res2" << endl;
		lock2.lock();
		cout << "Thread 1 get res2" << endl;
		lock2.unlock();
		lock1.unlock();
	});

	thread t2([] {
		cout << "Thread 2 wait res2" << endl;
		lock2.lock();
		cout << "Thread 2 get res2" << endl;
		_sleep(1);
		cout << "Thread 2 wait res1" << endl;
		lock1.lock();
		cout << "Thread 2 get res1" << endl;
		lock1.unlock();
		lock2.unlock();
	});

	t1.join();
	t2.join();
}

std::mutex mtx;
bool testTFinish = false;
std::condition_variable cv;
void ThreadNotifyTest()
{
	std::thread testT([]
	{
		testTFinish = false;
		cout << "notify thread start" << endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));
		cout << "notify thread finish" << endl;
		{
			std::lock_guard<std::mutex> lck(mtx);
			testTFinish = true;
		}
		cv.notify_one();
	});

	cout << "main thread start" << endl;

	while (true)
	{
		cout << "Loop" << endl;
		{
			std::unique_lock<std::mutex> lck(mtx);
			cv.wait(lck, [] {return testTFinish; });
			break;
		}
	}
	cout << "main thread finish" << endl;
	testT.join();
}

void ThreadTest()
{
	ThreadNotifyTest();
	//MutexLockTest();
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
	cout << "threads over.\n";//必须join完成

	int testN = 1;
	thread dThread([testN] {
		for (size_t i = 0; i < 100000; i++)
		{
			cout << testN;
		}
	});
	dThread.detach();
}

void MoveThread()
{
	thread t1(MutexTest1, 100, 'P');
	thread t2 = move(t1);


}

