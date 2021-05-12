/*
 * @Author: your name
 * @Date: 2021-01-22 17:50:59
 * @LastEditTime: 2021-01-27 09:52:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/2.7.cpp
 */

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <unistd.h>
using namespace std;

void do_work(unsigned id) {
    std::cout<<"线程"<<id<<"开始"<<std::endl;
    while (true)
    {
        sleep(2);
    }
    
}

void f() {
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 20; ++i) {
        threads.push_back(std::thread(do_work, i));
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join)); // 让容器vector中的每一个元素都执行一遍join()
    sleep(10);
}

int main() {
    f();
}
