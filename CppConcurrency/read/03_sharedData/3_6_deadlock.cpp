/*
 * @Author: your name
 * @Date: 2021-01-27 14:52:16
 * @LastEditTime: 2021-01-27 15:53:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/3.6.cpp
 */
//
//  3.6.cpp 死锁：问题描述及解决方案
//  Cpp-Concurrency
//

#include <iostream>
#include <mutex>
#include <thread>
using namespace std;

class some_big_object {
    public:
        some_big_object(int i=0):m_i(i){}
        void show()const 
        { 
            cout<<"i = "<<m_i<< ",  address = "<<this<<endl;
        }
    private:
        int m_i;
};

void swap(some_big_object& lhs, some_big_object& rhs) {
    
}

class X {
private:
    some_big_object some_detail;
    mutable std::mutex m;
public:
    X(some_big_object const& sd):some_detail(sd) {}
    
    friend void swap(X& lhs, X& rhs) {
        if (&lhs == &rhs) {
            return;
        }
        /*
        std::lock——可以一次性锁住多个(两个以上)的互斥量，并且没有副作用(死锁风险)。
        一个互斥量可以在同一线程上多次上锁

        调用std::lock()①锁住两个互斥量，并且两个std:lock_guard实例已经创建好②③。
        提供std::adopt_lock参数除了表示std::lock_guard对象可获取锁之外，
        还将锁交由std::lock_guard对象管理，而不需要std::lock_guard对象再去构建新的锁。
        */
    
        std::lock(lhs.m, rhs.m);    // 锁住两个互斥量 虽然可以避免死锁，但是没发帮助你获取其中一个锁
        std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock); // std::adopt_lcok表示除了 std::lock_guard的对象上锁以外，还表示现成的锁，而非尝试创建新的锁
        std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
        std::swap(lhs.some_detail, rhs.some_detail);
    }

    void show() const
    {
        this->some_detail.show();
    }
};

int main() {
    X a(some_big_object(18));
    a.show();
    X b(some_big_object(27));;
    b.show();

    swap(a,b);
    a.show();
    b.show();

}
