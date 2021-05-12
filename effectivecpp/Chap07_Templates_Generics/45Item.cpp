#include <iostream>
#include <string>

//https://github.com/HanxianshengGame/EffectiveCpp/blob/master/Terms/Terms_45.cpp


//条款45：运用成员函数模板接收所有兼容类型
/* 1. 同一个template的不同具现体之间不存在固有关系，需要定义泛化的隐式转换
 * 2. 泛化的copy与assignment函数，并不会阻止编译器生成他们自己的 copy与assignment函数
 */

//1. 了解 template 不同具现体之间的转换与 模板类型之间的转换并不兼容

using namespace std;

class Top {
	public:
		virtual ~Top()  = default;
};
class Middle: public Top {

};
class Bottom: public Middle 
{

};

template <typename T>
class SmartPtr {
	public:
		explicit SmartPtr(T *real_ptr)
			: real_ptr_(real_ptr)
		{ }

		~SmartPtr() {
			delete real_ptr_;
		}
	private:
		T *real_ptr_;
};

void Test1() {
	Top *pt1 = new Middle();
	Top *pt2 = new Bottom();
	const Top *pct2 = pt1;

	delete pt1;
	delete pt2;

	//  SmartPtr<Top> pt3 = SmartPtr<Middle>(new Middle());  //错误
}

//写出一个泛化copy构造函数来兼容，限制工作交给实际的类型去转换


template <typename T>
class SmartPtr2 {
	public:
		explicit SmartPtr2(T *real_ptr)
			: real_ptr_(real_ptr)
		{
		
			cout<<"SmartPtr2 构造函数"<<endl;
			if (real_ptr != nullptr){
				_count = new size_t(1);
				cout<<"SmartPtr2 构造函数  count "<< *(this->_count) <<endl;
			}
			
		}

		//泛化模板函数，如果 rhs 的内部指针能转换为 real_ptr_，则编译通过，限制工作交付给内置类型
		//如果 u 类型能转化成 T类型
		template<typename U>
			SmartPtr2(const SmartPtr2<U> &rhs)
			: real_ptr_(rhs.get()), _count(rhs.getcount())
			{ 
				cout<<"泛化模板"<<endl;
				(*(this->_count))++;
				cout<<"泛化模板  count "<< *(this->_count) <<endl;
			}

		T* get() const { return real_ptr_; }
		size_t* getcount() const { return _count; }

		~SmartPtr2() {
			cout<<"real_ptr_ "<<real_ptr_<<endl;
			(*(this->_count))--;
			if (this->real_ptr_ != nullptr &&  *this->_count ==0)
			{
					delete real_ptr_;
					this->real_ptr_=nullptr;
					
					cout<<"析构  count "<< *(this->_count) <<endl;
			}
			
		}
	private:
		T *real_ptr_;
		size_t* _count;
};

void Test2() {
	//SmartPtr2<Middle>(new Middle());
	SmartPtr2<Top> ptr = SmartPtr2<Middle>(new Middle());

}




//2. 泛化的 copy 构造函数并不会阻止编译器生成它们自己的 copy 构造函数，
// 如果你想完完全全控制 copy动作，请写出泛化版本与非泛化版本

int main() {
	//Test1();
	Test2();
}

/* 总结：
 * 1. 请使用 member function templates（成员函数模板）生成 ”可接受所有兼容类型“ 的函数
 * 2.  泛化的 copy 构造函数并不会阻止编译器生成它们自己的 copy 构造函数，
 * 如果你想完完全全控制 copy动作，请写出泛化版本与非泛化版本
 */
