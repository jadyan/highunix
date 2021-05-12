
#include <iostream>
#include <future>
#include <chrono>
#include <functional>
 
//声明一个可调对象F
using F = std::function<int(int, int, int&)>;		//等同于typedef std::function<int(int, int, int&)> F;
 
//函数可以改成任意参数，任意返回类型
int Test_Fun(int a, int b, int &c)
{
	//a = 1, b = 2
	c = a + b + 230;
	return c;
}
 
void Thread_Fun1(std::promise<F> &p)
{
	//为了突出效果，可以使线程休眠5s
	std::this_thread::sleep_for(std::chrono::seconds(5));
 
	std::cout << "传入函数Test_Fun" << std::endl;
 
	//传入函数Test_Fun
	p.set_value(std::bind(&Test_Fun, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
 
template<typename T, typename ...Args>
void Thread_Fun2(std::future<T> &f, Args&& ...args)
{
	//阻塞函数，直到收到相关联的std::promise对象传入的数据
	auto fun = f.get();		//fun等同于Test_Fun
 
	auto fResult = fun(std::forward<Args>(args)...);
 
	std::cout << "收到函数并运行，结果：" << fResult << std::endl;
}
 
int main()
{
	//声明一个std::promise对象pr1，其保存的值类型为int
	std::promise<F> pr1;
	//声明一个std::future对象fu1，并通过std::promise的get_future()函数与pr1绑定
	std::future<F> fu1 = pr1.get_future();
 
	//声明一个变量
	int iVal = 0;
 
	//创建一个线程t1，将函数Thread_Fun1及对象pr1放在线程里面执行
	std::thread t1(Thread_Fun1, std::ref(pr1));
	//创建一个线程t2，将函数Thread_Fun2及对象fu1放在线程里面执行
	std::thread t2(Thread_Fun2<F, int, int, int&>, std::ref(fu1), 1, 2, std::ref(iVal));
 
	//阻塞至线程结束
	t1.join();
	t2.join();
 
	//此时iVal的值变成233
 
	return 1;
}