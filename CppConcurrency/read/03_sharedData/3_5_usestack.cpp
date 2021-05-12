/*
 * @Author: your name
 * @Date: 2021-01-27 14:36:13
 * @LastEditTime: 2021-01-27 16:14:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/3_5_usestack.cpp
 */
//
//  3.5.cpp 发现接口内在的条件竞争
//  Cpp-Concurrency

//  线程安全的堆栈简单实现

#include <iostream>
#include <exception>
#include <stack>
#include <mutex>
#include <memory>
#include <unistd.h>

//#define AUTOLOCK(mtx) std::lock_guard<std::mutex> lck(mtx);
//#define AUTO_LOCK(mtx) std::unique_lock<std::mutex> lck(mtx);

using namespace std;

struct empty_stack: std::exception {
    const char* what() const throw() {
        return "empty stack";
    }
};

template <typename T>
class threadsafe_stack {
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack() 
    {
        cout<<"threadsafe_stack 构造函数"<<endl;
    }
    threadsafe_stack(const threadsafe_stack& other) {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;  // 在构造函数体中执行拷贝
    }
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;
    
    void push(T new_value) {
        std::lock_guard<std::mutex> lock(m);
        cout<<"stack push value "<<new_value<<endl;
        data.push(new_value);
    }
    
    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(m);
        // 调用pop前，检查栈是否为空
        if (data.empty())
            throw empty_stack();
        // 在修改堆栈前，分配出返回值
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        cout<<"stack pop value "<< data.top() <<endl;
        data.pop();
        return res;
                                     
    }
    
    void pop(T& value) {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty()){
            throw empty_stack();
        }
        value = data.top();

        data.pop();
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

int main() {
    threadsafe_stack<int> si;
    si.push(5);
    si.pop();
    si.push(18);
    si.push(27);
    if (!si.empty()) {
        cout<<"pop(x) call"<<endl;
        int x;
        si.pop(x);
    }
}
