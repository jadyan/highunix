/*
 * @Author: your name
 * @Date: 2021-02-03 10:36:03
 * @LastEditTime: 2021-03-08 14:04:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/my_packaged_task.cpp
 */
// packaged_task example
#include <iostream>     // std::cout
#include <future>       // std::packaged_task, std::future
#include <chrono>       // std::chrono::seconds
#include <thread>       // std::thread, std::this_thread::sleep_for

// count down taking a second for each value:
int countdown (int from, int to) {
	for (int i=from; i!=to; --i) {
		std::cout << i << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout << "Lift off!\n";
	return from-to;
}



/*

https://www.jianshu.com/p/72601d82f3df
std::packaged_task的作用就是提供一个不同线程之间的数据同步机制，它可以存储一个函数操作，并将其返回值传递给对应的future，
 而这个future在另外一个线程中也可以安全的访问到这个值。
*/
int main ()
{
	std::packaged_task<int(int,int)> tsk (countdown);   // set up packaged_task
	std::future<int> ret = tsk.get_future();            // get future

    // std::thread的第一个参数不止是函数，还可以是一个可调用对象，即支持operator()(Args...)操作
	std::thread th (std::move(tsk),10,2);   // spawn thread to count down from 10 to 0

    //tsk(10,0);
	int value = ret.get();                  // wait for the task to finish and get result

	std::cout << "The countdown lasted for " << value << " seconds.\n";

	th.join();

	return 0;
}
