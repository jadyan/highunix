#include <iostream>
#include <string>

/*
条款44：将与参数无关的代码抽离 templates
Factor parameter-independent code out of templates.

templates（模板）产生多个 classes 和多个 functions，所以一些不依赖于 template parameter（模板参数）的模板代码会引起膨胀。
non-type template parameters（非类型模板参数）引起的膨胀常常可以通过用 function parameters（函数参数）或 class data members（类数据成员）替换 template parameters（模板参数）而消除。
type parameters（类型参数）引起的膨胀可以通过让具有相同的二进制表示的实例化类型共享实现而减少。
*/


#include <boost/scoped_array.hpp>

using namespace std;


/*
SquareMatrixBase::invert 仅仅是一个计划用于 derived classes（派生类）以避免代码重复的方法,所以它是 protected 的而不是 public 的。
*/
template<typename T>
class SquareMatrixBase
{
protected:
	SquareMatrixBase(std::size_t n, T* pMem) : size(n), pData(pMem)
	{
	}

	void setDataPtr(T* t)
	{
		cout<<"setDataPtr 函数 : "<<pData<<endl;
	}

private:
	std::size_t size;
	T* pData;
};


template<typename T, std::size_t n>
class SquareMatrix : private SquareMatrixBase<T>
{
public:
	//using  SquareMatrixBase<T>::setDataPtr;
	SquareMatrix() : SquareMatrixBase<T>(n, 0), pData(new T[n * n])
	{
		
		cout<<"SquareMatrix 构造函数"<<endl;
		//setDataPtr(pData.get());
		this->setDataPtr(pData.get());
		//SquareMatrixBase<T>::setDataPtr(pData.get());
	}

private:
	boost::scoped_array<T> pData;
};

int main()
{
	SquareMatrix<double, 5> sm1;
	return 0;
}