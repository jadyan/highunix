#include <iostream>

using namespace std;

/*
 * @Author: your name
 * @Date: 2021-03-22 22:04:45
 * @LastEditTime: 2021-03-22 22:23:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/06_increment_decrement.cpp
 */
class UPInt 
{ // unlimited precision int
public:
    UPInt():i(0)
    {
        std::cout<<"初始化"<<std::endl;
    }
	// 注意：前缀与后缀形式返回值类型是不同的，引前缀形式返回一个用，后缀形式返回一个const类型
	UPInt& operator++() // ++前缀
	{
		//*this += 1; // 增加
		i += 1;
		return *this; // 取回值
	}

	const UPInt operator++(int) // ++后缀
	{
		// 注意：建立了一个显示的临时对象，这个临时对象必须被构造并在最后被析构，前缀没有这样的临时对象
		UPInt oldValue = *this; // 取回值
		// 后缀应该根据它们的前缀形式来实现
		++(*this); // 增加
        //this->i++;
		return oldValue; // 返回被取回的值
	}




	UPInt& operator--() // --前缀
	{
		i -= 1;
		return *this;
	}

	const UPInt operator--(int) // --后缀
	{
		UPInt oldValue = *this;
		--(*this);
		return oldValue;
	}

	UPInt& operator+=(int a) // +=操作符，UPInt与int相运算
	{
		i += a;
		return *this;
	}

	UPInt& operator-=(int a)
	{
		i -= a;
		return *this;
	}

    void print()
    {
        cout<<"this i = "<<this->i<<endl;
    }

private:
	int i;
}; 

int test_item_6()
{
#if 0
	UPInt i;
    ++i; // 调用i.operator++();
	i++; // 调用i.operator++(0);
	--i; // 调用i.operator--();
	i--; // 调用i.operator--(0);
#endif

	UPInt i;
    UPInt tmp;
    tmp = ++i; // 调用i.operator++();
    tmp.print();

    UPInt i2;
	tmp = i2++; // 调用i.operator++(0);
    tmp.print();
    
	--i; // 调用i.operator--();
	i--; // 调用i.operator--(0);

	//i++++; // 注意：++后缀返回的是const UPInt 
   // 相当于i.operator++(0).operator++(0)

	return 0;
}


int main()
{
    test_item_6();
    return 0;
}