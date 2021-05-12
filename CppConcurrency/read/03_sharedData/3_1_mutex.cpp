/*
 * @Author: jadyan
 * @Date: 2021-01-22 17:54:40
 * @LastEditTime: 2021-01-27 14:06:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/3.1.cpp
 */


#include <iostream>
#include <list>
#include <mutex>
#include <algorithm>
#include <unistd.h>

using namespace std;

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value) {
    cout<<"add"<<endl;
    std::lock_guard<std::mutex> guard(some_mutex);
    //C++17中添加了一个新特性，称为模板类参数推导，这样类似std::locak_guard这样简单的模板类型的模板参数列表可以省略
    //std::lock_guard guard(some_mutex);
    sleep(2);
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find) {
    cout<<"find"<<endl;
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}

#include <iostream>

int main() {
    add_to_list(42);
    std::cout << "contains(1)=" << list_contains(1) << ", contains(42)=" << list_contains(42) << std::endl;
}
