/*
 * @Author: your name
 * @Date: 2021-05-06 14:41:17
 * @LastEditTime: 2021-05-06 14:42:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/26_02_static_Limiting_the_number_of_objects_of_a_class.cpp
 */
#include<iostream>
 
class Printer
{
public:
	static Printer& thePrinter();
	void print();
private:
	Printer();
	Printer(const Printer& rhs);
};

Printer::Printer(){
	std::cout << "Printer()" << std::endl;
}

Printer::Printer(const Printer& rhs){
	std::cout << "Printer(const Printer& rhs)" << std::endl;
}

void Printer::print(){
	std::cout << "print..." << std::endl;
}

Printer& Printer::thePrinter()
{
	static Printer p;
	return p;
}



int main(int argc, char ** argv)
{
	Printer::thePrinter().print();
	Printer::thePrinter().print();
 
	return 0;
}