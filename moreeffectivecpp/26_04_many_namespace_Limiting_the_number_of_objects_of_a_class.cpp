/*
 * @Author: your name
 * @Date: 2021-05-06 15:24:46
 * @LastEditTime: 2021-05-07 09:10:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/26_04_many_namespace_Limiting_the_number_of_objects_of_a_class.cpp
 */
#include<iostream>
using namespace std;
 
class Printer{
public:
	class TooManyObjects{};//当外界申请太多对象时，抛出这种异常类  
	Printer();
	~Printer();
	void print();
private:
	static size_t numObjects;
	Printer(const Printer& rhs);//这里限制个数为1，所以不允许复制
};

size_t Printer::numObjects = 0;
Printer::Printer(){
#if 1
	try{
		if (numObjects >= 1)//这里限制个数为1，可根据需要改
			throw TooManyObjects();
	}
	catch (TooManyObjects& e){
		std::cout << "对象个数超过限制" << std::endl;
		exit(1);
	}
#endif
	std::cout << "Printer()" << std::endl;
	++numObjects;
}

Printer::~Printer(){
	std::cout << "~Printer()" << std::endl;
	--numObjects;
}

Printer::Printer(const Printer& rhs){
	std::cout << "Printer(const Printer& rhs)" << std::endl;
}

void Printer::print(){
	std::cout << "print..." << std::endl;
}



class ColorPrinter :public Printer{
public:
    ColorPrinter()
    {
        cout<<"ColorPrinter 构造"<<endl;
    }
    /*
    隐藏父类的print(), 没有virtual
    */
    void print(int i)
    {
        cout<<"ColorPrinter print 函数"<<endl;
    }
};

int main(int argc, char ** argv){
	Printer printer1;
	printer1.print();
	//Printer printer2;//导致程序中断
	//printer2.print();


    //ColorPrinter cp;
    //cp.print();
	return 0;
}