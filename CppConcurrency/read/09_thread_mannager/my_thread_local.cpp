/*
 * @Author: your name
 * @Date: 2021-03-09 21:02:20
 * @LastEditTime: 2021-03-09 21:15:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/09_thread_mannager/my_thread_local.cpp
 * https://murphypei.github.io/blog/2020/02/thread-local
 */
#include <iostream>
#include <thread>

thread_local int g_n = 1;

void f()
{
    g_n++;
    printf("id=%d, n=%d\n", std::this_thread::get_id(),g_n);
}

void foo()
{
    /*
    明为thread_local的本地变量在线程中是持续存在的，不同于普通临时变量的生命周期，
    它具有static变量一样的初始化特征和生命周期，虽然它并没有被声明为static。
    例子中foo函数中的thread_local变量 i 在每个线程第一次执行到的时候初始化，在每个线程各自累加，在线程结束时释放。
    */
    thread_local int i=0;
    printf("id=%d, i=%d\n", std::this_thread::get_id(), i);
    i++;
    //i++;
}

void f2()
{
    foo();
    foo();
}

int main()
{
    g_n++; 
    f();    
    std::thread t1(f);
    std::thread t2(f);

    t1.join();
    t2.join();


    f2();
    std::thread t4(f2);
    std::thread t5(f2);

    t4.join();
    t5.join();
    return 0;
}

