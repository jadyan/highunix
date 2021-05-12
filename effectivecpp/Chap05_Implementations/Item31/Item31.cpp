#include <memory>
#include <string>
#include "Person3.h"
#include "RealPerson3.h"
#include "Date.h"
#include "Address.h"

/*
最小化文件之间的编译依赖

handle class：成员函数必须通过 implementation pointer 取得对象数据；
	空间：需要消耗一个 implementation pointer 的大小，一个动态分配的 implementation object 大小；
	时间：动态申请和释放内存的时间开销，每次访问的指针取址跳转；
	安全：需要面对内存分配的 bad_alloc 异常；

interface class：每个函数都是 virtual；
	空间：派生对象必须内含一个 vptr；
	时间：每次调用需要付出一次间接跳跃（indirect jump）；
*/

int main()
{
	std::string n = "hello";
	Date d;
	Address a;

	std::shared_ptr<Person3> pp(RealPerson3::create(n, d, a));
	return 0;
}
