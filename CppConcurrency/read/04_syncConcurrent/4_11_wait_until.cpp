/*
 * @Author: your name
 * @Date: 2021-02-04 15:13:24
 * @LastEditTime: 2021-02-04 15:19:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/4.11.cpp
 */
//
//  4.11.cpp 限定等待时间->时间点
//  Cpp-Concurrency
//
//  等待一个条件变量一一有超时功能

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <chrono>

std::condition_variable cv;
bool done;
std::mutex m;

bool wait_loop() {
    //std::chrono::system_clock::now()是将返回系统时钟的当前时间
    auto const timeout = std::chrono::steady_clock::now() + std::chrono::milliseconds(5000);
    std::unique_lock<std::mutex> lk(m);
    while (!done) {
        if (cv.wait_until(lk, timeout) == std::cv_status::timeout)
        {
            std::cout<<"timeout"<<std::endl;
            break;
        }

    }
    return done;
}



int main(int argc, char ** argv)
{
    wait_loop();
}
