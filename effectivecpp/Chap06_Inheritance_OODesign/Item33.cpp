#include <iostream>
/*
Item 33: 避免覆盖（hiding）“通过继承得到的名字”
derived classes 中的名字覆盖 base classes 中的名字，在 public inheritance 中，这从来不是想要的。
为了使隐藏的名字重新可见，使用 using declarations 或者 forwarding functions（转调函数）。
*/


using namespace std;
class Base
{
public:
	virtual void mf1() = 0;
	virtual void mf1(int i) 
	{
		cout<<"父类 mf1(int i)"<<endl;	
	}
};

class Derived : private Base
{
public:
	virtual void mf1()
	{
		cout<<"子类 mf1(int i)"<<endl;	
		// forwarding function.
		// implicitly inline
		Base::mf1(30);
	}
};


int main()
{
	Derived d;
	const int x = 5;

	d.mf1();		// Fine: calls Derived::mf1
	//子类隐藏父类的方法
	//d.mf1(x);		// Error Base::mf1() is hidden.
	return 0;
}