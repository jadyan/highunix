#include <iostream>
#include <memory>
#include <string>
#include "Person3.h"
#include "Date.h"
#include "Address.h"

class RealPerson3 : public Person3
{
public:
	RealPerson3() {}

	//只要不使用可以编译过
	RealPerson3(const std::string& name, const Date& birthday);

	RealPerson3(const std::string& name, const Date& birthday, const Address& addr) :
		theName(name), theBirthDate(birthday), theAddress(addr)
	{
		using namespace std;
		cout<<"RealPerson3 构造"<<endl;
	}
	
	virtual ~RealPerson3() {}

/*
https://www.kancloud.cn/jerakrs/effective-c/1752206
除非 Interface 类的接口发生变化，否则 Interface 类的客户不需要重新编译
https://www.cnblogs.com/xiaojianliu/articles/12306444.html
*/
	static std::shared_ptr<Person3> create(const std::string& name, const Date& birthday, const Address& addr)
	{
		std::cout<<"创建"<<std::endl;
		return std::shared_ptr<Person3>(new RealPerson3(name, birthday, addr));
	}

	std::string name() const		{ return theName; }
	std::string birthDate() const	{ return "date"; }
	std::string address() const		{ return "addr"; }

private:
	std::string theName;
	Date theBirthDate;
	Address theAddress;
};
