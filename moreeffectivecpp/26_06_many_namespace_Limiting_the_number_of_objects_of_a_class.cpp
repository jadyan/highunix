/*
 * @Author: your name
 * @Date: 2021-05-07 09:31:08
 * @LastEditTime: 2021-05-07 09:31:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/26_06_many_namespace_Limiting_the_number_of_objects_of_a_class.cpp
 */
#include<iostream>

using namespace std;
 
class Printer{
public:
	class TooManyObjects{};//当外界申请太多对象时，抛出这种异常类  
	static Printer* makePrinter();
	static Printer* makePrinter(const Printer& rhs);
	~Printer();
	void print();
private:
	static size_t numObjects;
	static const size_t maxObjects = 10;
	Printer();
	Printer(const Printer& rhs);
};
size_t Printer::numObjects = 0;
const size_t Printer::maxObjects;
Printer::Printer(){
	try{
		if (numObjects >= maxObjects)
			throw TooManyObjects();
	}
	catch (TooManyObjects& e){
		std::cout << "对象个数超过限制" << std::endl;
		exit(1);
	}
	std::cout << "Printer()" << std::endl;
	++numObjects;
}
Printer::Printer(const Printer& rhs){
	try{
		if (numObjects >= maxObjects)
			throw TooManyObjects();
	}
	catch (TooManyObjects& e){
		std::cout << "对象个数超过限制" << std::endl;
		exit(1);
	}
	std::cout << "Printer(const Printer& rhs)" << std::endl;
	++numObjects;
}
Printer* Printer::makePrinter(){
	return new Printer;
}
Printer* Printer::makePrinter(const Printer& rhs){
	return new Printer(rhs);
}
Printer::~Printer(){
	std::cout << "~Printer()" << std::endl;
	--numObjects;
}
 
void Printer::print(){
	std::cout << "print..." << std::endl;
}
 


 
int main(){
	Printer* p1 = Printer::makePrinter();
	p1->print();
	Printer* p2 = Printer::makePrinter(*p1);
	p2->print();
	delete p2;
	delete p1;
	return 0;
}