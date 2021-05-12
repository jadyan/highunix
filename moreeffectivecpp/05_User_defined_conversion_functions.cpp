#include <iostream>
#include <string>


using namespace std;

class Name {
public:
	Name(const std::string& s); // 转换string到Name
};

class Rational {
public:
	Rational(int numerator = 0, int denominator = 1) // 转换int到有理数类
	{
		n = numerator;
		d = denominator;
	}

	operator double() const // 转换Rational类成double类型
	{
        cout<<"转换城double"<<endl;
		return static_cast<double>(n) / d;
	}

	double asDouble() const
	{
		return static_cast<double>(n) / d;
	}

private:
	int n, d;
};

//另外可能发生隐式转换的地方
template<class T>
class Array
{
public:
    //另外一种解决方式是把参数n用一个新类表示
    explicit Array(int n):lb(0),hb(n-1)
    {
        data = new T[n];
    }
    Array(int lowBound,int highBound):
        lb(lowBound),
        hb(highBound)
    {
        data  = new T[highBound+1];
    }
    T& operator[](int index)
    {
        return data[index];
    }

private:
    int lb;
    int hb;
    T * data;
};




template<class T>
class Array1
{
public:
    //另外一种解决方式是把参数n用一个新类表示
    //嵌套类
    class ArraySize{
        public:
            ArraySize(int n): _size(n)
            {
            }

            int size() const {return _size;}
        private:
            int _size;
    };
    //将int 变量转换城arraysize类型
    Array1(ArraySize size):
        lb(0),
        hb(size.size()-1)
    {
        cout << "call Array1 ctor" << endl;
        data = new T[size.size()];
    }
    Array1(int lowBound,int highBound):
        lb(lowBound),
        hb(highBound)
    {
        data  = new T[highBound+1];
    }
    T& operator[](int index)
    {
        return data[index];
    }

private:
    int lb;
    int hb;
    T * data;
};

bool operator==(const Array<int>&lhs,const Array<int>&rhs)
{
    //TODO
    cout << "调用Array1==了"<< endl;
    return false;
}

bool operator==(const Array1<int>&lhs,const Array1<int>&rhs)
{
    //TODO
    cout << "调用Array1==了"<< endl;
    return false;
}


int test_item_5()
{
	Rational r(1, 2); // r的值是1/2
	double d = 0.5 * r; // 转换r到double,然后做乘法
    cout<<"d="<<d<<endl;
	//fprintf(stdout, "value: %f\n", d);


    //会调用operator double();


	std::cout<<r<<std::endl; // 应该打印出"1/2",但事与愿违,是一个浮点数，而不是一个有理数,隐式类型转换的缺点
				 // 解决方法是不使用语法关键字的等同的函数来替代转换运算符,如增加asDouble函数，去掉operator double
    std::cout<<r.asDouble()<<std::endl;


    Array<int> a(10);
    cout << a[2] << endl;
    Array<int>b(10);
    
#if 0
//加了 explicit 就会报错
    for(int i=0;i<10;++i)
    {
        //这里如果手贱把a[i]写成了a，那么会调用上述重载的operator==,把b[i]隐式转换为Array<int>了
        //可以在构造函数使用explicit避免隐式调用
        //不加explicit 会隐式转换成
        //    if(a==static_cast<Array<int>>(b[i]))
        if(a == b[i]) 
            cout << "equal" <<endl;
        else
            cout << "no equal" << endl;
    }
#endif  

    Array1<int>a1(10);
    Array1<int>b1(10);
    for(int i = 0;i<10;++i)
    {
        /*这个时候没法把b1[i]隐式转换为ArraySize，然后再转换为Array<int>
        Array1没有一个用int类型的作为参数，编译器不能转换两次 （先转化成ArraySize，再转换城array1）
        */

        //if(a1 == b1[i]) 
        if(a1==b1)
            //这是不被编译器允许的
            cout << "equal " << endl;
        else
            cout << "no equal" << endl;
    }


	return 0;
}


int main(int argc , char **argv)
{
    test_item_5();
    return 0;
}