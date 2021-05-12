#include <iostream>


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
	Widget(const Widget& rhs);
	Widget& operator=(const Widget& rhs)
	{
		std::cout<<"=运算符重载调用"<<std::endl;
		*pImpl = *(rhs.pImpl);
		return *this;
	}
	void show()
	{
		(*pImpl).show();		
	}

private:
	WidgetImpl* pImpl;
};


int main()
{
	Widget wg1(new WidgetImpl(1,2,3));
	Widget wg2(new WidgetImpl(11,22,23));	
	wg1.show();
	wg2.show();	
	wg1=wg2;
	wg1.show();
	wg2.show();	
	
}
