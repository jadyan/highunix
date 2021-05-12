/*
 * @Author: your name
 * @Date: 2021-05-06 14:22:07
 * @LastEditTime: 2021-05-07 09:49:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/26_Limiting_the_number_of_objects_of_a_class.cpp
 * https://blog.csdn.net/weixin_30856965/article/details/95433125?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-9.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-9.control
 */
#include<iostream>
 
class Printer{
public:
	friend Printer& thePrinter();
	void print();
private:
	Printer();
	Printer(const Printer& rhs);
};


Printer::Printer(){
	std::cout << "Printer() 构造函数" << std::endl;
}

Printer::Printer(const Printer& rhs){
	std::cout << "Printer(const Printer& rhs)" << std::endl;
}

void Printer::print(){
	std::cout << "print..." << std::endl;
}

Printer& thePrinter(){
	static Printer p;
	return p;
}

 
int main()
{
    thePrinter().print();
    thePrinter().print();
    thePrinter().print();
    thePrinter().print();

    return 0;
}