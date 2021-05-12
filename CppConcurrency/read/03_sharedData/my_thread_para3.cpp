/*
 * @Author: your name
 * @Date: 2021-01-26 17:33:25
 * @LastEditTime: 2021-01-26 17:39:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/my_thread_para2.cpp
 */
#include <iostream>
#include <thread>
#include <unistd.h>


/*

https://longhaiqwe.github.io/2018/11/13/C++/C++11%E5%A4%9A%E7%BA%BF%E7%A8%8B-%E3%80%903%E3%80%91%E4%BC%A0%E5%8F%82%E7%BB%99%E7%BA%BF%E7%A8%8B/

将指向堆上内存的指针传递给线程时也要小心。因为在新线程试图访问内存之前，一些线程可能会删除该内存。

在这种情况下，访问无效地址会导致意外行为。
*/

void newThreadCallback(int * p)
{
    std::cout<<"Inside Thread :  "" : p = "<<p<<std::endl;
    std::chrono::milliseconds dura( 1000 );
    std::this_thread::sleep_for( dura );
    *p = 19;
}
void startNewThread()
{
    int * p = new int();
    *p = 10;
    std::cout<<"Inside Main Thread :  "" : *p = "<<*p<<std::endl;
    std::thread t(newThreadCallback,p);
    t.detach();
    delete p;
    p = NULL;
}
int main()
{
    startNewThread();
    std::chrono::milliseconds dura( 2000 );
    std::this_thread::sleep_for( dura );
    return 0;
}

