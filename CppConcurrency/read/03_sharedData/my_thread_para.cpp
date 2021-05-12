/*
 * @Author: your name
 * @Date: 2021-01-26 17:21:53
 * @LastEditTime: 2021-01-26 18:11:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/my_thread_para.cpp
 */



/*
https://www.codenong.com/js816df045f93e/
*/
#include <iostream>
#include <thread>



class A
{
public:
    A(int a):m_a(a)
    {
        std::cout << "A的构造函数，线程：" << std::this_thread::get_id() << ",this:"<<this << std::endl;
    }

    A(const A &a)
    {
        m_a = a.m_a;
        std::cout << "A的复制构造函数，线程：" << std::this_thread::get_id() << ",this:" << this << std::endl;
    }
    ~A()
    {
        std::cout << "A的析构函数，线程：" << std::this_thread::get_id() << ",this:" << this << std::endl;
    }
    int m_a;
};

void f(const A& a)
{
    std::cout << "启动线程f:" << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "结束线程f:" << std::this_thread::get_id() << std::endl;
}
int main()
{

    std::cout << "主线程启动：" << std::this_thread::get_id() << std::endl;
    {
        A a(5);

        /*
        临时对象的创建是在子线程中进行的，
        如果指向动态变量的指针作为参数传递给线程，
        且在子线程还没有使用该指针指向的资源之前，资源就被释放掉了，程序就发生不可预知的后果
        */
        //int a = 10;

        //std::thread threadF(f, std::ref(a));
            /*
            结果
            线程启动：140331465422656
            A的构造函数，线程：140331465422656,this:0x7ffc7c371270
            启动线程f:140331444352768
            结束线程f:140331444352768
            A的析构函数，线程：140331465422656,this:0x7ffc7c371270
            */
        std::thread threadF(f, a);
            /*
                原因 要拷到线程自己的堆栈，为什么会有两次？

                主线程启动：140711280527168
                A的构造函数，线程：140711280527168,this:0x7ffc28858220
                A的复制构造函数，线程：140711280527168,this:0x7ffc288581e0
                A的复制构造函数，线程：140711280527168,this:0x1c16040
                A的析构函数，线程：140711280527168,this:0x7ffc288581e0
                启动线程f:140711259457280
                结束线程f:140711259457280
                A的析构函数，线程：140711259457280,this:0x1c16040
                A的析构函数，线程：140711280527168,this:0x7ffc28858220
            */
        threadF.join();
    }
}


