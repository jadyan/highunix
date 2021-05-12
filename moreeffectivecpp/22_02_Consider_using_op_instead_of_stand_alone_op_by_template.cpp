/*
 * @Author: your name
 * @Date: 2021-04-01 16:15:02
 * @LastEditTime: 2021-04-01 16:49:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/22_Consider_using_op_instead_of_stand_alone_op.cpp
 */


/*
Description: 考虑用运算符的复制形式(+=,-=,op=)取代其单独形式(+,-,op)
*/
#include <iostream>

using namespace std;


template<class T>
const T operator+(const T& lhs,const T&rhs)
{
    cout<<"template T operator+"<<endl;
    return T(lhs) += rhs;

#if 0
    //与前面的程序相同，但是它们之间还是存在重要的差别。第二个模板包含
    //一个命名对象，result。这个命名对象意味着不能在 operator+ 里使用返回值优化（参见
    //条款 M20）。第一种实现方法总可以使用返回值优化，所以编译器为其生成优化代码的可能
    //就会更大。
    T result(lhs); // 拷贝 lhs 到 result 中
    return result += rhs; // rhs 与它相加并返回结果
#endif

}

template<class T>
const T operator-(const T& lhs,const T& rhs)
{
      cout<<"template T operator-"<<endl;
    return T(lhs) -= rhs;
}


class Rational {
public:
    Rational():m_i(0){}
    Rational(int value):m_i(value){}
    Rational(const Rational & tmp)
    {
        this->m_i=tmp.m_i;
        cout<<"拷贝构造函数..."<<endl;
    }
    Rational& operator+=(const Rational& rhs)
    {
        cout<<"operator+= 函数"<<endl;
        this->m_i += rhs.m_i;
    }

    Rational& operator-=(const Rational& rhs)
    {
        cout<<"operator-= 函数"<<endl;
        this->m_i -= rhs.m_i;
    }

    void print()
    {
        cout<<"m_i = "<<m_i<<endl;
    }
private:
    int m_i;
};





int main(int argc, char ** argv)
{
    Rational r1(30);
    Rational r2(40);
    r1 +=r2;
    r1.print();

    

cout<<"--------------------------------------------"<<endl;
    Rational r3 = r1-r2;
    r3.print();

    Rational r4 = r3+ r2;
    r4.print();

}