#include <iostream>
/*
Item 30: 理解 inline 化的介入和排除

inline 函数无法随着程序库的升级而升级。即如果 f 是程序库内的一个 inline 函数，一旦程序库的设计者决定改变 f，
所有用到 f 的客户端程序都必须重新编译；但如果 f 是 non-inline 函数，则只需要重新链接即可。

*/

class Person
{
public:
	// an implicit inline request:
	// age is defined in a class definition.
	int age() const { return theAge; }

private:
	int theAge;
};


// assume compilers are willing to inline calls to f
inline void f()
{
}

// pf points to f
void (*pf)() = f;


int main()
{
	// this call will be inlined because it's a "normal" call.
	f();

	// this call probably won't be because it's through a function pointer
	pf();
	return 0;
}
