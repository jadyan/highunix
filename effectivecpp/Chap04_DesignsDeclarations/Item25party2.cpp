#include <iostream>

#include <algorithm>

class WidgetImpl
{
public:
	WidgetImpl(int x, int y, int z) :
	  a(x), b(y), c(z)
	{
	}

	void show()
	{
		std::cout<<"a="<<a<<"b="<<b<<"c="<<c<<std::endl;
	}
private:
	int a, b, c;
};


class Widget
{
public:
	Widget(WidgetImpl* wi) : pImpl(wi) {}
	void swap(Widget& other)
	{
		//using std::swap;
		std::cout<<"swap 函数调用"<<std::endl;
		std::swap(pImpl, other.pImpl);
	}
	void show()
	{
		(*pImpl).show();		
	}
private:
	WidgetImpl* pImpl;
};


/*
 *这个函数开头的 "template<>" 表明这是一个针对 std::swap 的完全模板特化（total template specialization）
 （某些书中称为 "full template specialization" 或 "complete template specialization" ——译者注），
 函数名后面的 "<Widget>" 表明特化是在 T 为 Widget 类型时发生的。换句话说，当通用的 swap 模板用于 Widgets 时，就应该使用这个实现。通常，我们改变 std namespace 中的内容是不被允许的，但允许为我们自己创建的类型（就像 Widget）完全特化标准模板（就像 swap）。这就是我们现在在这里做的事情
 
 * */
namespace std
{
	// total template specialization for std::swap
	//特化(全特化)
	template<>
	void swap<Widget>(Widget& a, Widget& b)
	{
		std::cout<<"调用重构(特化) std swap"<<std::endl;
		a.swap(b);
	}
}

int main(int argc, char ** argv)
{
	Widget wg1(new WidgetImpl(1,2,3));
	Widget wg2(new WidgetImpl(11,22,23));	
	wg1.show();
	wg2.show();	
	//wg1.swap(wg2);
	std::swap(wg1, wg2);
	wg1.show();
	wg2.show();	
}
