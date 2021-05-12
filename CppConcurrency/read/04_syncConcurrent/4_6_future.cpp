/*
 * @Author: your name
 * @Date: 2021-02-02 09:41:40
 * @LastEditTime: 2021-02-02 09:43:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/4_6_future.cpp
 */
//
//  4.6.cpp 使用期望等待一次性事件->带返回值的后台任务
//  Cpp-Concurrency
//
//  使用 std::future 从异步任务中获取返回值

#include <iostream>
#include <future>
#include <iostream>
#include <chrono>
#include<thread>

int find_the_answer_to_ltuae() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 42;
}

void do_other_stuff() {
    
}

int main() {
    std::future<int> the_answer = std::async(find_the_answer_to_ltuae);
    do_other_stuff();
    std::cout << "The answer is " << the_answer.get() << std::endl;
}
