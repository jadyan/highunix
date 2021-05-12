/*
 * @Author: your name
 * @Date: 2021-04-02 16:40:43
 * @LastEditTime: 2021-04-02 17:15:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/24_02_Understand_the_costs_of_virtual_functions.cpp
 */
#include <iostream>
#include <string>
#include <functional>

using namespace std;

/********************************************************
编译器为每一个拥有虚函数的类的实例创建了一个虚函数表（virtual table），
这个表中有许多的槽（slot），
每个槽中存放的是虚函数的地址。虚函数表解决了继承、覆盖、添加虚函数的问题，保证其真实反应实际的函数。

上面定义了一个Base类，其中有三个虚函数。我们将Base类对象取址 &b 并强制转换为 int*，
取得虚函数表的地址。然后对虚函数表的地址取值 *vptr 并强转为 int*，
即取得第一个虚函数的地址了。将第一个虚函数的地址加1，取得第二个虚函数的地址，再加1即取得第三个虚函数的地址。
注意，之所以可以通过对象实例的地址得到虚函数表，
是因为 vptr 指针位于对象实例的最前面（这是由编译器决定的，
主要是为了保证取到虚函数表有最高的性能——如果有多层继承或是多重继承的情况下）。如图所示：
*********************************************************/
class Base_virtual
{
public:
	//long ctmp;
	virtual void x() { std::cout<< "Base_virtual::x()"<< std::endl; }
	virtual void y() { cout<< "Base_virtual::y()"<< endl; }
	virtual void z() { cout<< "Base_virtual::z()"<< endl; }
};	
class Base_virtual2
{
public: 
	virtual void x() { std::cout<< "Base_virtual2::x()"<< std::endl; }
    virtual void y() { cout<< "Base_virtual2::y()"<< endl; }
    virtual void z() { cout<< "Base_virtual2::z()"<< endl; }
};
class Derive:  public Base_virtual,   public Base_virtual2
{
public:
	virtual void x()  { cout << "Derive::x()" << endl; }
	virtual void x1() { cout << "Derive::x1()" << endl; }
	virtual void y2() { cout << "Derive::y1()" << endl; }
};
typedef void(*pFun)(void);



int main(int argc, char ** argv)
{
	cout<<"\n\n虚函数表地址 test!!! \n"<<endl;

	Base_virtual b;
	void(*func1)(void);
	long* vptr = (long*)&b;                     // 虚函数表地址
	
	cout<<"sizeof = "<<sizeof(b)<<endl;;
	func1 = (void(*)(void)) *((long*) *vptr);        // 第一个函数
	
	pFun func2 = (pFun)* ((long*) *vptr+1);      // 第二个函数
	pFun func3 = (pFun)* ((long*) *vptr+2);      // 第三个函数
	func1();     // 输出Base_virtual::x()
	func2();     // 输出Base_virtual::y()
	func3();     // 输出Base_virtual::z()

cout<<"\n\n 多继承虚函数表地址 test!!! \n"<<endl;
	Derive derive;
	long **vptr2 = (long**)&derive;
	cout<<"derive sizeof = "<<sizeof(derive)<<endl;;
	cout<<"pFun sizeof = "<<sizeof(pFun)<<endl;;

    cout<<"\n\n\n-------多重继承虚函数表-------"<<endl;
    pFun dfunc00  = (pFun)*(*(vptr2+0)+0);
    dfunc00();
    
    pFun dfunc01 =  (pFun)*(*(vptr2+0)+1);
    dfunc01();

    pFun dfunc02  = (pFun)*(*(vptr2+0)+2);
    dfunc02();

    pFun dfunc03  = (pFun)*(*(vptr2+0)+3);
    dfunc03();

    pFun dfunc04  = (pFun)*(*(vptr2+0)+4);
    dfunc04();


    cout<<"----1----"<<endl;
    pFun dfunc10 = (pFun)*(*(vptr2+1)+0);
    dfunc10();

    pFun dfunc11 = (pFun)*(*(vptr2+1)+1);
    dfunc11();

    pFun dfunc12 = (pFun)*(*(vptr2+1)+2);
    dfunc12();

#if 0
    /*
    虚函数表 多重继承的话，虚函数表第一维第一个继承类合在一起， 把第二个父类放在第二维
    */
    cout<<"----2----"<<endl;
    pFun dfunc20  = (pFun)*(*(vptr2 + 2)+0);
    dfunc20();

    pFun dfunc21  = (pFun)*(*(vptr2 + 2)+1);
    dfunc21();

    pFun dfunc22  = (pFun)*(*(vptr2 + 2)+2);
    dfunc22();
#endif

    cout<<"\n\n\n-------多重继承虚函数表end-------"<<endl;

}





