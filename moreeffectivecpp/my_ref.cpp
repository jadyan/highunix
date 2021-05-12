/*
 * @Author: your name
 * @Date: 2021-03-31 22:29:14
 * @LastEditTime: 2021-03-31 22:33:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/my_ref.cpp
 */
#include <iostream>

using namespace std;


class A
{
public:
    A(int i):m(i)
    {
        cout<<"构造"<<endl;
    }
    A(const A &tmp)
    {
        cout<<"拷贝构造"<<endl;
    }
    ~A()
    {
        cout<<"析构函数"<<endl;
    }
    void print()
    {
        cout<<"m = "<<m<<endl;
    }
private:
    int m;

};

A & test()
{
    A a(3);
    A &ra=a;
}

int main()
{
    test().print();
}