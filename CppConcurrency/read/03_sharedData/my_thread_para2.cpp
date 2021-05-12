/*
 * @Author: your name
 * @Date: 2021-01-26 17:33:25
 * @LastEditTime: 2021-01-26 17:37:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/my_thread_para2.cpp
 */
#include <iostream>
#include <thread>
#include <unistd.h>


/*

https://longhaiqwe.github.io/2018/11/13/C++/C++11%E5%A4%9A%E7%BA%BF%E7%A8%8B-%E3%80%903%E3%80%91%E4%BC%A0%E5%8F%82%E7%BB%99%E7%BA%BF%E7%A8%8B/

不要将变量的地址从本地堆栈传递给线程的回调函数。因为线程 1 中的局部变量可能超出范围，但是线程 2 仍然试图通过其地址访问它。
在这种情况下，访问无效地址会导致意外行为。
*/
void newThreadCallback(int * p)
{
    std::cout<<"Inside Thread :  "" : p = "<<p<<std::endl;
    std::chrono::milliseconds dura( 1000 );
    std::this_thread::sleep_for( dura );
    sleep(1);
    *p = 19;
}
void startNewThread()
{
    int i = 10;
    std::cout<<"Inside Main Thread :  "" : i = "<<i<<std::endl;
    std::thread t(newThreadCallback,&i);
    t.detach();
    //sleep(1);
    std::cout<<"Inside Main Thread :  "" : i = "<<i<<std::endl;
}

int main(int argc, char ** argv)
{
    startNewThread();
    std::chrono::milliseconds dura( 2000 );
    std::this_thread::sleep_for( dura );
    return 0;
}


