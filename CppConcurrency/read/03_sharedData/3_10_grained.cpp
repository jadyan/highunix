/*
 * @Author: your name
 * @Date: 2021-01-22 18:00:54
 * @LastEditTime: 2021-01-28 11:08:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/3.10.cpp
 */
//
//  3.10.cpp 锁的粒度
//  Cpp-Concurrency

//  比较操作符中一次锁住一个互斥量


#include <iostream>
#include <mutex>

using namespace std;

class Y {
private:
    int some_detail;
    mutable std::mutex m;
    int get_detail() const {
        std::lock_guard<std::mutex> lock_a(m); // 被锁保护
        return some_detail;
    }
public:
    Y(int sd) : some_detail(sd) {}
    friend bool operator==(Y const& lhs, Y const& rhs) {
        if (&lhs == &rhs)
            return true;
        int const lhs_value = lhs.get_detail();
        int const rhs_value = rhs.get_detail();
        return lhs_value == rhs_value; // 之后比较
    }
};

int main(int argc, char ** argv)
{
    Y a(27);
    Y b(18);
    if(a==b){
        std::cout<<"true"<<std::endl;
    }else{
        std::cout<<"false"<<std::endl;
    }
    return 0;
}