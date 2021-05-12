/*
 * @Author: jadyan
 * @Date: 2021-01-22 17:50:16
 * @LastEditTime: 2021-01-27 09:30:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/2.6.cpp
 */


#include <iostream>
#include <thread>
#include <utility>

using namespace std;

class scoped_thread {
    std::thread t;
public:
    explicit scoped_thread(std::thread t_): t(std::move(t_)) {
        cout <<"scoped_thread构造"<<endl;
        if (!t.joinable()) {
            throw std::logic_error("No thread");
        }
    }
    ~scoped_thread() {
        t.join();
    }
    scoped_thread(scoped_thread const&) = delete;
    scoped_thread& operator= (scoped_thread const &) = delete;
};

void do_something(int& i) {
    ++i;
}

struct func {
    int& i;
    
    func(int& i_) : i(i_) {}
    
    void operator() () {
        cout<<"线程开始!"<<endl;
        for (unsigned j = 0; j < 1000000; ++j) {
            do_something(i);
        }
    }
};

void do_something_in_current_thread() {
}

void f() {
    int some_local_state;
    /*
    https://www.zhihu.com/question/42789717

    */
    scoped_thread t((std::thread(func(some_local_state)))); // 外面需要再加一个括号？Parentheses were disambiguated as a function declaration
    //相当于声明
    //scoped_thread t(std::thread(func(some_local_state))); 
    
    do_something_in_current_thread();
}

int main() {
    f();
}
