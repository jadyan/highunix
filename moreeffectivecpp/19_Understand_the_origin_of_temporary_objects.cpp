/*
 * @Author: your name
 * @Date: 2021-04-01 10:24:31
 * @LastEditTime: 2021-04-01 11:16:00
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/19_Understand_the_origin_of_temporary_objects.cpp
 */
#include <iostream>
#include <string>
#include <iomanip>



/*
//c++中真正的临时对象是看不见的
//建立未命名的非堆对象会产生临时对象：
//(1)函数参数的隐式类型转换
//(2)函数返回对象

*/

using namespace std;

void swap(int a,int b)
{
    int tmp = a; //这里的tmp不是临时变量，而是局部对象
    a = b;
    b = tmp;
}


size_t countChar(const std::string& str, char ch)
{
	// 建立一个string类型的临时对象，通过以buffer做为参数调用string的构造函数来初始化这个临时对象,
	// countChar的参数str被绑定在这个临时的string对象上，当countChar返回时，临时对象自动释放

	// 将countChar(const std::string& str, char ch)修改为countChar(std::string& str, char ch)则会error
	return 1;
}

void testsetw()
{
    cout<<"---------------------------testsetw start------------------------------------------"<<endl;
    // 开头设置宽度为 4，后面的 runoob 字符长度大于 4，所以不起作用
    cout << setw(4) << "runoob" << endl;
    // 中间位置设置宽度为 4，后面的 runoob 字符长度大于 4，所以不起作用
    cout << "runoob" << setw(4) << "runoob" << endl;
    // 开头设置间距为 14，后面 runoob 字符数为6，前面补充 8 个空格
    cout << setw(14) << "runoob" << endl;
    // 中间位置设置间距为 14 ，后面 runoob 字符数为6，前面补充 8 个空格
    cout << "runoob" << setw(14) << "runoob" << endl;
    cout<<"---------------------------testsetw end------------------------------------------"<<endl;
}



size_t ChangeChar(  std::string& str)
{
    str[2]='j';
	return 1;
}


class base{
public:
    base(int i=3):m(i){}
    base(const base & tmp)
    {
        this->m= tmp.m;
    }
    ~base(){}
private:
    int m;
};


void classtmpobjecttest(base & b)
{
    cout<<"call b addr = "<<&b<<endl;
}

void cosntclasstmpobjecttest(const base & b)
{
    cout<<"call const b addr = "<<&b<<endl;
}

int test_item_19()
{
    #define MAX_STRING_LEN 64
	char buffer[MAX_STRING_LEN];
	char c;

    //setw设置宽度 代表执行最多输入64个字符
	std::cin >> c >> setw(MAX_STRING_LEN) >> buffer; 
    cout<<"buffer: ["<<buffer<<"] end"<<endl;

    /*
        对应函数的正被绑定
        的参数的类型是 const string&。仅当消除类型不匹配后，才能成功进行这个调用，你的编
        译器很乐意替你消除它，方法是建立一个 string 类型的临时对象。通过以 buffer 做为参数
        调用 string 的构造函数来初始化这个临时对象。countChar 的参数 str 被绑定在这个临时
        的 string 对象上。当 countChar 返回时，临时对象自动释放
    */
    //仅当通过传值（by value）方式传递对象或传递常量引用（reference-to-const）参数
    //时，才会发生这些类型转换。 char[] 先转换成一个临时的变量str变量
	std::cout<<"There are "<<countChar(buffer, c)<<" occurrences of the character "<<c<<" in "<<buffer<<std::endl;


   char  str[]="jadyan";
   cout<<"str = "<<str<<endl;
   //仅当通过传值（by value）方式传递对象或传递常量引用（reference-to-const）参数
    //时，才会发生这些类型转换。 char[] 先转换成一个临时的变量str变量
    /*
        当程
        序员期望修改非临时对象时，对非常量引用（references-to-non-const）进行的隐式类型
        转换却修改临时对象。这就是为什么 C++语言禁止为非常量引用（reference-to-non-const）
        产生临时对象
    */
   //ChangeChar(str);
   cout<<"str2 = "<<str<<endl;

	return 0;
}



int main(int argc, char ** argv)
{
    testsetw();
    test_item_19();


    base b;
    cout<<"b addr = "<<&b<<endl;
    classtmpobjecttest(b);
    //不会产生临时变量 没有涉及到类型转换
    cosntclasstmpobjecttest(b);
}