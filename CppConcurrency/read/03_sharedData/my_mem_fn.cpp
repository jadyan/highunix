/*
 * @Author: your name
 * @Date: 2021-01-27 10:09:54
 * @LastEditTime: 2021-01-27 10:55:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/my_mem_fn.cpp
 */



/*
mem_fun	把成员函数转为函数对象，使用对象指针进行绑定
mem_fun_ref	把成员函数转为函数对象，使用对象(引用)进行绑定
mem_fn	把成员函数转为函数对象，使用对象指针或对象(引用)进行绑定
bind	包括但不限于mem_fn的功能，更为通用的解决方案，详见目录里std::bind的文章
*/
#include <functional>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <functional>

 
using namespace std;
using namespace std::placeholders;    // adds visibility of _1, _2, _3,...
 
 
// 定义一个类Foo用来测试，适配其成员函数
// 其中函数pln(x)是输出x，并换行的意思； 
class Foo
{
public:
    // 无参数member function
    void print() { 
		cout<<"print 函数"<<endl;
		cout<<"a_ = "<<a_<<endl;
	}   

    // 接受一个参数的member function
    void print2(int i) 
    {
		cout<<"print2 函数"<<endl;
        cout<<"a_ = "<<a_<<endl;
        cout<<"i = "<<i<<endl;
    }

    int a_{ 100 };
};


void mem_fun_test()
{
	// 1. mem_fun is for a pointer to an obj.
	// 定义一个容器，存入几个Foo*, 然后使用mem_fun来取指针并绑定到Foo的成员函数
	vector<Foo*> fpv;
	fpv.push_back(new Foo());
	fpv.push_back(new Foo());
	fpv.push_back(new Foo());
	fpv.push_back(new Foo());

	for_each(fpv.begin(), fpv.end(), mem_fun(&Foo::print));
	// mem_fun_ref用来绑定的target应该为Foo对象，而不是指针
	//for_each(fpv.begin(), fpv.end(), mem_fun_ref(&Foo::print));   // error. 

	cout<<"xxxxxxxxxxxbindxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
	for_each(fpv.begin(), fpv.end(), bind(&Foo::print, std::placeholders::_1));    // also can use bind
	cout<<"xxxxxxxxxxxbind2xxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
	//如果成员函数接受额外参数（不仅仅是对象本身）, 那么mem_fun就无能为力了，要用bind
	for_each(fpv.begin(), fpv.end(), bind(&Foo::print2, std::placeholders::_1, 10)); // must use bind




	vector<Foo> fv;
	fv.push_back(Foo());
	fv.push_back(Foo());
	fv.push_back(Foo());
	fv.push_back(Foo());
	
	// mem_fun 不能用来绑定对象.
	// for_each(fv.begin(), fv.end(), mem_fun(&Foo::print));    // error.
	for_each(fv.begin(), fv.end(), bind(&Foo::print, std::placeholders::_1));      // bind也可以


	// 如果成员函数接受额外参数（不仅仅是对象本身）, 那么mem_fun就无能为力了，要用bind
	//for_each(fv.begin(), fv.end(), mem_fun_ref(&Foo::print2, 10));
	for_each(fv.begin(), fv.end(), bind(&Foo::print2, std::placeholders::_1, 10)); // bind可以有很多参数

	// 3. mem_fn既可以用于指针、引用，还可以用于对象本身，因此在C++11中使用mem_fn可以替代mem_fun和mem_fun_ref.
	for_each(fpv.begin(),   fpv.end(),  mem_fn(&Foo::print));       // ptr
	for_each(fv.begin(), fv.end(), mem_fn(&Foo::print));   

	//for_each(fv.begin(), fv.end(), mem_fn(&Foo::print2, 10));
	for_each(fv.begin(), fv.end(), bind(&Foo::print2, std::placeholders::_1, 10)); //must use bind

}



void findEmptyString(const vector<string>& strings)
{
	auto end = strings.end();
	//此处的成员函数指针要表示为相应的成员函数的形式，且使用mem_fn进行包装
	auto it = find_if(strings.begin(), end, mem_fn(&string::empty) );  
 
	if ( it == end)
	{
		cout<<"no empty strings~"<<endl;
	}else
	{
		cout<<"empty string at position: "<<it - strings.begin()<<endl;
	}
}
 
int main()
{
	vector<string> myVector;
	string one = "buaa";
	string two = "";
	myVector.push_back(one);
	myVector.push_back(one);
	myVector.push_back(two);
	myVector.push_back(one);
 
	findEmptyString(myVector);

	cout<<"xxxxxxxxxxxmem_funxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
	mem_fun_test();
 
	return 0;
}

