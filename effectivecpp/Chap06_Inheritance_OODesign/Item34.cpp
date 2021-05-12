#include<iostream>
using namespace std;

// 条款34：区分接口继承和实现继承
// Differentiate between inheritance of interface and inheritance of implementation.

// public 继承由两部分组成：函数接口（function interfaces）继承和函数实现（function implementations）继承。而不同类型的函数继承程度也是不一样的：

// 成员函数的接口总是会被继承；
// 声明一个 pure virtual 函数的目的是为了让 derived classes 只继承函数接口；
// 声明简朴的（非纯）impure virtual 函数的目的是让 derived classes 继承该函数的接口和缺省实现；
// 声明 non-virtual 函数的目的是为了让 derived classes 继承函数的接口及一份强制性的实现；

// Represents airports
class Airport
{
};


class Airplane
{
public:
	virtual void fly(const Airport& destination) = 0;
};

// An implementation of pure virtual function.
void Airplane::fly(const Airport& destination)
{
	cout<<"Airplane::fly"<<endl;
	// default code for flyaing an airplane to the given destination.
}


class ModelA : public Airplane
{
public:
	virtual void fly(const Airport& destination)
	{
		Airplane::fly(destination);
	}
};

class ModelB : public Airplane
{
public:
	virtual void fly(const Airport& destination)
	{
		cout<<"ModelB::fly"<<endl;
		Airplane::fly(destination);
	}
};

class ModelC : public Airplane
{
public:
	virtual void fly(const Airport& destination);
};

void ModelC::fly(const Airport& destination)
{
	cout<<"ModelC::fly"<<endl;
	// code for flyaing a ModelC airplane to the given destination.
}

int main()
{
	Airport a;

	Airplane* b = new ModelB;
	b->fly(a);

	Airplane* c = new ModelC;
	c->fly(a);

	//Airplane air;

	delete c;
	delete b;

	return 0;
}