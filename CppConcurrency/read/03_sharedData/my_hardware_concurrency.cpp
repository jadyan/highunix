#include <iostream>
#include <thread>


/*
 *
 *一、功能
获取硬件支持的并发线程数

二、返回值
正常返回支持的并发线程数，若值非良定义或不可计算，则返回 0 
 *
 * */
 
int main()
{
    unsigned int in = std::thread::hardware_concurrency();
 
    std::cout << in << std::endl;
}

