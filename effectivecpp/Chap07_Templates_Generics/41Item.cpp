
#include <iostream>
#include <string>

/**
 * 条款41：了解隐式接口和编译期多态
 * 
 * 显式接口由函数的签名式（函数名称、参数类型、返回类型）构成；
 * 隐式接口不基于函数的签名式，它由有效表达式（valid expressions）组成；
 * 
 * **/

using namespace std;
class Widget
{
public:
	Widget() : _size(11){}
	Widget(std::size_t theSize) : _size(theSize){}
	virtual ~Widget()	{}

	virtual std::size_t size() const
	{
		return _size;
	}
	virtual void normalize()
	{
		cout<<"normalize 函数"<<endl;
	}
	void swap(Widget& other)
	{
		cout<<"swap  函数"<<endl;
		_size = other.size();
	}

private:
	std::size_t _size;
};

template<typename T>
void doProcessing(T& w)
{
	if (w.size() > 10)
	{
		T temp(w);
		temp.normalize();
		temp.swap(w);
	}
}

int main()
{
	Widget w(20);
	doProcessing(w);
	return 0;
}