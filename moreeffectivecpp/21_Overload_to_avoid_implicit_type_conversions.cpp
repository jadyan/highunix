/*
 * @Author: your name
 * @Date: 2021-04-01 15:28:29
 * @LastEditTime: 2021-04-01 15:54:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/21_Overload_to_avoid_implicit_type_conversions.cpp
 */
#include <iostream>

using namespace std;

class UPInt { // unlimited precision integers class
public:
	UPInt():m_i(0) {}
	UPInt(int value):m_i(value) 
    {
        cout<<"构造函数"<<endl;
    }
    UPInt(const UPInt & tmp)
    {
        this->m_i= tmp.m_i;
        cout<<"拷贝构造函数"<<endl;
    }
    void print()
    {
        cout<<"m_i = "<<m_i<<endl;
    }
    friend const UPInt operator+(const UPInt & lhr, const UPInt &rhs);
    friend const UPInt operator + (const UPInt & lhr, int rhs );
    friend const UPInt operator + (int lhr, const UPInt &rhs);
private:
    int m_i;
};

const UPInt operator+(const UPInt & lhr, const UPInt &rhs)
{
    cout<<"两个对象重载"<<endl;
    return UPInt(lhr.m_i+rhs.m_i);
}
#if 1
const UPInt operator + (const UPInt & lhr, int rhs )
{
    cout<<"前面是对象，后面是int"<<endl;
    return UPInt(lhr.m_i+rhs);
}

const UPInt operator + (int lhr, const UPInt &rhs)
{
    cout<<"前面是int，后面是对象"<<endl;
    return UPInt(lhr+rhs.m_i);
}
#endif


int main()
{
    UPInt up1(18);
    UPInt up2(19);
    // 正确,没有由 upi1 或 upi2 生成的临时对象
    UPInt up3=up1+up2;
    up3.print();

cout<<"-------------------------------------"<<endl;
/*
这些语句也能够成功运行。
如果没有重载const UPInt operator + (const UPInt & lhr, int rhs )
方法是通过建立临时对象把整形数 10 转换为 UPInts

若果重载了 const UPInt operator + (const UPInt & lhr, int rhs )
就会直接调用 operator + (const UPInt & lhr, int rhs ) 不会产生临时对象
*/
    UPInt upi3;
    //正确, 没有由 upi1 or 10 生成的临时对象
    upi3 = up1 + 10;
    upi3.print();
cout<<"-------------------------------------"<<endl;
    UPInt upi4;
    upi4 = 10 +  up2;
    upi4.print();

}