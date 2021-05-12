/*
 * @Author: your name
 * @Date: 2021-01-26 17:33:25
 * @LastEditTime: 2021-01-26 17:47:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/my_thread_para2.cpp
 */
#include <iostream>
#include <thread>
#include <unistd.h>


#include <iostream>
#include <thread>
class DummyClass {
public:
    DummyClass()
    {}
    DummyClass(const DummyClass & obj)
    {}
    void sampleMemberFunction(int x)
    {
        std::cout<<"Inside sampleMemberFunction "<<x<<std::endl;
    }
};
int main() {
 
    DummyClass dummyObj;
    int x = 10;
    std::thread threadObj(&DummyClass::sampleMemberFunction,&dummyObj, x);
    threadObj.join();
    return 0;
}