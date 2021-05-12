/*
 * @Author: your name
 * @Date: 2021-02-22 14:52:25
 * @LastEditTime: 2021-02-22 14:59:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/05_memrymodel/my_atomic_bool.cpp
 * 
 * https://blog.csdn.net/wanxuexiang/article/details/104280021
 */
#include <atomic>
#include <future>
#include <thread>
#include <chrono>
#include <iostream>

/*
但是使用store()去写入(true或false)还是好于std::atomic_flag中限制性很强的clear()。
同样，test_and_set()函数也可以被更加通用的exchange()成员函数所替换，
exchange()成员函数允许使用新选的值替换已存储的值，并且会自动检索原始值。
*/

long data = 0;
std:: atomic<bool> readFlag(false);

void provider (void)
{
    std::cout<<"return"<<std::endl;
    std::cin.get();
    data = 9527;
    readFlag.store(true);
}

void consumer(void)
{
    while(!readFlag.load())
    {
        std::cout.put('.').flush();
        std:: this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    std::cout << "data "<<data<<std::endl;
}

int main(void)
{
    auto p = std::async(std::launch::async,provider);
    auto c = std::async(std::launch::async,consumer);
}