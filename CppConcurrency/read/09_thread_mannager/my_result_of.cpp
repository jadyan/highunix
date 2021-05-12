/*
 * @Author: your name
 * @Date: 2021-03-08 14:15:42
 * @LastEditTime: 2021-03-08 15:40:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /jadyan/code/book/CppConcurrency/read/09_thread_mannager/my_result_of.cpp
 * 
 * https://blog.csdn.net/qq_31175231/article/details/77165279
 */
#include <iostream>
#include <type_traits>
#include <functional>


//int i = 4;
//decltype(i) a; //推导结果为int。a的类型为int。

int fn(int) {return int();}                            // function
typedef int(&fn_ref)(int);                             // function reference
typedef int(*fn_ptr)(int);                             // function pointer
struct fn_class { int operator()(int i){return i;} };  // function-like class
/*
   type：可调用对象的返回类型.(主要用该成员来获得结果)
   */
int main() {

	int i;
	decltype(fn) a;

	std::cout<<"decltype(fn) = "<<a<<std::endl;

	typedef std::result_of<decltype(fn)&(int)>::type A;  // int
	typedef std::result_of<fn_ref(int)>::type B;         // int
	typedef std::result_of<fn_ptr(int)>::type C;         // int
	typedef std::result_of<fn_class(int)>::type D;       // int

	std::cout << std::boolalpha;
	std::cout << "typedefs of int:" << std::endl;
	/*
	   std::is_same 判断类型是否一致
	 * */
	std::cout << "A: " << std::is_same<int,A>::value << std::endl;
	std::cout << "B: " << std::is_same<int,B>::value << std::endl;
	std::cout << "C: " << std::is_same<int,C>::value << std::endl;
	std::cout << "D: " << std::is_same<int,D>::value << std::endl;

	return 0;
}
