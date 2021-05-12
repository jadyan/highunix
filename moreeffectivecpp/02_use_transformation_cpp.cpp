/*
 * @Author: your name
 * @Date: 2021-03-10 20:36:53
 * @LastEditTime: 2021-03-10 20:56:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/02_use_transformation_cpp.cpp
 */
// 02_01_Cpp风格Cast.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
using namespace std;

class Widget {
	virtual void printInfo() { cout << "Widget!" << endl; };
};
class SpecialWidget :public Widget {
	virtual void printInfo() { cout << "SpecialWidget!" << endl; };
};

void update(SpecialWidget *psw)
{
    std::cout<<"update point function"<<std::endl;
}
void updateViaRef(SpecialWidget &rsw)
{
    std::cout<<"update ref function"<<std::endl;
}

int doSomething()
{
    std::cout<<"doSomething function"<<std::endl;
}

int main()
{
	SpecialWidget sw;
	const SpecialWidget & csw = sw;
	//update(&csw); //报错
	update(const_cast<SpecialWidget*>(&csw)); //正确，使用const_cast
	update((SpecialWidget*)&csw); //C风格也可以做到

	Widget *pw = new SpecialWidget;
	//update(pw); //错误，类别出错
	//update(const_cast<SpecialWidget*>(pw));//错误，const_cast只能用于处理constness和volatileness

	//dynamic_cast用于安全的沿着类的继承关系向下进行类型转换，即把基类的指针或引用转换成指向其派生类或兄弟类的指针或引用
	update(dynamic_cast<SpecialWidget*>(pw));//dynamic_cast会检查你的类别是否具有多态性质(虚函数),如果没有会报错
	update((SpecialWidget*)pw); //C风格，不会检查
	update(static_cast<SpecialWidget*>(pw));//直接static_cast也可以

	//updateViaRef(*pw); //报错，类别错误
	updateViaRef(dynamic_cast<SpecialWidget&>(*pw)); //


	//reinterpret_cast用于函数指针的转换
	typedef void(*FuncPtr)();
	FuncPtr funcPtrArray[10];
	
	//funcPtrArray[0] = &doSomething; //报错，类型错误
	funcPtrArray[0] = reinterpret_cast<FuncPtr>(&doSomething);

	funcPtrArray[0] = (void(*)())&doSomething; //C风格



    int firstNameber, secondNumber;
    //double ret= dynamic_cast<double>(firstNameber); //error 未涉及继承

    const SpecialWidget sw2;
    //update(dynamic_cast<SpecialWidget*>(&sw2));   //error 不能改变常量


	return 0;
}