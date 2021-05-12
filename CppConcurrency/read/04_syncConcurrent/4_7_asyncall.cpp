/*
 * @Author: jadyan
 * @Date: 2021-02-02 13:36:45
 * @LastEditTime: 2021-02-02 17:28:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/04_syncConcurrent/4.7.cpp
 */

//  4.7.cpp 使用期望等待一次性事件->带返回值的后台任务
//  Cpp-Concurrency
//
//  使用 std::async 向函数传递参数

#include <iostream>
#include <string>
#include <future>
#include <chrono>
using namespace std;


struct X {
    void foo(int i , std::string const& str)
    {
        cout<<"foo 函数"<<endl;
        cout<<"i= "<<i<<"str="<<str<<endl;
    }
    std::string bar(std::string const& str)
    {
        cout<<"bar 函数"<<endl;
        cout<<"str = "<<str<<endl;
    }
};




struct Y {
    double operator()(double d)
    {
        cout<<"括号重载函数: d = "<<d<<endl;
    }
};


X baz(X& x) 
{
    cout<<"xxxxx baz 函数"<<endl;

}

void testY()
{
    Y y;
    auto f3 = std::async(Y(), 3.141); // 调用tmpy(3.141), tmpy通过移动 Y 的移动构造函数得到
    auto f4 = std::async(std::ref(y), 2.718); // 调用y(2.718)

    f3.get();
    f4.get();

    X x;
    auto f6 = std::async(baz, std::ref(x)); // 调用baz(x)
    f6.get();
}



#if 1
class move_only {
public:
    move_only()
    {
        cout<<"move_only 无参构造"<<endl;
    }
    move_only(move_only&&)
    {
        cout<<"move_only&& 函数"<<endl;
    }
    move_only(move_only const&) = delete;
    move_only& operator=(move_only&&);
    move_only& operator=(move_only const&) = delete;
    void operator()()
    {
        cout<<"move_only operator() 函数"<<endl;
    }
};


void testmoveonly()
{
    X x;
    cout<<"testmoveonly 函数："<<endl;
    auto f5 = std::async(move_only()); // 调用tmp(), tmp是通过 std::move(move_only()) 构造得到
   
    auto f7 = std::async(std::launch::async, Y(), 1.2); // 在新线程上执行
    
    auto f8 = std::async(std::launch::deferred, baz, std::ref(x)); // 在wait() 或 get()调用时执行
    auto f9 = std::async(std::launch::deferred | std::launch::async, baz, std::ref(x)); // 实现选择执行方式

    
    auto f10 = std::async(baz, std::ref(x));
    f5.get();
    cout<<"xxxxxxxxxxx"<<endl;
    f7.get();
    cout<<"xxxxxxxxxxx"<<endl;
    f8.get();

    cout<<"xxxxxxxxxxx"<<endl;
    f9.get();

}


// f8.wait(); // 调用延迟函数
#endif



int main()
{

    X x;
    auto f1 = std::async(&X::foo, &x, 42, "hello"); // 调用 p->foo(42, "hello"), p 是指向 x 的指针
    auto f2 = std::async(&X::bar, x, "goodbye"); // 调用tmpx.bar("goodbye"), tmpx 是 x 的拷贝副本
    f1.get();
    //f2.get();
    //std::this_thread::sleep_for(std::chrono::seconds(2));

    testY();
    testmoveonly();




    return 0;
}