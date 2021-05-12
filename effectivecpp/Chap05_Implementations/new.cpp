#include <iostream>
#include <new>
using namespace std;



//int *pi=new int;         // 默认初始化， *pi的值为未定义。  
//int *pi=new int();       // 值初始化为0，*pi的值为0  
//string *ps=new string;   // 默认初始化 *ps的值为空string  
//string *ps=new string(); // 值初始化为空string

int main(int argc, char **argv)
{
	int *p = new int;

	*p=27;
	cout<<"p = "<<*p<<" adress=%x"<<p<<endl;
	int *p2= new int;
	cout<<"p2 = "<<*p2<<" adress=%x"<<p2<<endl;

	int *p3= new int(*p);
	cout<<"p3 = "<<*p3<<" adress=%x"<<p3<<endl;
}
