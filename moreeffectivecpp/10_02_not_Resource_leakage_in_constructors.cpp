/*
 * @Author: your name
 * @Date: 2021-03-30 15:49:48
 * @LastEditTime: 2021-03-30 16:05:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/10_02_not_Resource_leakage_in_constructors.cpp
 */



// 造函数中抛出异常，对象的析构函数将不会被执行。
// 构造函数抛出异常时，本应该在析构函数中被delete的对象没有被delete，会导致内存泄露。
// 当对象发生部分构造时，已经构造完毕的子对象（非动态分配）将会逆序地被析构。


/*
https://blog.csdn.net/lisonglisonglisong/article/details/44276343
    C++拒绝为没有完成构造操作的对象调用析构函数，即使你使用了delete语句。
    由于 Tester 的析构函数不会执行，所以给A对象 a 动态分配（new）的空间无法释放，将造成内存泄露。

    在构造函数中抛出异常，很危险， c++ 不会对没有构造完成的对象，调用析构，即使调用了delete方法
*/


#include <string>
#include <iostream>
#include <exception>
using namespace std;
 
class A {
public:
	A(){};
};
 
class B {
public:
	B() { 
        //// 造函数中抛出异常，对象的析构函数将不会被执行。
		throw ("测试：在B的构造函数中抛出一个异常");
		cout << "构造 B 对象!" << endl;
	};
	~B(){ cout << "销毁 B 对象!" << endl; };
};
 
class Tester {
public:
	Tester(const string& name, const string& address)
    {
        a = new A();
        b = new B();
        cout << "构造 Tester 对象!" << endl;
    }
	~Tester()
    {
        delete a;
        delete b;
        cout << "销毁 Tester 对象!" << endl;
    }
private:
	string theName;
	string theAddress;
	A *a;
	B *b;

};


/*
    C++拒绝为没有完成构造操作的对象调用析构函数，即使你使用了delete语句。
    由于 Tester 的析构函数不会执行，所以给A对象 a 动态分配（new）的空间无法释放，将造成内存泄露。
*/

int main()
{
	Tester *tes = NULL;
	try 
	{
		tes = new Tester("songlee","201");
	} 
	catch(const char * e) 
	{
		cout << e << endl;
	};
	delete tes; // 删除NULL指针是安全的
    cout<<"done"<<endl;
	return 0;
}
