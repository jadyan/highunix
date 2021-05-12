/*
 * @Author: your name
 * @Date: 2021-03-31 10:09:33
 * @LastEditTime: 2021-03-31 10:23:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/13_catch_exceptions_by_reference.cpp
 */
  
//通过引用捕获异常
//通过指针捕获异常不会因为拷贝对象导致的效率下降，但是带来的问题是：
//抛出的指针不能指向一个局部对象。程序员应该在全局或者堆上定义这样一个
//变量才能保证不会因为scope结束而被销毁。但有带来了新的问题，捕获该异常
//的用户并不知道该指针是来自于堆上的变量还是全局的，是否应该delete。如果
//不是堆上建立的异常对象，则delete会不可预测


//总结：
//(1)值捕获一方面每次抛出发生两次拷贝，另外可能出现派生类到基类的截断现象
//(2)指针捕获无法确定是否释放异常对象
//(3)通过引用捕获是最靠谱的

#include <iostream>
using namespace std;


struct MyException : public exception
{
    const char * what () const throw ()
    {
        return "C++ Exception";
    }
};



void someFunction()
{
    /*
        了能让程序正常运行，程序员定义异常
        对象时必须确保当程序控制权离开抛出指针的函数后，对象还能够继续生存。全局与静态对
        象都能够做到这一点
    */
    //static MyException ex; // 异常对象
    //throw &ex;
    
    MyException *ex = new  MyException();
    throw ex; // 抛出一个指针，指向 ex

    //MyException ex; // 异常对象
    //throw &ex;

}

void doSomething()
{
    try {
        someFunction(); // 抛出一个 exception*
    }
    catch (MyException *ex) { // 捕获 exception*;
        std::cout << "MyException caught" << std::endl;
        std::cout << ex->what() << std::endl;
    }
}

void pointexception()
{
    doSomething();
}

int main(int argc, char ** argv)
{
    pointexception();
    return 0;
}