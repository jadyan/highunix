/*
 * @Author: your name
 * @Date: 2021-01-22 17:25:26
 * @LastEditTime: 2021-01-26 16:23:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/2.3.cpp
 */
#include <iostream>
#include <thread>

using namespace std;
class thread_guard {
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_): t(t_) {}
    ~thread_guard() {
        if (t.joinable()) { // 判断线程是否加入
            cout<<"线程join 开始"<<endl;
            t.join();
        }
    }
    thread_guard(thread_guard const&) = delete; // 不让编译器自动生成它们
    thread_guard& operator=(thread_guard const&) = delete;
};

void do_something(int& i) {
    cout<<"线程函数"<<endl;
    ++i;
}

struct func {
    int& i;
    func(int& i_): i(i_) {}
    
    void operator()() {
        for (unsigned j = 0; j < 1000000; ++j) {
            do_something(i);
        }
    }
};

void do_something_in_current_thread() {
    std::cout<<"主线程"<<std::endl;

}

void f() {
    int some_local_state;
    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
    
    do_something_in_current_thread();
} // 局部对象被逆序销毁

int main() {
    f();
}
