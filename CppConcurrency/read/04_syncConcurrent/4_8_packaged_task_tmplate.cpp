/*
 * @Author: your name
 * @Date: 2021-02-02 17:33:47
 * @LastEditTime: 2021-02-03 10:19:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/4.8.cpp
 */
//
//  4.8.cpp 使用期望等待一次性事件->任务与期望
//  Cpp-Concurrency
//
//  std::packaged_task<> 的特化一一局部类定义

#include <iostream>
#include <future>
#include <vector>

using namespace std;
namespace std{

template<> 
class packaged_task<std::string(std::vector<char>*, int)> {
public:
    template <typename Callable>
    explicit packaged_task(Callable&& f){}
    packaged_task()
    {
        cout<<"packaged_task 构造"<<endl;
    }
    std::future<std::string> get_future(){}
    void operator()(std::vector<char>*, int){}
};

}



int main()
{
    cout<<"packaged_task偏特化"<<endl;
    packaged_task<std::string(std::vector<char>*, int)> pkg;
}
