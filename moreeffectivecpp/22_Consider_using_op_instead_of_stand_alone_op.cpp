/*
 * @Author: your name
 * @Date: 2021-04-01 16:15:02
 * @LastEditTime: 2021-04-01 16:37:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/22_Consider_using_op_instead_of_stand_alone_op.cpp
 */
#include <iostream>

using namespace std;

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



// operator+根据operator+=实现
const Rational operator+(const Rational& lhs, const Rational& rhs)
{
	return Rational(lhs) += rhs;
}

// operator-根据operator-=实现
const Rational operator-(const Rational& lhs, const Rational& rhs)
{
    cout<<"operator- 函数"<<endl;
	return Rational(lhs) -= rhs;

}


int main(int argc, char ** argv)
{
    Rational r1(30);
    Rational r2(40);
    r1 +=r2;
    r1.print();

cout<<"--------------------------------------------"<<endl;
    Rational r3 = r1-r2;
    r3.print();

}