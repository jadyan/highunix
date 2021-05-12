/*
 * @Author: your name
 * @Date: 2021-01-29 10:10:24
 * @LastEditTime: 2021-01-29 10:11:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/4.2.cpp
 */

//

//  4.2.cpp 等待一个事件或其他事件->使用条件变量构建线程安全队列
//  Cpp-Concurrency
//

//  std::queue 接口
#include <iostream>
#include <queue>
 
 using namespace std;

template <class T, class Container = std::queue<T> >
class queue {
public:
    explicit queue(const Container&);
    explicit queue(Container&& = Container()); // 右值引用
    queue(queue&& q);
    
    template <class Alloc> explicit queue(const Alloc&);
    template <class Alloc> queue(const Container&, const Alloc&);
    template <class Alloc> queue(Container&&, const Alloc);
    template <class Alloc> queue(queue&&, const Alloc);
    
    queue& operator=(queue&& q);
    void swap(queue&& q);
    
    bool empty() const;
    size_t size() const;
    
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    
    void push(const T& x);
    void push(T&& x);
    void pop();
};

int main() {
    
}
