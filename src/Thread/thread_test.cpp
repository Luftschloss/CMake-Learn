#include <mutex>
#include <vector>
#include "thread_func.h"
#include "thread_test.h"
#include "threads.h"
#include "thread_utils.h"

using namespace std;

void ThreadFunc1()
{
	cout << "ThreadFunc1" << endl;
}

void ThreadFunc2(int p)
{
	cout << "ThreadFunc2  Input : " << p << endl;
}

void ThreadWhile1()
{
	while (1)
	{
		cout << "While Thread1 On" << endl;
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

void ThreadParam1()
{
	ThreadX x;
	int num = 5;
	std::thread t(&ThreadX::do_work, &x, num);
	auto id = t.get_id();
	t.join();
	auto id2 = t.get_id();

	std::cout << "ThreadParam1  \t" << num << std::endl;
}

void ThreadTest2()
{
	std::vector<int> arr;
	auto sum0 = 0;
	for (size_t i = 0; i < 1000; i++)
	{
		arr.push_back(i);
		sum0 += i;
	}
	auto sum = parallel_accumulate(arr.begin(), arr.end(), 0);

}

std::mutex cv_mtx;
bool ready = false;
std::condition_variable cv2;

void condition_work()
{
	auto tid = std::this_thread::get_id();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	{
		std::lock_guard<std::mutex> lock(cv_mtx);
		ready = true;
	}
	cv2.notify_one();
}

void ThreadCondition1()
{
	auto tid = std::this_thread::get_id();
	std::thread t(condition_work);
	//主线程等待条件满足
	{
		std::unique_lock<std::mutex> lock(cv_mtx);
		cv2.wait(lock, [] {return ready; });
	}
	std::cout << "ThreadCondition1 resumed" << std::endl;
	t.join();
	std::cout << "ThreadCondition1 finish" << std::endl;
}

class RingBufferData
{
public:
	RingBufferData(int _id = 0, int _val = 0)
	{
		id = _id;
		value = _val;
		sprintf(data, "ID = %d, Value = %d\n", _id, _val);
	}
private:
	int id;
	int value;
	char data[128];
};

RingBuffer<RingBufferData> ringBuffer(512);

const size_t N = 1024;

void producer()
{
	auto start = clock();
	for (size_t i = 0; i < N; i++)
	{
		RingBufferData data(i % 512, i);
		ringBuffer.Push(data);
	}
	double tm = (clock() - start) / CLOCKS_PER_SEC;
	auto a = this_thread::get_id();
	printf("producer tid=%lu %f MB/s elapsed= %u size= %lu\n", this_thread::get_id(), N * sizeof(RingBufferData)/ tm * (1024 * 1024), tm, N);
}
void consumer()
{
	_sleep(1);
	RingBufferData data;
	auto start = clock();
	unsigned int i = 0;
	while (i < N)
	{
		if (ringBuffer.Pop(data))
		{
			i++;
		}
	}
	double tm = (clock() - start) / CLOCKS_PER_SEC;
	printf("consumer tid=%lu %f MB/s elapsed= %f, size=%u \n", this_thread::get_id(), N * sizeof(RingBufferData) / (tm * 1024 * 1024), tm, i);
}


void RingBufferTest()
{
	std::thread producer1(producer);
	std::thread consumer(consumer);
	producer1.join();
	consumer.join();
}