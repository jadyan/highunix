/*
 * @Author: your name
 * @Date: 2021-04-01 13:45:46
 * @LastEditTime: 2021-04-01 14:13:55
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/20_Facilitate_the_return_value_optimization.cpp
 */
#include<iostream>
#include <string>

using namespace std;
class Rational
{
public:
    Rational(int numerator = 0, int denominator = 1) :
      n(numerator), d(denominator)
    {
        cout<<"构造函数"<<endl;
    }

    Rational(const Rational & rational)
    {
        this->n=rational.n;
        this->d= rational.d;
        cout<<"拷贝构造函数"<<endl;
    }

    int numerator() const { return n; }
    int denominator() const { return d; }
    void show()
    {
        std::cout<<"n="<<n<<std::endl;
        std::cout<<"d="<<d<<std::endl;
    }
private:
    int n, d;
};


const Rational operator*(const Rational& lhs, const Rational& rhs)
{
    return Rational(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}



int main(int argc, char ** argv)
{
	Rational a = (10,9);
	Rational b(1, 2);

    /* 
        编译器就会被允许消除在 operator*内的临时变量和 operator*返回的临时变量。它们
        能在为目标 c 分配的内存里构造 return 表达式定义的对象。如果你的编译器这样去做，调
        用 operator*的临时对象的开销就是零：没有建立临时对象
        这里允许返回临时变量 编译器优化
    */
	Rational c = a * b; 
    c.show();
	return 0;
}