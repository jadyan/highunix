#include<iostream>
using namespace std;

/*
条款34：区分接口继承和实现继承
成员函数的接口总是会被继承；
声明一个 pure virtual 函数的目的是为了让 derived classes 只继承函数接口；
声明简朴的（非纯）impure virtual 函数的目的是让 derived classes 继承该函数的接口和缺省实现；
声明 non-virtual 函数的目的是为了让 derived classes 继承函数的接口及一份强制性的实现；

*/

// Represents airports
class Airport
{
};


class Airplane
{
public:
	virtual void fly(const Airport& destination) = 0;

protected:
	void defaultFly(const Airport& destination);
};

void Airplane::defaultFly(const Airport& destination)
{
	std::cout<<"Airplane::defaultFly"<<endl;
	// default code for flyaing an airplane to the given destination.
}


class ModelA : public Airplane
{
public:
	virtual void fly(const Airport& destination)
	{
		std::cout<<"ModelA::fly"<<endl;
		defaultFly(destination);
	}
};

class ModelB : public Airplane
{
public:
	virtual void fly(const Airport& destination)
	{
		std::cout<<"ModelB::fly"<<endl;
		defaultFly(destination);
	}
};

class ModelC : public Airplane
{
public:
	virtual void fly(const Airport& destination);
};

void ModelC::fly(const Airport& destination)
{
	std::cout<<"ModelC::fly"<<endl;
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