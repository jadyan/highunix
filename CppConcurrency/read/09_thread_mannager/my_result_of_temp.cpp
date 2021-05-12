
#include <iostream>
#include <functional>
 
using namespace std;
using namespace std::placeholders;
 
typedef double(*FUNC)(double);
 
double func(double d)
{
	return d*d;
}
 
int main()
{
	typedef result_of<FUNC(double)>::type my_type;
	result_of<FUNC(double)>::type d = func(9.0);

	std::cout << std::boolalpha;
	std::cout << "tyep: " << std::is_same<double,my_type>::value << std::endl;
	std::cout << "d: " <<d << std::endl;
	return 0;
}

