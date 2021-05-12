/*
 * @Author: your name
 * @Date: 2021-01-26 15:39:39
 * @LastEditTime: 2021-01-26 15:41:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/helloworld.cpp
 */
//

#include <iostream>
#include <thread>
#include <unistd.h>

void hello() {
    std::cout << "Hello Concurrent World\n";
    for (;;)
    {
        std::cout<<"hello thread"<<std::endl;
        sleep (1);
    }
}

int main() {
    std::thread t(hello);
    for(;;)
    {
        std::cout<<"main thread"<<std::endl;
        sleep (1);
    }
    t.join();
}
