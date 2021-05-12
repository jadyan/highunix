/*
 * @Author: your name
 * @Date: 2021-01-22 17:45:56
 * @LastEditTime: 2021-01-22 17:45:57
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/2.2.cpp
 */

#include <iostream>
#include <thread>

using namespace std;

void do_something(int &i) {
    cout<<"线程函数"<<endl;
    ++i;
}

struct func {
    int& i;
    
    func (int& i_):i(i_){}
    
    void operator()() 
	{
		cout<<"线程1"<<endl;
        for (unsigned j = 0; j < 10000000; ++j) {
            do_something(i);
        }
    }
};

void do_something_in_current_thread() {
	cout<<"主线程"<<endl;	

}

void f() {
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    try {
        do_something_in_current_thread();
    }
    catch(...) {
        t.join();
        throw;
    }
    t.join();
}

int main() {
    f();
}
