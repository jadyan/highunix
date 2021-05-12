/*
 * @Author: your name
 * @Date: 2021-01-27 14:30:45
 * @LastEditTime: 2021-01-27 14:31:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/3_3_stack.cpp
 */

//
//  3.3.cpp 发现接口内在的条件竞争
//  Cpp-Concurrency
//
//  Copyright © 2017年 kelele67. All rights reserved.
//  std::stack 容器的实现
#include <iostream>
#include <deque>
using namespace std;
template <typename T, typename Container=std::deque<T> >

class stack {
public:
    explicit stack(const Container&);
    explicit stack(Container&& = Container());
    template <class Alloc> explicit stack(const Alloc&);
    template <class Alloc> stack(const Container&, const Alloc&);
    template <class Alloc> stack(Container&&, const Alloc&);
    template <class Alloc> stack(stack&&, const Alloc&);
    
    bool empty() const;
    size_t size() const;
    T& top();
    T const& top() const;
    void push(T const&);
    void push(T&&);
    void pop();
    void swap(stack&&);
};

int main() {
    
}
