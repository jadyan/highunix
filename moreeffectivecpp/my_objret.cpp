/*
 * @Author: your name
 * @Date: 2021-04-01 14:17:23
 * @LastEditTime: 2021-04-01 14:23:58
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/my_objret.cpp
 */
#include <iostream>
#include <string>

using namespace std;

class Base{
public:
    Base(int i=3):m_i(i)
    {
        cout<<"构造函数"<<endl;
    }
    Base(const Base &b)
    {
        this->m_i= b.m_i;
        cout<<"拷贝构造函数"<<endl;
    }
    ~Base()
    {
        cout<<"析构函数"<<endl;
    }
private:
    int m_i;
};

Base testReturnValue()
{
    Base b(18);
    return b;
}

int main(int argc, char ** argv)
{
    /* 
    编译器优化，不会调用拷贝构造函数
    */
    Base b = testReturnValue();
    Base b1(19);
    cout<<"----"<<endl;
    /*
    这里会调用拷贝构造函数
    */
    Base b2=b1;
}