#include<iostream>
/*
条款42：了解 typename 的双重含义
// 从属名称（dependent names）：template 内相依赖于某个 template 参数；
// 嵌套从属名称（nested dependent name）：从属名称在 class 内呈嵌套状；
typename另外一个作用为：使用嵌套依赖类型(nested depended name)
*/


template<typename T>
class Base
{
};

template<typename T>
class Derived : Base<T>::Nested
{
public:
	explicit Derived(int x) : Base<T>::Nested(x)
	{
        //从属名称（dependent names）：template 内相依赖于某个 template 参数；
		typename Base<T>::Nested temp;
	}
};


template<typename IterT>
void workWithIterator(IterT iter)
{
    /*
    https://blog.csdn.net/gatieme/article/details/50946005
    这里的 typename 告诉编译器，T::innerClass 是一个类，
    程序要声明一个 T::innerClass 类的对象，而不是声明 T 的静态成员，而 typename 如果换成 class 则语法错误。
	*/
    typename std::iterator_traits<IterT>::value_type temp(*iter);
}

template<typename IterT>
void workWithIterator2(IterT iter)
{

    /*
    https://www.cnblogs.com/zhuyf87/archive/2013/03/04/2942209.html
    这个时候typename的作用就是告诉c++编译器，typename后面的字符串为一个类型名称，而不是成员函数或者成员变量，
    这个时候如果前面没有typename，
    编译器没有任何办法知道T::LengthType是一个类型还是一个成员名称（静态数据成员或者静态函数），所以编译不能够通过。
    */
	typedef typename std::iterator_traits<IterT>::value_type value_type;
	value_type temp(*iter);
}

int main()
{
	return 0;
}