/*
 * @Author: your name
 * @Date: 2021-03-30 15:17:09
 * @LastEditTime: 2021-03-30 15:48:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/10_not_Resource_leakage_in_constructors.cpp
 */
//两种情况下调用析构函数
//(1) 对象正常结束或者显式调用delete
//(2) 异常处理系统删除一个对象
//
//
//如果异常在析构函数中抛出，没有被Session的析构函数捕获
//catch(...)可以捕获所有异常，如果不向外层throw,则这些异常被忽略

//https://blog.csdn.net/lisonglisonglisong/article/details/44276343


// 在构造函数中抛出异常是C++中通知对象构造失败的唯一方法。

// 构造函数中抛出异常，对象的析构函数将不会被执行。

// 构造函数抛出异常时，本应该在析构函数中被delete的对象没有被delete，会导致内存泄露。

// 当对象发生部分构造时，已经构造完毕的子对象（非动态分配）将会逆序地被析构。



#include <iostream>
#include <string>
#include <exception>
using namespace std;
 
/******************类定义**********************/
class person {
public:
	person(const string& str):name(str)
	{
		throw "测试：在构造函数中抛出一个异常";
        //在异常点以后的语句都没有执行，抛出异常后，析构函数也不会执行
		cout << "构造一个对象！" << endl;
	};
	~person()
	{
		cout << "销毁一个对象！" << endl;
	};
private:
	string name;
};
 
/******************测试类**********************/
int main()
{
	try 
	{
		person me("songlee");
	} 
    //catch(exception e)
	catch(const char * e) 
	{
		cout << e << endl;
	};
	//getchar();
	return 0;

}

//总结：避免异常在析构函数内部抛出
//如果因为try发生异常在stack-unwinding过程中调用了析构函数释放资源，结果析构中也出现并抛出了异常
//则会导致调用terminate函数，直接终止程序，导致资源无法释放,出现内存泄露
//通过捕获并忽略异常保证够在异常转递的stack-unwinding过程中，防止 terminate 被调用，保证资源能够正常释放
//
//stack-unwinding通常和异常处理相关，指的是scope结束，编译器依次销毁scope中的资源
//Reference:https://stackoverflow.com/questions/2331316/what-is-stack-unwinding
//参考代码:stack-unwinding.cpp