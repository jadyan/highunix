#include <iostream>
using namespace std;


// 为什么说new是低效的

// ① 一般来说，操作越简单，意味着封装了更多的实现细节。new作为一个通用接口，需要处理任意时间、任意位置申请任意大小内存的请求，它在设计上就无法兼顾一些特殊场景的优化，在管理上也会带来一定开销。

// ② 系统调用带来的开销。多数操作系统上，申请内存会从用户模式切换到内核模式，当前线程会block住，上下文切换将会消耗一定时间。

// ③ 分配可能是带锁的。这意味着分配难以并行化。

class A
{
	private:
		int data;
	public:
		A(int indata) 
			: data(indata) { }
		void print()
		{
			cout << data << endl;
		}
};
int main()
{
	const int size = 10;
	char buf[size * sizeof(A)]; // 内存分配
	cout<<"buf size"<<sizeof(buf) <<"地址 0x" << &buf<<endl;

	for (size_t i = 0; i < size; i++)
	{
		cout<<"地址 0x"<< &buf + i * sizeof(A) <<endl;
		new (buf + i * sizeof(A)) A(i); // 对象构造
	}
	A* arr = (A*)buf;
	for (size_t i = 0; i < size; i++)
	{
		arr[i].print();
		arr[i].~A(); // 对象析构
	}
}
