/*
 * @Author: jadyan
 * @Date: 2021-01-27 14:08:20
 * @LastEditTime: 2021-01-27 14:28:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/3_1_mutex_class.cpp
 */

#include <iostream>
#include <mutex>
#include <string>
#include <unistd.h>

using namespace std;

class some_data {
    int a;
    std::string b;
public:
    void do_something() 
    {
        cout<<"do_something data 函数"<<endl;
    }
};


class data_wrapper {
private:
    some_data data;
    std::mutex m;
public:
    template<typename Function>
    void process_data(Function func) {
        std::lock_guard<std::mutex> l(m);
        cout<<"process_data start"<<endl;
        func(data); // 传递“保护”数据给用户
    }
};

some_data *unprotected;

void malicious_function(some_data& protected_data) {
    cout<<"malicious_function start"<<endl;
    unprotected = &protected_data;
}

data_wrapper x;

void foo() {
    x.process_data(malicious_function); // 传递一个恶意函数
    cout<<"保护调用"<<endl;
    unprotected->do_something(); // 在无保护的情况下访问保护数据
}

int main() {
    foo();
}
