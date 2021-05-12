/*
 * @Author: your name
 * @Date: 2021-03-05 08:56:07
 * @LastEditTime: 2021-03-05 08:56:07
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/09_thread_mannager/templatetest.cpp
 */
#include <iostream>

using namespace std;
/*
模板函数自动推导
*/


class templatetest{
private:
	int b;
public:
	templatetest():b(3)
	{
		cout<<"构造函数"<<endl;
	}; 
	template<typename T>
	T fun(T tmpb)
	{
		//b=tmpb;
		cout<<"temp 函数"<<endl;
		return ++tmpb;
	}

};



int main()
{
	templatetest ct;
	cout<<"ct.fun(2.7) = "<< ct.fun(2.7)<<endl;
	cout<<"ct.fun(9) = "<< ct.fun(9)<<endl;
}
