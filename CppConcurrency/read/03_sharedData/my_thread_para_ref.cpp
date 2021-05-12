/*
 * @Author: your name
 * @Date: 2021-01-26 17:33:25
 * @LastEditTime: 2021-01-26 17:46:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/my_thread_para2.cpp
 */
#include <iostream>
#include <thread>
#include <unistd.h>


/*

https://longhaiqwe.github.io/2018/11/13/C++/C++11%E5%A4%9A%E7%BA%BF%E7%A8%8B-%E3%80%903%E3%80%91%E4%BC%A0%E5%8F%82%E7%BB%99%E7%BA%BF%E7%A8%8B/

因为参数被复制到新线程堆栈，所以，如果你需要以普通方式传递引用

结果：
In Main Thread : Before Thread Start x = 9
Inside Thread x = 10
In Main Thread : After Thread Joins x = 9

原因：
即使threadCallback接受引用作为参数，但进行了更改之后，在线程之外也不可见。

这是因为线程函数threadCallback中的 x 是在新线程堆栈中复制的临时值的引用。


使用std::ref()

In Main Thread : Before Thread Start x = 9
Inside Thread x = 10
In Main Thread : After Thread Joins x = 10


*/

#include <iostream>
#include <thread>

void threadCallback(int const & x)
{
    int & y = const_cast<int &>(x);
    y++;
    std::cout<<"Inside Thread x = "<<x<<std::endl;
}
int main()
{
    int x = 9;
    std::cout<<"In Main Thread : Before Thread Start x = "<<x<<std::endl;
    //std::thread threadObj(threadCallback, x);
    std::thread threadObj(threadCallback, std::ref(x));
    threadObj.join();
    std::cout<<"In Main Thread : After Thread Joins x = "<<x<<std::endl;
    return 0;
}

