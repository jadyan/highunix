/*
 * @Author: your name
 * @Date: 2021-02-03 14:47:08
 * @LastEditTime: 2021-02-04 10:31:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/my_promise1.cpp
 */


/*
*std::promise 是C++11并发编程中常用的一个类，常配合std::future使用。其作用是在一个线程t1中保存一个类型typename T的值，可供相绑定的std::future对象在另一线程t2中获取。
*
*/

#include <iostream>
#include <future>
#include <chrono>

using namespace std;
 
void Thread_Fun1(std::promise<int> &p)
{
	//为了突出效果，可以使线程休眠5s
	std::this_thread::sleep_for(std::chrono::seconds(5));
 
	int iVal = 233;
	std::cout << "传入数据(int)：" << iVal << std::endl;
 
	//传入数据iVal
	p.set_value(iVal);
}
 
void Thread_Fun2(std::future<int> &f)
{
	//阻塞函数，直到收到相关联的std::promise对象传入的数据
    cout<<"等待数据...."<<endl;
	auto iVal = f.get();		//iVal = 233
 
	std::cout << "收到数据(int)：" << iVal << std::endl;
}
 
int main()
{
	//声明一个std::promise对象pr1，其保存的值类型为int
	std::promise<int> pr1;
	//声明一个std::future对象fu1，并通过std::promise的get_future()函数与pr1绑定
	std::future<int> fu1 = pr1.get_future();
 
	//创建一个线程t1，将函数Thread_Fun1及对象pr1放在线程里面执行
	std::thread t1(Thread_Fun1, std::ref(pr1));
	//创建一个线程t2，将函数Thread_Fun2及对象fu1放在线程里面执行
	std::thread t2(Thread_Fun2, std::ref(fu1));
 
	//阻塞至线程结束
	t1.join();
	t2.join();
 
	return 1;
}
