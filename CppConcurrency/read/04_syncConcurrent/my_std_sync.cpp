#include <iostream>
#include <future>
#include <iostream>


/*
https://www.cnblogs.com/haippy/p/3280643.html
https://www.cnblogs.com/moodlxs/p/10111601.html


https://blog.csdn.net/u012507022/article/details/86100716


（1）如果使用std::thread()创建的线程太多，则可能创建线程失败，系统报告异常，崩溃；

（2）如果用std::async,一般就不会报异常崩溃，因为如果系统资源紧张导致无法创建新线程的时候，std::async这种不加额外参数的调用就不会创建新线程，而是后续谁调用了future::get()来请求结果，那么这个异步任务就运行在执行这条get()语句所在的线程上。

（3）如果你强制std::async创建新线程，那么就必须使用std::launch::async，承受的代价就是系统资源紧张时，可能程序崩溃。经验：一个程序里，线程的数量不易超过100-200，与时间片有关，详情参考操作系统。


std::async 函数，本文后面会介绍 std::async() 函数。
std::promise::get_future，get_future 为 promise 类的成员函数，详见 C++11 并发指南四(<future> 详解一 std::promise 介绍)。
std::packaged_task::get_future，此时 get_future为 packaged_task 的成员函数，详见C++11 并发指南四(<future> 详解二 std::packaged_task 介绍)。
*/

// std::future可以从异步任务中获取结果，一般与std::async配合使用，std::async用于创建异步任务，实际上就是创建一个线程执行相应任务。

// std::async就是异步编程的高级封装，封装了std::future的操作，基本上可以代替std::thread 的所有事情。

// std::async的操作，其实相当于封装了std::promise、std::packaged_task加上std::thread。

using  namespace std;
bool  is_prime(int x)
{ 
	for (int i=1; i<x; i++)
	{
		cout<<"x % i = "<< x % i <<endl;
		if (x % i == 0)
			return false;
	}
	return true;

}



int myThread()
{
	cout << "Thread start id = " << this_thread::get_id() << endl;
	//chrono::milliseconds time(5000); //休息5秒
	//this_thread::sleep_for(time);
	cout << "Thread end id = " << this_thread::get_id() << endl;
	return 5;
}



void getmyThread()
{

	future<int> result = async(myThread);
	cout << "continue......" << endl;

	std::future_status status = result.wait_for(std::chrono::seconds(6));
	//超时，线程还没执行完
	if (status == std::future_status::ready)
	{
		cout << "线程执行完" << endl;
	}	 
	//成功，线程已经执行完
	else if (status == std::future_status::timeout)
	{
		cout << "线程超时" << endl;
	}
	//延迟，async第一个参数被设置为std::launch::deferred
	else if (status == std::future_status::deferred)
	{
		cout << "线程延迟执行" << endl;
	}

}

int main()
{

	getmyThread();
	//std::future<bool> fut = std::async(is_prime, 700020007);
	//std::future<bool > fut = std::async( is_prime, 10);
	std::future<bool > fut = std::async(std::launch::async, is_prime, 10);
	std::cout << "please wait";
	std::chrono::milliseconds span(1);
	/*
		wait_for可设置超时时间，如果在超时时间之内任务完成，则返回std::future_status::ready状态；
		如果在超时时间之内任务尚未完成，则返回std::future_status::timeout状态
	*/
	// while (fut.wait_for(span) != std::future_status::ready)
	// {
	// 	std::cout << ".";
	// }




	std::future_status status;
	do {
		status = fut.wait_for(std::chrono::seconds(1));
		if (status == std::future_status::deferred) {
			std::cout << "deferred" << std::endl;
		} else if (status == std::future_status::timeout) {
			std::cout << "timeout" << std::endl;
		} else if (status == std::future_status::ready) {
			std::cout << "ready!" << std::endl;
		}
	} while (status != std::future_status::ready); 

	std::cout << std::endl;

	bool ret = fut.get();
	std::cout << "final result: " << ret << std::endl;
	return 0;
}


