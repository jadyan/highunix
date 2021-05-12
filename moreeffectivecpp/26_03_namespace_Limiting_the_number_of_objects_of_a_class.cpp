/*
 * @Author: your name
 * @Date: 2021-05-06 14:50:27
 * @LastEditTime: 2021-05-06 15:15:00
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/moreeffectivecpp/26_03_namespace_Limiting_the_number_of_objects_of_a_class.cpp
 */
#include <iostream>

using namespace std;

namespace PrintingStuff {

	class Printer{
		public:
			friend Printer& thePrinter();
            void print(){
                std::cout << "print..." << std::endl;
            }
		private:
			//avoid construct
			Printer();
			Printer(const Printer&);

	};

	Printer::Printer()
	{
		cout << "Printer::Printer() was called" << endl;
	}

	//should not use inline here
	//because inline will lead to multi copy of functions
	//while static indicate only one copy
	//so don't inline functions has local static object
	//refer to more effective c++ page 101
	Printer& thePrinter(){
		static Printer p;
		return p;
	}
}


int main(int argc, char ** argv)
{
    PrintingStuff::thePrinter().print();
}