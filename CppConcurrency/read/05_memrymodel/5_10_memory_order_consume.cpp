/*
 * @Author: your name
 * @Date: 2021-02-23 13:45:34
 * @LastEditTime: 2021-02-23 14:00:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/05_memrymodel/5_10_memory_order_consume.cpp
 */
//
//  5.10.cpp 同步操作和强制排序->原子操作中的内存顺序
//  获取-释放序列
//  Cpp-Concurrency
//
/*
对a的存储①在存储p②之前，并且存储p的操作标记为memory_order_release，加载p③的操作标记为
memory_order_consume，这意味着存储p仅先行那些需要加载p的操作。同样，也意味着X结构体中数据成员所在的断
言语句④⑤不会被触发，因为对x变量操作的表达式对加载p的操作携带有依赖。另一方面，对于加载变量a⑥的断言就不
能确定是否会被触发；这个操作并不依赖于p的加载操作，所以这里没法保证数据已经被读取。当然，这个情况也很明
显，因为这个操作被标记为memory_order_relaxed。
*/

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <assert.h>

using namespace std;
struct X {
    int i;
    std::string s;
};

std::atomic<X*> p;
std::atomic<int> a;

void create_x() {
    X* x = new X;
    x->i = 42;
    x->s = "hello";
    a.store(99, std::memory_order_relaxed); // 在p之前储存a
    p.store(x, std::memory_order_release);
}

void print(const X &x)
{
    cout<<"x.i = "<<x.i<<" x.s= "<<x.s<<endl;
}
void use_x() {
    X* x;
    while (!(x = p.load(std::memory_order_consume))) // std::memory_order_consume 意味着储存前需要先加载
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    print(*x);
    assert(x->i == 42); // 不会触发
    assert(x->s == "hello"); // 不会触发
    assert(a.load(std::memory_order_relaxed) == 99); // 存储p仅先行那些需要加载p的操作  不能确定
}

int main() {
    std::thread t1(create_x);
    std::thread t2(use_x);
    t1.join();
    t2.join();
}
