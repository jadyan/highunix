/*
 * @Author: your name
 * @Date: 2021-03-31 09:15:39
 * @LastEditTime: 2021-03-31 09:50:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/12_02_differs_exception_parameter_calling_a_virtual_function.cpp
 */
#include <iostream>
using namespace std;


class Widget
{
public:
    Widget():m(0)
    {
        cout<<"Widget 构造函数"<<endl;
    }

    Widget(const  Widget & wi)
    {
        this->m = wi.m;
        cout<<"Widget 拷贝构造函数"<<endl;
    }

    ~Widget()
    {
        cout<<"Widget 析构函数"<<endl;
    }
    
    void setm(int i)
    {
        this->m = i;
    }

    void print()
    {
        cout<<"this m is = "<< this->m<<endl;
    }
    
private:
    int m;
};


class SpecialWidget: public Widget 
{
public:
    SpecialWidget():n(0),Widget()
    {
        cout<<"SpecialWidget 构造函数"<<endl;
    }
    SpecialWidget(const SpecialWidget &sp):Widget(sp)
    {
        this->n = sp.n;
        cout<<"SpecialWidget 拷贝构造函数"<<endl;

    }
    
    ~SpecialWidget()
    {
        cout<<"SpecialWidget 析构函数"<<endl;
    }
private:
    int n;
};

void passAndThrowWidget()
{
    static Widget localWidget; // 现在是静态变量（static）;
    localWidget.setm(3);
    //一直存在至程序结束
    throw localWidget; // 仍将对 localWidge 进行拷贝操作
}


void passAndThrowWidget2()
{
    SpecialWidget localSpecialWidget;
    Widget& rw = localSpecialWidget; // rw 引用 SpecialWidget
    throw rw; //它抛出一个类型为 Widget
}


void test1()
{
    try{
        passAndThrowWidget();
    }
    catch(Widget &wi)  //此处不加引用，也会调用拷贝构造
    {
        //cout<<"catch "<<<<endl;
        wi.print();
        //throw ; //继续往上抛
    }
}


void test2()
{
    try{
        passAndThrowWidget2();
    }
    catch(Widget &wi)  //此处不加引用，也会调用拷贝构造
    {
        //cout<<"catch "<<<<endl;
        wi.print();
    }
}

int main(int argc, char** argv)
{
    //test1();
    test2();
}