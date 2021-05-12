/*
 * @Author: your name
 * @Date: 2021-03-30 17:00:56
 * @LastEditTime: 2021-03-30 23:10:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/my_teminate.cpp
 */


/*
https://blog.csdn.net/rain_qingtian/article/details/10133721
C++中处理异常的过程是这样的：在执行程序发生异常，可以不在本函数中处理，而是抛出一个错误信息，把它传递给上一级的函数来解
决，上一级解决不了，再传给其上一级，由其上一级处理。如此逐级上传，直到最高一级还无法处理的话，运行系统会自动调用系统函数terminate，
https://blog.51cto.com/lihaichuan/1198702
*/


#include <iostream>
#include <exception>
using namespace std;

void terminatorb()  //函数名字可以任意，不一定非得是terminate
{
    cout<<"I will be back!"<<endl;
    exit(0);
}

//void (*old_terminatea)()=set_terminate(terminatorb);

//int  *p =new int(3);

class Botch
{
    public:
    class Fruit {};
    void f()
    {
        cout<<"Botch::f()"<<endl;
        //throw "C yichan";
        throw Fruit();
    }
    //不能再析构函数内抛异常，因为这是程序结束，后面不会再有处理异常的地方了
    //析构中异常找不到catch
    ~Botch() 
    {
        throw "c";
    }
};

int main()
{
    set_terminate(terminatorb);
    Botch b;
    try{
        //Botch b;  //局部变量除了{}，所以才只显示析构函数显示的信息
        b.f();
    }
    catch(const char * c)
    {
        cout<<"异常"<<c<<endl;
    }
    catch(...)
    {
        cout<<"inside cathc(...)"<<endl;
    }
}