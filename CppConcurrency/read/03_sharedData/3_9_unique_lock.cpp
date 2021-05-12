/*
 * @Author: your name
 * @Date: 2021-01-28 10:13:22
 * @LastEditTime: 2021-01-28 10:59:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/03_sharedData/3_9_unique_lock.cpp
 */
//
//https://www.cnblogs.com/fnlingnzb-learner/p/9542183.html
//https://www.cnblogs.com/rynerlute/p/11809408.html
//  3.9.cpp std::unique_lock 灵活的锁
//  Cpp-Concurrency
//  交换操作中使用 std::lock() 和 std::unique_lock

#include <iostream>
#include <mutex>
#include <string>
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


/*
std::lock_guard其实就是简单的RAII封装，在构造函数中进行加锁，析构函数中进行解锁，这样可以保证函数退出时，锁一定被释放。

std::unique_lock的功能相比std::lock_guard来说，就强大多了，是std::lock_guard的功能超集， 
封装了各种加锁操作，阻塞的，非阻塞的，还可以结合条件变量一起使用，基本上对锁的各种操作都封装了，当然了，功能丰富是有代价的，那就是性能和内存开销都比std::lock_guard大得多，所以，需要有选择地使用。
std::unique_lock也会在析构的时候自动解锁，所以说，是std::lock_guard的功能超集。

std::adopt_lock  表示这个互斥量已经被lock了（你必须要把互斥量提前lock了 ，否者会报异常）；
adopt_lock在构造unique_lock时锁定mtx，阻塞直到获取锁为止
defer_lock在构造unique_lock时不锁定mtx
try_to_lock在构造unique_lock时尝试锁定mtx，获未获取都不阻塞

*/
class X {
private:
    some_big_object some_detail;
    mutable std::mutex m;
public:
    X(some_big_object const& sd): some_detail(sd) {}
    
    friend void swap(X& lhs, X&rhs) {
        if (&lhs == &rhs)
            return;
        std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock); // defer_lock 留下未上锁的互斥量
        std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock);
        std::lock(lock_a, lock_b);  // 互斥量在这里上锁
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
