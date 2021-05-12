/*
 * @Author: your name
 * @Date: 2021-02-04 09:20:28
 * @LastEditTime: 2021-02-04 10:25:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/my_shared_futrue.cpp
 */

#include <iostream>
#include <future>
#include <chrono>
#include <utility>
#include <thread>



/*

https://blog.csdn.net/fengbingchun/article/details/104118831
    [capture](parameters) -> return-type
    { 
    //body
    }
    capture 是可选的，后面讲如何使用
    parameters 为表达式的参数，类似于函数中的形参
    return-type 返回值类型

*/
 
namespace myfuture {
 
///
// reference: https://en.cppreference.com/w/cpp/thread/shared_future
int test_shared_future_1()
{
	std::promise<void> ready_promise, t1_ready_promise, t2_ready_promise;
	// 通过std::future移动构造std::shared_future对象,t1_ready_promise和t2_ready_promise均会用到ready_future
	std::shared_future<void> ready_future(ready_promise.get_future());
 
    //high_resolution_clock的精度是纳秒
    //std::chrono::time_point 表示一个具体时间
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
 
	auto fun1 = [&, ready_future]() -> std::chrono::duration<double, std::milli> {
			t1_ready_promise.set_value();
			ready_future.wait(); // waits for the signal from main()
			return std::chrono::high_resolution_clock::now() - start;
		};
 
	auto fun2 = [&, ready_future]() -> std::chrono::duration<double, std::milli> {
		t2_ready_promise.set_value();
		ready_future.wait(); // waits for the signal from main()
		return std::chrono::high_resolution_clock::now() - start;
	};
 
	auto fut1 = t1_ready_promise.get_future();
	auto fut2 = t2_ready_promise.get_future();
 
	auto result1 = std::async(std::launch::async, fun1);
	auto result2 = std::async(std::launch::async, fun2);
 
	// wait for the threads to become ready
	fut1.wait();
	fut2.wait();
 
	// the threads are ready, start the clock
	start = std::chrono::high_resolution_clock::now();
 
	// signal the threads to go
	ready_promise.set_value();
 
	std::cout << "Thread 1 received the signal " << result1.get().count() << " ms after start\n"
		<< "Thread 2 received the signal " << result2.get().count() << " ms after start\n";
 
	return 0;
}
 
///
// reference: https://en.cppreference.com/w/cpp/thread/shared_future/wait
int fib(int n)
{
	if (n < 3) return 1;
	else return fib(n - 1) + fib(n - 2);
}
 
int test_shared_future_2()
{
	// 将std::shared_future<int>调整为std::future<int>也是正确的
	std::shared_future<int> f1 = std::async(std::launch::async, []() { return fib(20); });
	std::shared_future<int> f2 = std::async(std::launch::async, []() { return fib(25); });
 
	std::cout << "waiting...\n";
	f1.wait();
	f2.wait();
 
	std::cout << "f1: " << f1.get() << '\n';
	std::cout << "f2: " << f2.get() << '\n';
 
	return 0;
}
 
///
// reference: https://en.cppreference.com/w/cpp/thread/shared_future/wait_for
int test_shared_future_3()
{
	// 将std::shared_future<int>调整为std::future<int>也是正确的
	std::shared_future<int> future = std::async(std::launch::async, [](){ 
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 8;  
    }); 
 
    std::cout << "waiting...\n";
    std::future_status status;
    do {
        status = future.wait_for(std::chrono::seconds(1));
        if (status == std::future_status::deferred) {
            std::cout << "deferred\n";
        } else if (status == std::future_status::timeout) {
            std::cout << "timeout\n";
        } else if (status == std::future_status::ready) {
            std::cout << "ready!\n";
        }
    } while (status != std::future_status::ready); 
 
    std::cout << "result is " << future.get() << '\n';
 
	return 0;
}
 
} // namespace future_



int main( int argc , char **argv)
{

std::chrono::time_point<std::chrono::high_resolution_clock> start;


    //std::thread t1(myfuture::test_shared_future_1);
    //std::thread t2(myfuture::test_shared_future_2);
    std::thread t3(myfuture::test_shared_future_3);

    //t1.join();
    //t2.join();
    t3.join();
}