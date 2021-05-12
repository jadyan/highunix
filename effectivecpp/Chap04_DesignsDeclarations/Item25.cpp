#include <iostream>
#include <algorithm>


/**
 *
 * 条款25：考虑写出一个不抛异常的 swap 函数
 *Consider support for a non-throwing swap.
 *
 * **/

#if 0

class WidgetImpl
{
public:
	WidgetImpl(int x, int y, int z) :
	  a(x), b(y), c(z)
	{
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
		*pImpl = *(rhs.pImpl);
		return *this;
	}

private:
	WidgetImpl* pImpl;
};

#endif




namespace WidgetStuff
{
	template<typename T>
	class WidgetImpl
	{
	public:
		WidgetImpl(int x, int y, int z) :
		  a(x), b(y), c(z)
		{
		}

	private:
		int a, b, c;
	};


	template<typename T>
	class Widget
	{
	public:
		Widget(WidgetImpl<T>* wi) : pImpl(wi) {}
		void swap(Widget& other)
		{
			using std::swap;
			swap(pImpl, other.pImpl);
		}

	private:
		WidgetImpl<T>* pImpl;
	};


	template<typename T>
	void swap(Widget<T>& a, Widget<T>& b)
	{
		a.swap(b);
	}
}

int main()
{
	WidgetStuff::WidgetImpl<int> wi1(1, 2, 3);
	WidgetStuff::WidgetImpl<int> wi2(2, 4, 6);

	WidgetStuff::Widget<int> w1(&wi1);
	WidgetStuff::Widget<int> w2(&wi2);

	WidgetStuff::swap(w1, w2);
	return 0;
}
