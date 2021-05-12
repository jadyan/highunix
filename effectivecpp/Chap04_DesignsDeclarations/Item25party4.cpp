#include <iostream>

#include <algorithm>

template<typename T>
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


template<typename T>
class Widget
{
public:
	Widget(WidgetImpl<T>* wi) : pImpl(wi) {}
	void swap(Widget& other)
	{
		std::cout<<"Widget 成员swap函数"<<std::endl;
		using std::swap;
		swap(pImpl, other.pImpl);
	}

	void show()
	{
		(*pImpl).show();		
	}
private:
	WidgetImpl<T>* pImpl;
};


namespace std
{
	// error C2768: 'std::swap' : illegal use of explicit template arguments
	// this code should not compile although some compilers erroneously accept it

	/*
		它是非法的。试图部分特化（partially specialize）一个函数模板（std::swap），
		但是尽管 C++ 允许类模板的部分特化（partial specialization），
		但不允许函数模板这样做。这样的代码不能编译（尽管一些编译器错误地接受了它）
		模板函数不存在偏特化


		这时以下的写法会企图偏特化一个 function template，但 C++ 只允许对 class template 偏特化：
		所以想要偏特化一个 function template 的惯用做法是添加一个重载版：
		https://www.kancloud.cn/jerakrs/effective-c/1752199
	*/
	// template<typename T>
	// void swap<Widget<T> >(Widget<T>& a, Widget<T>& b)
	// {
	// 	a.swap(b);
	// }

	/*
		相当于重载函数模板
	*/
	template<class T>
	void swap(Widget<T>& a, Widget<T>& b)
	{

		//std::cout<<"std 重载swap函数"<<std::endl;
		//std::cout<<"调用重构 std swap"<<std::endl;
		printf("调用重构 std swap\n");
		a.swap(b);
	}

}




int main()
{
	WidgetImpl<int> wi1(1, 2, 3);
	WidgetImpl<int> wi2(2, 4, 6);

	Widget<int> w1(&wi1);
	Widget<int> w2(&wi2);

	w1.show();
	w2.show();
	std::swap(w1, w2);
	w1.show();
	w2.show();
	return 0;
}
