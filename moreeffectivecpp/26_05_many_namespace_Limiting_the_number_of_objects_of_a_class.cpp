/*
 * @Author: your name
 * @Date: 2021-05-07 09:14:49
 * @LastEditTime: 2021-05-07 09:27:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/26_05_many_namespace_Limiting_the_number_of_objects_of_a_class.cpp
 */

#include<iostream>

using namespace std;
 
class Printer{
public:
	class TooManyObjects{};//当外界申请太多对象时，抛出这种异常类  
	static Printer* makePrinter();
	~Printer();
	void print();
    Printer(const Printer& rhs);
private:
	static size_t numObjects;
	Printer();
	//Printer(const Printer& rhs);//不要定义此函数，因为不希望允许复制行为
	//Printer(const Printer& rhs)
    //{
    //    cout<<"拷贝构造"<<endl;
    //}
};


size_t Printer::numObjects = 0;

Printer::Printer(){
	try{
		if (numObjects >= 1)//这里限制个数为1，可根据需要改
			throw TooManyObjects();
	}
	catch (TooManyObjects& e){
		std::cout << "对象个数超过限制" << std::endl;
		exit(1);
	}
	std::cout << "Printer()" << std::endl;
	++numObjects;
}
Printer* Printer::makePrinter(){
	return new Printer;
}
Printer::~Printer(){
	std::cout << "~Printer()" << std::endl;
	--numObjects;
}
 
void Printer::print(){
	std::cout << "print..." << std::endl;
}
 


 
int main(){
	//Printer p1;//错误，默认构造函数是private
	Printer* p2 = Printer::makePrinter();
	p2->print();
    cout<<"---"<<endl;
	Printer* p3 = p2;//这里是指针赋值  不会调用拷贝构造       ----错误，拷贝构造函数是private且没定义
    //Printer  p32 = *p2;  //错误，拷贝构造函数是private且没定义
    cout<<"---"<<endl;
	delete p2;
 
	Printer* p4 = Printer::makePrinter();
	p4->print();
	delete p4;
 
	return 0;
}