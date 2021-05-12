/*
 * @Author: your name
 * @Date: 2021-04-02 14:05:18
 * @LastEditTime: 2021-04-02 16:47:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/24_Understand_the_costs_of_virtual_functions.cpp
 */
#include <iostream>
#include <string>
#include <functional>

using namespace std;


/*
必须为每个包含虚函数的类的 virtual
talbe 留出空间。类的 vtbl 的大小与类中声明的虚函数的数量成正比（包括从基类继承的
虚函数）。每个类应该只有一个 virtual table，所以 virtual table 所需的空间不会太大，
但是如果你有大量的类或者在每个类中有大量的虚函数，你会发现 vtbl 会占用大量的地址
空间。

更普通的设计方法是采用启发式算法来决定哪一个 object 文件应该包含类的 vtbl。通
常启发式算法是这样的：要在一个 object 文件中生成一个类的 vtbl，要求该 object 文件
包含该类的第一个非内联、非纯虚拟函数（non-inline non-pure virual function）定义
（也就是类的实现体）。因此上述 C1 类的 vtbl 将被放置到包含 C1::~C1 定义的 object 文件
里（不是内联的函数） C2 类的 vtbl 被放置到包含 C1::~C2 定义的 object 文件里（不是内
联函数）
析构函数不是内联函数？


虚函数不能是内联的。这
是因为“内联”是指“在编译期间用被调用的函数体本身来代替函数调用的指令，”但是虚
函数的“虚”是指“直到运行时才能知道要调用的是哪一个函数。


*/

/*
            |-------->implementation of C1:~C1()
C1's vtbl   |-------->implementation of C1:f1()
            |-------->implementation of C1:f2()
            |-------->implementation of C1:f3()
*/
class C1 {
public:
    C1(){};

    virtual void f1()
    {
        cout<<"C1:f1 函数"<<endl;
    }
    virtual int f2(char c)const
    {
        cout<<"C1:f2 函数"<<endl;
    } 
    virtual void f3(const string& s)
    {
        cout<<"C1:f3 函数"<<endl;
    }
    void f4() const
    {
         cout<<"C1:f4 函数"<<endl;
    }
#if 1
    virtual ~C1()
    {
        cout<<"C1:~C1 函数"<<endl;
    }
#endif
};


/*
            |-------->implementation of C2:~C2()
C2's vtbl   |-------->implementation of C2:f1()
            |-------->implementation of C1:f2()
            |-------->implementation of C1:f3()
            |-------->implementation of C2:f5()
*/

class C2: public C1 {
public:
    C2(){}; // 非虚函数
    virtual ~C2()
    {
        cout<<"C2:~C2函数"<<endl;
    }
    virtual void f1() // 重定义函数
    {
        cout<<"C2:f1函数"<<endl;
    }
    virtual void f5(char *str)
    {
        cout<<"C2:f5函数"<<endl;
    }
};


//typedef std::function<void()> Functional; 
typedef void (*voidfun)(void);

int main(int argc, char** argv)
{
cout<<"----没有继承----"<<endl;
    C1 c1;
    cout<<"sizeof(c1) = "<<sizeof(c1)<<endl;
    long *p=(long*)&c1;
    voidfun f=(voidfun)*(long*)*p;
    f();
    voidfun f1 = (voidfun)*((long*)*p+1);
    f1();
    voidfun f2 = (voidfun)*((long*)*p+2);
    f2();
    voidfun f3 = (voidfun)*((long*)*p+3);
    f3();
cout<<"\n\n\n----单继承单指针----"<<endl;
    C2 c2;
    cout<<"sizeof(c2) = "<<sizeof(c2)<<endl;
    long *p2 = (long*)&c2;

    //C2:f1函数
    voidfun f20 = (voidfun)*((long*)*p2);
    f20();

    //C1:f2 函数
    voidfun f21 = (voidfun)*((long*)*p2+1);
    f21();

    //C1:f3 函数
    voidfun f22 = (voidfun)*((long*)*p2+2);
    f22();

    //C2:~C2函数  C1:~C1 函数 调用子类的析构 一定会调用父类的析构
    voidfun f23 = (voidfun)*((long*)*p2+3);
    //f23();

    //C1:~C1 函数 不能直接调用父类的析构函数 f24是父类的析构
    voidfun f24 = (voidfun)*((long*)*p2+4);
    //f24();
    
    voidfun f25 = (voidfun)*((long*)*p2+5);
    f25();


cout<<"\n\n\n\n\n---end---"<<endl;


}
